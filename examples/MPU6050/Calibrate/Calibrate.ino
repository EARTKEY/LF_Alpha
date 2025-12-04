/*
    LF_Alpha MPU6050 Read Data Example
    This example shows how to read data from the MPU6050 on the LF_Alpha_M1.
    available methods:
        -begin(); Initializes the MPU6050.
        -calibrateSensor(); Calibrates the MPU6050 motion/idle threshold values.
        -deepCalibrateSensor(); Calibrates the complete MPU6050 Registers.
        -readAccX(); Reads the X-axis acceleration from the MPU6050.
        -readAccY(); Reads the Y-axis acceleration from the MPU6050.
        -readAccZ(); Reads the Z-axis acceleration from the MPU6050.
        -readGyroX(); Reads the X-axis gyroscope from the MPU6050.
        -readGyroY(); Reads the Y-axis gyroscope from the MPU6050.
        -readGyroZ(); Reads the Z-axis gyroscope from the MPU6050.
        -getMotionState(); Reads the motion statu s from the MPU6050.
        -getCalibratedThreshold(); Provides the Calibrated Result (gyroOffsetValue).
        -setIdleThresholdGyro(value); Pass the idleThreshold value
        -end(); De-initializes the MPU6050.
*/

// Note : make sure that the LF_Alpha Kit is not moving while calibrating.

#include <LF_Alpha.h>

// Create an instance of MPU6050
LF_Alpha_MPU6050 mpuSensor;

void setup()
{
    Serial.begin(115200);
    Serial.println("LF_Alpha_M1 MPU6050 Calibrate Example");

    // Initialize the MPU6050
    mpuSensor.begin();
    Serial.println("MPU6050 Initialized");

    // Calibrate the MPU6050
    Serial.println("Calibrating MPU6050...");
    delay(3000);
    mpuSensor.deepCalibrateSensor();
    mpuSensor.calibrateSensor();
    Serial.println("MPU6050 Calibration Completed");
}

void loop()
{
    // nothing to do here..
}