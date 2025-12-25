/*
    LF_Alpha AHT20 Temperature and Humidity Sensor Example
    This example shows how to use the AHT20 temperature and humidity sensor on the LF_Alpha.
    available methods:
        -begin(); Initializes the AHT20 sensor.
        -readTempC(); Reads the temperature value in Celsius from the sensor.
        -readTempF(); Reads the temperature value in Fahrenheit from the sensor.
        -readHumi(); Reads the humidity value in percentage from the sensor.
        -end(); Ends the communication with the AHT20 sensor.
*/

#include <LF_Alpha.h>

// Create an instance of the LF_Alpha_AHT20 class
LF_Alpha_AHT20 ahtSensor;

void setup()
{
    Serial.begin(115200);
    Serial.println("LF_Alpha AHT20 Temperature and Humidity Sensor Example");
    
    // Initialize the AHT20 sensor
    bool status = ahtSensor.begin();
    if(status == true)
    {
        Serial.println("AHT20 Sensor Initialized");
    }
    else
    {
        Serial.println("AHT20 Sensor Initialization Failed");
        while(1);
    }
}

void loop()
{
    // Read temperature and humidity values from the sensor
    float tempC = ahtSensor.readTempC();
    float tempF = ahtSensor.readTempF();
    float humi = ahtSensor.readHumi();

    // Print the values to the serial monitor
    Serial.print("Temperature in Celsius : ");
    Serial.print(tempC);
    Serial.println(" °C");
    Serial.print("Temperature in Fahrenheit: ");
    Serial.print(tempF);
    Serial.println(" °F");
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.println(" %");
    Serial.println("--------------------------------------");

    // Wait for 5 second before reading again
    delay(5000);
}