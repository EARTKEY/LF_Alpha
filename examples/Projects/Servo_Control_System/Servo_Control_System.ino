/*
 * Servo Control System
 * This program reads analog values from a potentiometer, maps them to a servo motor's angular position,
 * and displays the potentiometer value and servo angle on a 128x64 OLED display.
 * It is designed for use with the LF Alpha Board.
 * This code is provided for educational purposes and demonstrates basic interfacing
 * with a potentiometer, servo motor, and OLED display on the LF Alpha Board.

 * LF_Alpha.h: Custom library for LF Alpha Board peripherals (potentiometer, servo, OLED).
 */
#include <LF_Alpha.h> // Include the custom library for LF Alpha Board peripherals.

// Define constants for serial communication, startup delay, and sampling delay.
#define SERIAL_BAUD 115200 // Baud rate for serial communication, used for debugging and monitoring.
#define Startupscreen_Delay 4000  
#define Sampling_Delay 200        

// Declare hardware objects using the LF_Alpha library classes.
LF_Alpha_resPot potentiometer;  
LF_Alpha_Servo servoMotor;      
LF_Alpha_OLED OLED;
uint16_t potValue;
// Initialization status flags for potentiometer, servo motor, and OLED display
bool potStatus;
bool servoStatus;
bool oledStatus;
// Function Declarations of OLED Display Functions
void showStartupScreen(void);                  
void showPotServoData(uint8_t angle); 
void showError(void);                          

void setup()
{
  Serial.begin(SERIAL_BAUD);          // Initialize serial communication at the defined baud rate.
  Serial.println(F("Servo Control System")); 
  // Initialize the potentiometer and check its status.
  potStatus=potentiometer.begin(); // Initialization of the potentiometer. Returns true on success.
  Serial.println(F("Potentiometer Initialisation Completed"));
  servoStatus=servoMotor.begin();                 // Initialize the servo motor. 
  Serial.println(F("Servo Motor Initialisation Completed")); 
  oledStatus=OLED.begin();                       // Initialize the OLED display. 
  Serial.println(F("OLED Initialisation Completed")); 
  showStartupScreen();                          // Call the function to display the startup screen on the OLED.

  // Check if potentiometer initialization failed. If so, display an error and halt.
  if(!potStatus || !servoStatus || !oledStatus)
  {
    showError(); // Call the error display function.
  }
}

void loop()
{
  potValue = potentiometer.readAnalog(); // Read the raw analog value from the potentiometer (0-4095 for 12-bit ADC).

  // Map the raw potentiometer value (0-4095) to a servo angle (0-180 degrees).
  uint8_t servoAngle = map(potValue, 0, 4095, 0, 180); // The `map` function scales a number from one range to another.

  servoMotor.write(servoAngle);  // Command the servo motor to move to the calculated angle.
  Serial.print(F("Potentiometer Value: ")); 
  Serial.print(potValue); 
  Serial.print(F(" | Servo Angle: "));     
  Serial.print(servoAngle);  
  Serial.println(F("°"));          

  showPotServoData(servoAngle); // Update the OLED display with current potentiometer and servo data.
  delay(Sampling_Delay);        // Pause for a short duration to control the sampling rate.
}

// Startup Screen
void showStartupScreen(void)
{
  OLED.clearDisplay();                  
  OLED.setTextSize(2);                  // Set the text size for the startup message.
  OLED.setTextColor(SSD1306_WHITE);     // Set the text color to white (assuming monochrome OLED).
  OLED.setCursor(35,0);                 // setCursor(x,y) 
  OLED.println("SERVO");                
  OLED.setCursor(25,20);                // Set the cursor position for the second line.
  OLED.println("CONTROL");              
  OLED.setCursor(30,43);                
  OLED.println("SYSTEM");               
  OLED.display();                       // Push the buffer content to the physical OLED display.
  delay(Startupscreen_Delay);           // Pause for the defined startup screen duration.
}

// Displays an error message if the sensor fails to initialize
void showError(void)
{
  OLED.clearDisplay();
  OLED.setCursor(10,20); 
  if(!potStatus)
    OLED.println("POT ERROR!");//Displays error when pot initialisation fails.
  else if(!servoStatus)
    OLED.println("SERVO ERROR!");//Displays error when Servo initialisation fails.
  else if(!oledStatus)
    OLED.println("OLED ERROR!");//Displays error when OLED initialisation fails.
  else
    OLED.println("UNKNOWN ERROR!");
  
  OLED.display();
  
  while(true);
}

//Main Display
// Displays POT Value and Servo Angle data on the OLED screen
void showPotServoData(uint8_t angle)
{
  OLED.clearDisplay();                  // Clear any existing content on the OLED display.
  OLED.setTextSize(1);                  // Set text size for the header.
  OLED.setCursor(5,5);                  // setCursor(x,y)
  OLED.println("SERVO CONTROL SYSTEM"); // Print the system title. 
  OLED.setCursor(0,25);                 
  OLED.print("POT VALUE   : ");         
  OLED.setCursor(0,50);                 
  OLED.print("SERVO ANGLE : ");         

  OLED.setTextSize(2);                  // Set larger text size for the actual values.
  OLED.setCursor(80,20);                //setCursor(x,y)
  OLED.print(potValue); 
  OLED.setCursor(85,45);                //setCursor(x,y)
  OLED.print(angle);

  OLED.setTextSize(1);                  // Revert to smaller text size for the degree symbol. 
  OLED.print((char)247);                // Print the degree symbol (ASCII 247). 
  OLED.display();                       // Push the buffer content to the physical OLED display.
}