/*
    LF_Alpha_WiFI WiFi Example.
    This example demonstrates how to initialize and use the LF_Alpha_WiFi class.
    available methods:
        - begin(); Initializes the WiFi module.
        - connect(SSID, PASSWORD); Connects to a WiFi network.
        - disconnectWiFi(); Disconnects from the WiFi network.
        - configure(mode); Configures the WiFi mode (WIFI_OFF, WIFI_STA, WIFI_AP, WIFI_AP_STA).
        - getCurrentMode(); Gets the current WiFi mode.
        - readStatus(); Reads the current WiFi connection status.
        - getIPAddress(); Gets the IP address of the device.
        - checkInternet(); Checks if the device is connected to the internet.
        - startAccessPoint(SSID, PASSWORD); Starts an access point with the given SSID
        - stopAccessPoint(); Stops the access point.
        - end(); De-initializes the WiFi module.
*/

#include <LF_Alpha.h>

// Create an instance of LF_Alpha_WiFi
LF_Alpha_WiFi wifi;

#define WiFi_SSID           "Your_SSID"
#define WiFi_PASSWORD       "Your_PASSWORD"

void setup()
{
    Serial.begin(115200);
    Serial.println("LF_Alpha_WiFi Example");

    // Initialize the WiFi module
    wifi.begin();
    wifi.configure(WIFI_STA); // Set WiFi mode to Station
    Serial.println("WiFi module initialized");

    // Connect to a WiFi network
    wifi.connect(WiFi_SSID, WiFi_PASSWORD);
    Serial.println("Connecting to WiFi...");

    // Wait for connection
    while (!wifi.readStatus())
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    // Get and print the IP address
    String ipAddress = wifi.getIPAddress();
    Serial.print("IP Address: ");
    Serial.println(ipAddress);

    // Check internet connectivity
    if (wifi.checkInternet())
    {
        Serial.println("Internet connection is available");
    }
    else
    {
        Serial.println("No internet connection");
    }
}

void loop()
{
    // nothing to do here
}