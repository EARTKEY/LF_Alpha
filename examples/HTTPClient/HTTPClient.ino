/*
    LF_Alpha_HTTPClient Example
    This example demonstrates how to use the LF_Alpha_HTTPClient class to perform HTTP GET and POST requests.
    It connects to a specified server URL and sends requests, printing the responses to the Serial Monitor.

    Note:
    Make sure to replace "your-ssid" and "your-password" with your actual WiFi credentials.
    Also, replace "http://example.com/api" with the actual server URL you want to interact with.

    available methods:
        - begin(url): Initializes the HTTP client with the specified server URL.
        - get(payload): Sends an HTTP GET request to the server and returns the HTTP status code.
        - post(payload): Sends an HTTP POST request with the given payload to the server and returns the HTTP status code.
        - put(payload): Sends an HTTP PUT request with the given payload to the server and returns the HTTP status code.
        - del(payload): Sends an HTTP DELETE request with the given payload to the server and returns the HTTP status code.
        - end(): Ends the HTTP connection.
*/

#include <LF_Alpha.h>

// Create an instance of the HTTP Client
LF_Alpha_HTTPClient httpClient;

// Create an instance of the WiFi class
LF_Alpha_WiFi wifi;

// configurations
#define endPointURL             "http://example.com/api"    // Replace with your server URL
#define WiFi_SSID               "your-ssid"                 // Replace with your WiFi SSID
#define WiFi_PASSWORD           "your-password"             // Replace with your WiFi Password
/* Note: Also change the json payload as per your requirement */

void setup(){
    Serial.begin(115200);
    Serial.println("LF_Alpha_HTTPClient Example");

    // Connect to WiFi
    wifi.connect(WiFi_SSID, WiFi_PASSWORD);
    Serial.println("Connecting to WiFi...");
    while(!wifi.readStatus()){
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nConnected to WiFi");

    // Initialize the HTTP client with the server URL
    httpClient.begin(endPointURL);
    Serial.println("HTTP Client initialized");

    // Perform a GET request
    int getResponseCode = httpClient.get();
    if(getResponseCode > 0){
        Serial.print("GET request successful, HTTP Status Code: ");
        Serial.println(getResponseCode);
        Serial.print("Response: ");
        Serial.println(httpClient.getResponse());
    } 
    else {
        Serial.println("GET request failed");
    }

    // Perform a POST request with a sample JSON payload
    String jsonPayload = "{\"key\":\"value\"}";
    int responseCode = httpClient.post(jsonPayload);
    if(responseCode > 0){
        Serial.print("POST request successful, HTTP Status Code: ");
        Serial.println(responseCode);
        Serial.print("Response: ");
        Serial.println(httpClient.getResponse());
    } else {
        Serial.println("POST request failed");
    }

    // End the HTTP connection
    httpClient.end();
}

void loop()
{
    // Nothing to do here
}