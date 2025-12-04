/*
    LF_Alpha OLED Display Example
    This example shows how to use the OLED display on the LF_Alpha_M1.
    available methods:
        -begin(); Initializes the OLED display.
        -clearDisplay(); Clears the OLED display.
        -setCursor(x, y); Sets the cursor position on the OLED display.
        -print(str); Prints the specified string on the OLED display.
        -setTextSize(size); Sets the text size on the OLED display.
        -setTextColor(color); Sets the text color on the OLED display.
        -drawLine(x1, y1, x2, y2); Draws a line on the OLED display.
        -drawRect(x, y, w, h); Draws a rectangle on the OLED display.
        -fillRect(x, y, w, h); Fills a rectangle on the OLED display.
        -drawCircle(x, y, r); Draws a circle on the OLED display.
        -fillCircle(x, y, r); Fills a circle on the OLED display.
        -drawTriangle(x1, y1, x2, y2, x3, y3); Draws a triangle on the OLED display.
        -fillTriangle(x1, y1, x2, y2, x3, y3); Fills a triangle on the OLED display.
        -display(); Displays the content on the OLED display.
        -end(); Ends the communication with the OLED display.

        etc.
*/

#include <LF_Alpha.h>

// Create an instance of OLEDDisplay
LF_Alpha_OLED OLED;

void setup()
{
  Serial.begin(115200);
  Serial.println("LF_Alpha_M1 OLED Display Example");

  // Initialize the OLED display
  OLED.begin();
  Serial.println("OLED Display Initialized");

  OLED.clearDisplay();
  OLED.setCursor(40, 20);
  OLED.setTextSize(1);                
  OLED.setTextColor(SSD1306_WHITE);
  OLED.println("Hi there,");

  OLED.setCursor(2, 30);
  OLED.println("Welcome to LF ALpha");
  
  OLED.display();
}

void loop()
{
  // do nothing

}