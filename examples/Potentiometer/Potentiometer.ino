/*
    LF_Alpha Potentiometer Example
    This example demonstrates how to initialize and use the LF_Alpha_M1 Potentiometer.
    available methods:
        -begin(); Initializes the Potentiometer.
        -read(); Reads the current value of the potentiometer (0-1023 for 10-bit ADC resolution).
        -readAnalog(); Reads the current value of the potentiometer (0-4095 for 12-bit ADC resolution).
        -readVoltage(); Reads the current value of the potentiometer in volts (0-5V).
        -readPercentage(); Reads the current value of the potentiometer in percentage (0-100%).
        -getAdcResolution(); Gets the current ADC resolution.
        -setAdcResolution(uint8_t res); Sets the ADC resolution (default is 12 bits).
        -end(); De-initializes the Potentiometer.
*/

#include <LF_Alpha.h>

// Create an instance of the LF_Alpha_resPot class
LF_Alpha_resPot potentiometer;

void setup(){
    Serial.begin(115200);
    Serial.println("LF_Alpha_M1 Potentiometer Example");

    // Initialize the Potentiometer
    potentiometer.begin();
    Serial.println("Potentiometer initialized");
}

void loop(){
    // Read the potentiometer value
    int potValue = potentiometer.readAnalog();

    // Read the potentiometer value in voltage
    int potValueInVoltage = potentiometer.readVoltage();

    // Read the potentiometer value in percentage
    int potValueInPercentage = potentiometer.readPercentage();

    // Print the potentiometer value to the Serial Monitor
    Serial.print("Potentiometer Value: ");
    Serial.println(potValue);
    Serial.print("Potentiometer Value in Voltage: ");
    Serial.println(potValueInVoltage);
    Serial.print("Potentiometer Value in Percentage: ");
    Serial.println(potValueInPercentage);
    Serial.println("--------------------");

    // Wait for 5 seconds before the next reading
    delay(5000);
}