//Environment Monitoring System using AHT20 sensor and OLED display

/*This project demonstrates how to interface an AHT20 temperature and humidity sensor
 * with an Arduino-compatible board and display the readings on a 128x64 SSD1306 OLED display.
 * The system initializes the sensor and display, shows a startup screen, handles sensor errors,
 * and continuously updates the temperature and humidity data on the OLED.

Libraries Used
 *  LF_Alpha.h: A custom library  providing simplified access to sensor data.
 *  U8g2lib.h: A universal graphics library for OLEDs and other displays, supporting various display controllers and interfaces.
 *  This Libraries has 3 Font Sizes Small(6x12),Medium(7x14B),Large(10x20).
*/

#include <LF_Alpha.h>// Includes the custom library for the AHT20 temperature and humidity sensor.
#include <U8g2lib.h>// Includes the U8g2 graphics library for controlling the OLED display.

#define SERIAL_BAUD 115200// Defines the baud rate for serial communication, used for debugging and outputting data to the Serial Monitor.

#define Startupscreen_Delay 5000// Defines a delay for the startup screen duration in milliseconds.

#define Sampling_Delay 500// Defines a delay for the sampling interval between sensor readings in milliseconds.

// Object creation of AHT_20 sensor data
LF_Alpha_AHT20 ahtsensor;// Creates an instance of the LF_Alpha_AHT20 class to interact with the AHT20 sensor.


// Initializes a 128x64 SSD1306 OLED display using hardware I2C communication
U8G2_SSD1306_128X64_NONAME_F_HW_I2C OLED(U8G2_R0);

// Function Declarations of OLED Display Functions
void showstartupscreen(void);
void showerror(void);
void showEnvironmentdata(float tempC, uint8_t humi);

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("Environment Monitoring System");// Prints the text in Serial monitor.
  bool status = ahtsensor.begin();// Initializes the AHT20 sensor
  Serial.println("AHT_20 Sensor Initialisation Completed Successfully");
  OLED.begin();                  // Initializes the OLED display
  Serial.println("OLED Initialisation Completed Successfully. ");
  showstartupscreen();
  if (!status) { // Checks if the AHT20 sensor initialization failed.
     showerror(); // If initialization failed, calls the function to display an error message.
  }
}

void loop() {
  float tempC = ahtsensor.readTempC();// Reads temperature value in Celsius from the AHT20 sensor
  uint8_t humi  = ahtsensor.readHumi();//Reads Humidity Value in Percentage from AHT_20 Sensor
  Serial.print("Temperature: ");//Prints the value of temperature upto 2 precision in Serial Monitor
  Serial.print(tempC,2);
  Serial.print(" C | Humidity: ");
  Serial.print(humi);//Prints the value of temperature upto 2 precision in Serial Monitor
  Serial.println(" %");
  showEnvironmentdata(tempC, humi);
  delay(Sampling_Delay);
}
// Startup Screen
void showstartupscreen(void) { // Function to display the initial startup message on the OLED.
  OLED.clearBuffer();
  OLED.setFont(u8g2_font_10x20_tf);
  OLED.setCursor(8, 15);  OLED.print(F("ENVIRONMENT"));
  OLED.setCursor(12, 38); OLED.print(F("MONITORING"));
  OLED.setCursor(31, 62); OLED.print(F("SYSTEM"));
  OLED.sendBuffer(); 
  delay(Startupscreen_Delay);
}
// Displays an error message if the sensor fails to initialize
void showerror(void) {// Function to display an error message on the OLED.
  OLED.clearBuffer();
  OLED.setFont(u8g2_font_7x14B_tf);// Sets the font size(medium) for the text to be displayed on the OLED.
  OLED.drawStr(25, 35, "Sensor Error!");//drawStr(x,y,text to be displayed)
  OLED.sendBuffer();
  while (true);
}

//Main Display
// Displays temperature and humidity data on the OLED screen
void showEnvironmentdata(float tempC, uint8_t humi) {
  OLED.clearBuffer();
  
  // Header Display on OLED
  OLED.setFont(u8g2_font_6x12_tf);// Sets the font size(small) for the text to be displayed on the OLED.
  OLED.drawStr(12, 12, "AHT20 SENSOR DATA");// Draws the header text "AHT20 SENSOR DATA" on the OLED.Here X axis=12,Y axis=12

  // Temperature Values in Degree Celsius
  OLED.setFont(u8g2_font_10x20_tf);// Sets the font size(Large) for the text to be displayed on the OLED.
  OLED.drawStr(5, 42, "T  : ");//drawStr(x,y,text)
  OLED.setCursor(55,42);//setCursor(x,y)
  OLED.print(tempC,2);
  OLED.drawGlyph(105, 42, 176); //drawGlyph(X,Y,ASCII Value for displaying degree character)
  OLED.drawStr(114, 42, "C");

  //Humidity Values in Percentage
  OLED.drawStr(5, 62, "H  : ");//drawStr(x,y,text)
  OLED.setCursor(55, 62);//setCursor(x,y)
  OLED.print(humi);   
  OLED.print("%");
  OLED.sendBuffer();
}