/*
    LF_Alpha Buzzer Example
    This example demonstrates how to initialize and use the LF_Alpha Buzzer.
    available methods:
        -begin(); Initializes the buzzer.
        -write(LOGIC); HIGH or LOW
        -beep(DURATION); Beeps the buzzer for a specified duration in milliseconds.
        -end(); De-initializes the buzzer.
*/

#include <LF_Alpha.h>

// Create an instance of Buzzer
LF_Alpha_Buzzer Buzzer;

void setup(){
    Serial.begin(115200);
    Serial.println("LF_Alpha Buzzer Example");

    // Initialize the buzzer
    Buzzer.begin();
    
}

void loop(){

    // Turn on Buzzer for 1 second
    Serial.println("Buzzer ON");
    Buzzer.write(HIGH);
    delay(1000);

    // Turn off Buzzer for 1 second
    Serial.println("Buzzer OFF");
    Buzzer.write(LOW);
    delay(1000);

    // Beep Buzzer for 500 milliseconds
    Serial.println("Buzzer BEEP for 500ms");
    Buzzer.beep(500);
    delay(1000);
}