/*
    LF_Alpha_WiFI Access Point Example.
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

// configurations
#define AP_SSID             "MyAccessPoint"
#define AP_PASSWORD         "MyPassword"

void setup()
{
  Serial.begin(115200);
  Serial.println("LF_Alpha_WiFi Access Point Example");

  // Initialize the WiFi module
  wifi.begin();
  wifi.configure(WIFI_AP_STA); // Set WiFi mode to Access Point + Station
  Serial.println("WiFi module initialized");

  // Start an access point
  if (wifi.startAccessPoint(AP_SSID, AP_PASSWORD))
  {
    Serial.println("Access Point started successfully");
  }
  else
  {
    Serial.println("Failed to start Access Point");
  }
}

void loop()
{
  // Nothing to do here
}