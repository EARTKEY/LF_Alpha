/*
    LF_Alpha HCSR04 Ultrasonic Sensor Example
    This example demonstrates how to initialize and use the LF_Alpha_M1 HCSR04 Ultrasonic Sensor.
    available methods:
        -begin(); Initializes the HCSR04 Ultrasonic Sensor.
        -readCM(); Reads the distance measured by the HCSR04 Ultrasonic Sensor in centimeters.
        -readINCH(); Reads the distance measured by the HCSR04 Ultrasonic Sensor in inches.
        -end(); De-initializes the HCSR04 Ultrasonic Sensor.
*/

#include <LF_Alpha.h>

// Create an instance of the LF_Alpha_HCSR04 class
LF_Alpha_HCSR04 ultrasonicSensor;

void setup(){
    Serial.begin(115200);
    Serial.println("LF_Alpha_M1 HCSR04 Ultrasonic Sensor Example");

    // Initialize the HCSR04 Ultrasonic Sensor
    ultrasonicSensor.begin();
    Serial.println("HCSR04 Ultrasonic Sensor initialized");
}

void loop(){
    // Read the distance in centimeters
    long distanceInCM = ultrasonicSensor.readCM();

    // Read the distance in inches
    long distanceInInches = ultrasonicSensor.readINCH();

    // Print the distance to the Serial Monitor
    Serial.print("Distance in CM: ");
    Serial.print(distanceInCM);
    Serial.println(" cm");
    Serial.print("Distance in Inches: ");
    Serial.print(distanceInInches);
    Serial.println(" inches");
    Serial.println("--------------------");

    // Wait for 2 seconds before the next reading
    delay(2000);
}