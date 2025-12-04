/*
    LF_Alpha_MAX30102 Basic Read Example
    This example demonstrates how to initialize the LF_Alpha_MAX30102 sensor and plot the heartbeat data.
    It reads the red LED values and prints them to the Serial Monitor for plotting.
    available methods:
        - begin(): Initializes the MAX30102 sensor.
        - sensorConfiguration(): Configures the sensor with specified parameters.
        - readBPM(): Reads the Blood Pulse Rate (BPM) value.
        - readSPO2(): Reads the Oxygen Saturation (SPO2) value.
        - readTempC(): Reads the temperature in Celsius.
        - readTempF(): Reads the temperature in Fahrenheit.
        - end(): Deactivates the sensor.
*/

#include <LF_Alpha.h>

// Create an instance of the MAX30102 sensor
LF_Alpha_MAX30102 oximeterSensor;

void setup()
{
    Serial.begin(115200);
    Serial.println("MAX30102 Heart Beat Plotter Example");
    Serial.println("-----------------------------------\n");

    // Initialize the MAX30102 sensor
    if (!oximeterSensor.begin())
    {
        Serial.println("MAX30102 not found. :(");
        while (1);
    }

    // Configure the sensor with default parameters
    oximeterSensor.configure(/*ledBrightness=*/60, /*sampleAverage=*/SAMPLEAVG_8,
                             /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_400,
                             /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);
    Serial.println("MAX30102 initialized successfully.");
}

void loop()
{
    Serial.println(oximeterSensor.readBPM());
}
