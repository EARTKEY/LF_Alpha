/*
    LF_Alpha UserLEDs Example
    This example demonstrates how to initialize and use the LF_Alpha UserLEDs.
    available methods:
        -begin(); Initializes the UserLEDs.
        -blink(); Blinks the UserLEDs.
        -writeLED1(); Writes to the first UserLED.
        -writeLED2(); Writes to the second UserLED.
        -write(); Writes to the UserLEDs.
        -end(); De-initializes the UserLEDs.
*/

#include <LF_Alpha.h>

// Create an instance of UserLEDs
LF_Alpha_userLEDs userLEDs;

void setup() {
  Serial.begin(115200);
  Serial.println("LF_Alpha UserLEDs Example");

  // Initialize the userLEDs
  userLEDs.begin(userLED1, userLED2);
  Serial.println("UserLEDs initialized");

  // Initially Turn off both LEDs
  userLEDs.writeLED1(HIGH);               // Active LOW
  userLEDs.writeLED2(LOW);                // Active HIGH
}

void loop() {
  // Turn on both LEDs
  userLEDs.writeLED1(LOW);
  userLEDs.writeLED2(HIGH);
  delay(1000);

  // Turn off both LEDs
  userLEDs.writeLED1(HIGH);
  userLEDs.writeLED2(LOW);
  delay(1000);

  // Blink 5 times 
  for (uint8_t i = 0; i < 5; i++) {
    userLEDs.blink(userLED1, 500);
    userLEDs.blink(userLED2, 500);
  }
  delay(1000);
}