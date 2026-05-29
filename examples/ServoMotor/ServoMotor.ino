/*
    LF_Alpha Servo Motor Example
    This example shows how to use the servo motor on the LF_Alpha.
    available methods:
        -begin(); Initializes the servo motor.
        -write(ANGLE); Sets the servo motor angle to the specified value.
        -read(); Reads the current angle of the servo motor.
        -sweep(); Sweeps the servo motor from 0 to 180 degrees.
        -end(); De-initializes the servo motor.
*/

#include <LF_Alpha.h>

// Create an instance of ServoMotor
LF_Alpha_Servo servoMotor;

void setup()
{
    Serial.begin(115200);
    Serial.println("LF_Alpha Servo Motor Example");

    // Initialize the servo motor
    servoMotor.begin();
    Serial.println("Servo Motor Initialized");
}

void loop()
{
    // Set servo motor angle to 90 degrees
    servoMotor.write(90);
    Serial.println("Servo Motor Angle set to 90 degrees");
    delay(1000);

    // Set servo motor angle to 0 degrees
    servoMotor.write(0);
    Serial.println("Servo Motor Angle set to 0 degrees");
    delay(1000);

    // Sweep servo motor from 0 to 180 degrees
    servoMotor.sweep();
    Serial.println("Servo Motor Sweeped from 0 to 180 degrees");
    delay(2000);
}