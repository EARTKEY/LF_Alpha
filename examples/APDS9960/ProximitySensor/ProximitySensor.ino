/*
    LF_Alpha APDS9960 Proximity Sensor Example
    This example shows how to use the proximity sensor on the LF_Alpha.
    available methods:
        -begin(); Initializes the proximity sensor.
        -enableProximity(); Enables the proximity sensor.
        -setProximityInterruptThreshold(low, high); Sets the proximity interrupt threshold.
        -enableProximityInterrupt(); Enables the proximity interrupt.
        -disableProximityInterrupt(); Disables the proximity interrupt.
        -readProximity(); Reads the proximity data from the sensor.
        -clearInterrupt(); Clears the interrupt flag.
        -end(); De-initializes the proximity sensor.
*/

#include <LF_Alpha.h>

//create an instance of APDS9960
LF_Alpha_APDS9960 proximitySensor;

void setup()
{
    Serial.begin(115200);
    pinMode(APDSINT, INPUT_PULLUP);
    Serial.println("LF_Alpha APDS9960 Proximity Sensor Example");

    // Initialize the proximity sensor
    bool status = proximitySensor.begin();
    if(status)
    {
        Serial.println("Proximity Sensor Initialized");
    }
    else
    {
        Serial.println("Proximity Sensor Initialization Failed");
        while(1);
    }

    // Enable the proximity sensor
    proximitySensor.enableProximity();

    //set the interrupt threshold to fire when proximity reading goes above 175
    proximitySensor.setProximityInterruptThreshold(0, 175);

    //enable the proximity interrupt
    proximitySensor.enableProximityInterrupt();
}

void loop()
{
    if(digitalRead(APDSINT) == LOW)
    {
        Serial.print("Proximity Sensor Data: ");
        Serial.println(proximitySensor.readProximity());

        //clear the interrupt flag
        proximitySensor.clearInterrupt();
    }
}