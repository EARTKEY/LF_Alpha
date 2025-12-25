/*
    LF_Alpha APDS9960 Gesture Sensor Example
    This example shows how to use the gesture sensor on the LF_Alpha.
    available methods:
        -begin(); Initializes the gesture sensor.
        -enableGesture(); Enables the gesture sensor.
        -gestureValid(); Checks if gesture data is valid.
        -setGestureDimensions(dim); Sets the gesture dimensions.
        -setGestureGain(gain); Sets the gesture gain.
        -setGestureProximityThreshold(thresh); Sets the gesture proximity threshold.
        -setGestureOffsets(up, down, left, right); Sets the gesture offsets.
        -readGesture(); Reads the gesture data from the sensor.
        -end(); De-initializes the gesture sensor.
*/

#include <LF_Alpha.h>

// Create an instance of LF_Alpha_APDS9960
LF_Alpha_APDS9960 gestureSensor;

void setup()
{
    Serial.begin(115200);
    Serial.println("LF_Alpha Gesture Sensor Example");

    // Initialize the gesture sensor
    bool status = gestureSensor.begin();
    if (status)
    {
        Serial.println("Gesture Sensor Initialized");
    }
    else
    {
        Serial.println("Failed to Initialize Gesture Sensor");
        while (1);
    }
    
    // Enable the gesture sensor and proximity sensor
    gestureSensor.enableProximity();
    gestureSensor.enableGesture();
}

void loop()
{
    uint8_t gestureData = gestureSensor.readGesture();
    if(gestureData == APDS9960_DOWN) 
    {
        Serial.println("Down");
    }
    if(gestureData == APDS9960_UP) 
    {
        Serial.println("Up");
    }
    if(gestureData == APDS9960_LEFT) 
    {
        Serial.println("Left");
    }
    if(gestureData == APDS9960_RIGHT) 
    {
        Serial.println("Right");
    }

    delay(100);
}