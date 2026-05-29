/*
    LF_Alpha RGB LEDs Blink Example
    This example demonstrates how to initialize and use the LF_Alpha RGB LEDs.
    available methods:
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
    Serial.println("LF_Alpha RGB LEDs Blink Example");

    // Initialize the RGB LEDs
    rgbLEDs.begin();
    Serial.println("RGB LEDs initialized");
}

void loop(){
    // Blink all Red
    for(int i = 0; i < 5; i++){
        rgbLEDs.blink(i, 255, 0, 0, 1000);
        rgbLEDs.show();
        delay(1000);
    }

    // Blink all Green
    for(int i = 0; i < 5; i++){
        rgbLEDs.blink(i, 0, 255, 0, 1000);
        rgbLEDs.show();
        delay(1000);
    }

    // Blink all Blue
    for(int i = 0; i < 5; i++){
        rgbLEDs.blink(i, 0, 0, 255, 1000);
        rgbLEDs.show();
        delay(1000);
    }
}
