/*
    LF_Alpha_MAX0102 Temprature Read Example. 
    This example demonstrates how to initialize and use the LF_Alpha_MAX0102 sensor to read temperature values.
    It reads the temperature in Celsius and Fahrenheit and prints them to the Serial Monitor.

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
LF_Alpha_MAX30102 tempSensor;

void setup() {
    Serial.begin(115200);
    Serial.println("LF_Alpha_MAX0102 Basic Read Temperature Example");
    Serial.println("-----------------------------------------\n");

    // Initialize the MAX30102 sensor
    if (!tempSensor.begin()) {
        Serial.println("MAX30102 not found.");
        while (1);
    }

    tempSensor.configure(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4,
                         /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100,
                         /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);

    Serial.println("MAX30102 initialized successfully. :)\n");
}

void loop()
{
    float temperatureC = tempSensor.readTempC();
    float temperatureF = tempSensor.readTempF();

    Serial.print(F("Temperature in Celsius: "));
    Serial.print(temperatureC);
    Serial.println(" °C");

    Serial.print(F("Temperature in Fahrenheit: "));
    Serial.print(temperatureF);
    Serial.println(" °F");

    Serial.println("-------------------------------\n");
    delay(2000);
}