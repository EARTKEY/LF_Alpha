/*
    LF_Alpha_MAX30102 Basic Read SPO2 Example. The normal SPO2 level is between 95-100% and heart rate is between 60-100 bpm.
    This example demonstrates how to initialize and use the LF_Alpha_MAX30102 sensor to read SPO2 and heart rate values.
    It reads the SPO2 and heart rate values and prints them to the Serial Monitor.

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

// Global variables to store SPO2 and heart rate values
int32_t SPO2 = 0;
int8_t SPO2Valid = 0;
int32_t heartRate = 0;
int8_t heartRateValid = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("LF_Alpha_MAX30102 Basic Read SPO2 Example");
  Serial.println("-----------------------------------------\n");

  // Initialize the MAX30102 sensor
  if (!oximeterSensor.begin()) {
    Serial.println("MAX30102 not found.");
    while (1);
  }

  oximeterSensor.configure(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4,
      /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100,
      /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);

  Serial.println("MAX30102 initialized successfully. :)\n");

  Serial.println("Put your fingers on the MAX30102 Sensor and wait for 5 Seconds");

  delay(5000);
}

void loop()
{
  oximeterSensor.readSPO2(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);

  Serial.print(F("heartRate :"));
  Serial.println(heartRate, DEC);

  Serial.print(F("heartRateValid :"));
  Serial.println(heartRateValid, DEC);

  Serial.print(F("SPO2 :"));
  Serial.println(SPO2, DEC);

  Serial.print(F("SPO2Valid :"));
  Serial.println(SPO2Valid, DEC);

  Serial.println("-------------------------------\n");
  delay(1000);
}