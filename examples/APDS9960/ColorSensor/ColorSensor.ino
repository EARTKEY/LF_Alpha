/*
    LF_Alpha APDS9960 Color Sensor Example
    This example shows how to use the color sensor on the LF_Alpha.
    available methods:
        -begin(); Initializes the color sensor.
        -enableColor(); Enables the color sensor.
        -colorDataReady(); Checks if color data is ready.
        -readColors(&red, &green, &blue, &clear); Reads color data from the sensor.
        -getColorName(red, green, blue, clear); Returns a human-readable color name using simple heuristics.
        -getColorNameHSV(red, green, blue, clear); Alternative method converting to HSV and returning a color name.
        -end(); De-initializes the color sensor.
*/

#include <LF_Alpha.h>

// Create an instance of LF_Alpha_APDS9960
LF_Alpha_APDS9960 colorSensor;

// Color name detection function 
String getColorName(uint16_t red, uint16_t green, uint16_t blue, uint16_t clear);

// Alternative: More detailed color detection with HSV conversion
String getColorNameHSV(uint16_t red, uint16_t green, uint16_t blue, uint16_t clear);

void setup()
{
    Serial.begin(115200);
    Serial.println("LF_Alpha APDS9960 Color Detection with Names");

    // Initialize the color sensor
    bool status = colorSensor.begin();
    if (status)
    {
        Serial.println("Color Sensor Initialized");
    }
    else
    {
        Serial.println("Color Sensor Initialization Failed");
        while (1);
    }

    // Enable color sensor
    colorSensor.enableColor();
}

void loop()
{
    uint16_t red, green, blue, clear;

    // Check if color data is ready
    if (colorSensor.colorDataReady())
    {
        // Read color data
        colorSensor.readColors(&red, &green, &blue, &clear);
        
        // Get color name using both methods
        String colorName1 = getColorName(red, green, blue, clear);
        String colorName2 = getColorNameHSV(red, green, blue, clear);
        
        // Print both raw values and color names
        Serial.println("Color Detection Results:");
        
        // Raw RGB values
        Serial.print("Red: ");
        Serial.print(red);
        Serial.print(" | Green: ");
        Serial.print(green);
        Serial.print(" | Blue: ");
        Serial.print(blue);
        Serial.print(" | Clear: ");
        Serial.println(clear);
                
        // Color names
        Serial.print("Detected Color (Method 1): ");
        Serial.println(colorName1);
        
        Serial.print("Detected Color (Method 2-HSV): ");
        Serial.println(colorName2);
        
        Serial.println("--------------------------------");
        Serial.println();
    }
    
    delay(1000);
}

String getColorName(uint16_t red, uint16_t green, uint16_t blue, uint16_t clear) {
    // Step 1: Normalize RGB values to 0-255 range
    // APDS9960 returns 16-bit values, normalize based on clear channel
    float r, g, b;
    
    // Prevent division by zero
    if (clear == 0) {
        return "Dark/No Light";
    }
    
    // Normalize to 0-255 range using clear channel as reference
    r = (float)red / clear * 255.0;
    g = (float)green / clear * 255.0;
    b = (float)blue / clear * 255.0;
    
    // Clamp values to 0-255
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);
    
    // Step 2: Check for grayscale colors (low saturation)
    float maxRGB = max(max(r, g), b);
    float minRGB = min(min(r, g), b);
    float delta = maxRGB - minRGB;
    
    // Calculate brightness
    float brightness = (r + g + b) / 3.0;
    
    // If very low brightness, it's black
    if (brightness < 30) {
        return "Black";
    }
    
    // If very high brightness with low saturation, it's white
    if (brightness > 220 && delta < 30) {
        return "White";
    }
    
    // If low saturation (grayscale), determine gray shades
    if (delta < 30) {
        if (brightness < 80) return "Dark Gray";
        if (brightness < 160) return "Gray";
        return "Light Gray";
    }
    
    // Step 3: Determine dominant color for saturated colors
    // Find which channel is dominant
    if (r > g && r > b) {
        // Red is dominant
        if (g > b * 1.5) {
            return "Orange";
        } else if (g > b) {
            return "Red-Orange";
        } else if (b > g * 1.5) {
            return "Magenta";
        } else {
            return "Red";
        }
    }
    else if (g > r && g > b) {
        // Green is dominant
        if (r > b * 1.5) {
            return "Yellow-Green";
        } else if (r > b) {
            return "Yellow";
        } else if (b > r * 1.5) {
            return "Cyan";
        } else {
            return "Green";
        }
    }
    else if (b > r && b > g) {
        // Blue is dominant
        if (r > g * 1.5) {
            return "Purple";
        } else if (r > g) {
            return "Blue-Purple";
        } else if (g > r * 1.5) {
            return "Cyan-Blue";
        } else {
            return "Blue";
        }
    }
    else {
        // Colors are very close - mixed colors
        if (abs(r - g) < 20 && abs(g - b) < 20) {
            if (brightness > 180) return "White";
            if (brightness > 100) return "Gray";
            return "Dark Gray";
        }
        return "Mixed Color";
    }
}

// Alternative: More detailed color detection with HSV conversion
String getColorNameHSV(uint16_t red, uint16_t green, uint16_t blue, uint16_t clear) {
    // Normalize to 0-255 range
    if (clear == 0) return "Dark/No Light";
    
    float r = (float)red / clear * 255.0;
    float g = (float)green / clear * 255.0;
    float b = (float)blue / clear * 255.0;
    
    r = constrain(r, 0, 255) / 255.0;
    g = constrain(g, 0, 255) / 255.0;
    b = constrain(b, 0, 255) / 255.0;
    
    // Calculate HSV
    float maxRGB = max(max(r, g), b);
    float minRGB = min(min(r, g), b);
    float delta = maxRGB - minRGB;
    
    // Value (brightness)
    float v = maxRGB;
    
    // Saturation
    float s = (maxRGB > 0) ? (delta / maxRGB) : 0;
    
    // Hue
    float h = 0;
    if (delta != 0) {
        if (maxRGB == r) {
            h = 60.0 * fmod(((g - b) / delta), 6.0);
        } else if (maxRGB == g) {
            h = 60.0 * (((b - r) / delta) + 2.0);
        } else {
            h = 60.0 * (((r - g) / delta) + 4.0);
        }
    }
    
    if (h < 0) h += 360.0;
    
    // Determine color name based on HSV
    // Low value = dark colors
    if (v < 0.15) return "Black";
    
    // Low saturation = grayscale
    if (s < 0.15) {
        if (v > 0.85) return "White";
        if (v > 0.60) return "Light Gray";
        if (v > 0.35) return "Gray";
        return "Dark Gray";
    }
    
    // High saturation - determine by hue
    if (h < 15 || h >= 345) return "Red";
    if (h < 45) return "Orange";
    if (h < 75) return "Yellow";
    if (h < 150) return "Green";
    if (h < 210) return "Cyan";
    if (h < 270) return "Blue";
    if (h < 315) return "Purple";
    if (h < 345) return "Magenta";
    
    return "Unknown";
}