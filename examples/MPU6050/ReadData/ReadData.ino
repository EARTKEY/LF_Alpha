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

#include <LF_Alpha.h>

// Create an instance of MPU6050
LF_Alpha_MPU6050 mpuSensor;

void setup()
{
    Serial.begin(115200);
    Serial.println("LF_Alpha_M1 MPU6050 Read Data Example");

    // Initialize the MPU6050
    bool status = mpuSensor.begin();
    if(status == true)
    {
        Serial.println("MPU6050 Initialized");
    }
    else
    {
        Serial.println("MPU6050 Initialization Failed");
    }
}

void loop()
{
    // Read the acceleration from the MPU6050
    float accX = mpuSensor.readAccX();
    float accY = mpuSensor.readAccY();
    float accZ = mpuSensor.readAccZ();
    Serial.print("Acceleration: ");
    Serial.print(accX);
    Serial.print(", ");
    Serial.print(accY);
    Serial.print(", ");
    Serial.print(accZ);
    Serial.println(" m/s^2");

    // Read the gyroscope from the MPU6050
    float gyroX = mpuSensor.readGyroX();
    float gyroY = mpuSensor.readGyroY();
    float gyroZ = mpuSensor.readGyroZ();
    Serial.print("Gyroscope: ");
    Serial.print(gyroX);
    Serial.print(", ");
    Serial.print(gyroY);
    Serial.print(", ");
    Serial.print(gyroZ);
    Serial.println(" rad/s");
   
    // Read the motion status from the MPU6050
    byte motionStatus = mpuSensor.getMotionState();
    if(motionStatus == 1)
    {
        Serial.println("Motion Status: Moving");
    }
    else
    {
        Serial.println("Motion Status: Stationary/Idle");
    }

    Serial.println("---------------------------------------------");
    delay(3000);
}