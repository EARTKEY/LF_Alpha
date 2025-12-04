/*
    LF_Alpha I2C Scanner Example
    This example demonstrates how to initialize and use the LF_Alpha_M1 I2C Scanner.
    It scans the I2C bus for devices and returns their addresses and number of devices found.

    available methods:
        - scan(): Scans the I2C bus and populates the device count and addresses.
        - getAllAddresses(): Returns a string of all found I2C addresses.
        - getDeviceCount(): Returns the number of devices found on the I2C bus.
        - end(): Cleans up resources (not strictly necessary for this class).
*/

#include <LF_Alpha.h>

// Create an instance of the I2C Scanner
LF_Alpha_I2CScanner scanner;

void setup() {
    Serial.begin(115200);
    Serial.println("LF_Alpha_M1 I2C Scanner Example");

    // Start the I2C Scanner
    scanner.scan();
    
    //get the number of devices found
    byte deviceCount = scanner.getDeviceCount();

    //get the addresses of the devices found
    String addresses = scanner.getAllAddresses();

    // Print the results
    Serial.println("I2C Scanner Results:");
    Serial.print("Number of devices found: ");
    Serial.println(deviceCount);       
    Serial.print("Addresses: ");
    Serial.println(addresses);
}

void loop()
{
    // Nothing to do here
}