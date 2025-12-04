/*
    LF_Alpha RGB LEDs Simple Example
    This example demonstrates how to initialize and use the LF_Alpha_M1 RGB LEDs.
    available methods:
        -begin(); Initializes the RGB LEDs.
        -write(n, r, g, b); Writes to the RGB LEDs.
        -clear(); Clears the RGB LEDs.
        -show(); Shows the RGB LEDs.
        -setBrightness(); Sets the brightness of the RGB LEDs.
        -updateLength(); Updates the length of the RGB LEDs.
        -blink(); Blinks the RGB LEDs.
        -rainbow(); Shows a rainbow effect on the RGB LEDs.
        -getPixelColor(n); Gets the color of the RGB LEDs.
        -end(); De-initializes the RGB LEDs.
*/

#include <LF_Alpha.h>

// Create an instance of the LF_Alpha_rgbLEDs class
LF_Alpha_rgbLEDs rgbLEDs;

void setup(){
    Serial.begin(115200);
    Serial.println("LF_Alpha_M1 RGB LEDs Simple Example");

    // Initialize the RGB LEDs
    rgbLEDs.begin();
    Serial.println("RGB LEDs initialized");
}

void loop(){

    for(int i = 0; i < 5; i++){
        rgbLEDs.write(i, 255, 0, 0);
        rgbLEDs.show();
        delay(1000);
        rgbLEDs.write(i, 0, 255, 0);
        rgbLEDs.show();
        delay(1000);
        rgbLEDs.write(i, 0, 0, 255);
        rgbLEDs.show();
        delay(1000);
    }

    // blink all the RGB LEDs
    rgbLEDs.blink(0, 255, 0, 0, 1000);      // red
    rgbLEDs.blink(1, 0, 255, 0, 1000);      // green
    rgbLEDs.blink(2, 0, 0, 255, 1000);      // blue
    rgbLEDs.blink(3, 255, 0, 255, 1000);    // purple
    rgbLEDs.blink(4, 0, 255, 255, 1000);    // cyan
    rgbLEDs.show(); 
    delay(1000);

    // Clear the RGB LEDs
    rgbLEDs.clear();
    rgbLEDs.show();

    // Wait for 5 seconds before the next reading
    delay(5000);
}