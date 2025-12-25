/*
    LF_Alpha Relays Example
    This example demonstrates how to initialize and use the LF_Alpha Relays.
    The Relays can control up to 2 relays, and this example shows how to turn them on and off.
    available methods:
        - begin(RLY1): Initializes the relay module with a single pin (RLY1 or RLY2).
        - begin(RLY1, RLY2): Initializes the relay module with two pins (RLY1 and RLY2).
        - writeRLY1(LOGIC): controls the HIGH/LOW logic for respective relay 
        - writeRLY2(LOGIC): controls the HIGH/LOW logic for respective relay
        - end(): de-initilize the relay module

    Both Relay Example:
*/

// Include the LF_Alpha library
#include <LF_Alpha.h>

// Create an instance of the LF_Alpha_Relays
LF_Alpha_Relays relays;

void setup(){
    Serial.begin(115200);
    Serial.println("LF_Alpha Relays Example");

    // Initialize the relays
    relays.begin(RLY1, RLY2);
    Serial.println("Relays initialized");
}

void loop(){
    // Turn on relay 1
    relays.writeRLY1(HIGH);
    Serial.println("Relay 1 is ON.");
    delay(2000);

    // Turn off relay 1
    relays.writeRLY1(LOW);
    Serial.println("Relay 1 is OFF.");
    delay(500);

    // Turn on relay 
    relays.writeRLY2(HIGH);
    Serial.println("Relay 2 is ON.");
    delay(2000);

    // Turn off relay 2
    relays.writeRLY2(LOW);
    Serial.println("Relay 2 is OFF.");
    delay(500);

    // wait for 10 sec before next cycle
    delay(10000);
}






