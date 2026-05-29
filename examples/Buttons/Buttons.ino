/*
    LF_Alpha Buttons Example
    This example demonstrates how to initialize and use the LF_Alpha Buttons.
    available methods:
        -begin(upButton); Initializes the up button.
        -begin(upButton, downButton); Initializes the up and down buttons.
        -begin(upButton, downButton, backButton, okayButton); Initializes all the buttons.
        -read(_whichButton_); Reads the state of the specified button.
*/

#include <LF_Alpha.h>

// Create an instance of the LF_Alpha_Buttons class
LF_Alpha_Buttons buttons;

void setup(){
    Serial.begin(115200);
    Serial.println("LF_Alpha Buttons Example");

    // Initialize all the buttons
    buttons.begin(upButton, downButton, backButton, okayButton);
    Serial.println("Buttons initialized");
}

void loop(){
    if (buttons.read(upButton) == LOW) {
        Serial.println("Up Button pressed");
    }
    if (buttons.read(downButton) == LOW) {
        Serial.println("Down Button pressed");
    }
    if (buttons.read(backButton) == LOW) {
        Serial.println("Back Button pressed");
    }
    if (buttons.read(okayButton) == LOW) {
        Serial.println("Okay Button pressed");
    }

    delay(200);
}