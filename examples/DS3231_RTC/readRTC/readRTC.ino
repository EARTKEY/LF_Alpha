/*
    LF_Alpha DS3231 RTC Example
    This example shows how to use the DS3231 RTC on the LF_Alpha.
    available methods:
        -begin(); Initializes the DS3231 RTC.
        -setDateTime(__DATE__, __TIME__); Sets the date and time of the DS3231 RTC.
        -setDateTime(year, month, day, hour, minute, second); Sets the date and time of the DS3231 RTC. (24-hour format)
        -readTime(); Reads the current time from the DS3231 RTC.
        -readDate(); Reads the current date from the DS3231 RTC.
        -getYear(); Reads the current year from the DS3231 RTC.
        -getDayNumber(); Reads the current day number from the DS3231 RTC.
        -getDayName(); Reads the current day name from the DS3231 RTC.
        -getMonthName(); Reads the current month name from the DS3231 RTC.
        -getMonthNumber(); Reads the current month number from the DS3231 RTC.
        -isLeapYear(); Checks if the current year is a leap year and returns true if it is, false if it is not.
        -end(); Ends the communication with the DS3231 RTC.
*/

#include <LF_Alpha.h>

// create an instance of DS3231_RTC
LF_Alpha_RTC RTC;

void setup()
{
  Serial.begin(115200);
  Serial.println("LF_Alpha DS3231 RTC Example");

  // Initialize the DS3231 RTC
  bool status = RTC.begin();
  if (status == true)
  {
    Serial.println("DS3231 RTC Initialized");
  }
  else
  {
    Serial.println("DS3231 RTC Initialization Failed :( ");
    while (1);      // halt the program
  }

}

void loop()
{
  // Read the current time from the DS3231 RTC
  String time = RTC.readTime();
  // Read the current date from the DS3231 RTC
  String date = RTC.readDate();

  // Print the current time and date
  Serial.println("Current Time: " + time);
  Serial.println("Current Date: " + date);
  Serial.println("Current Year: " + String(RTC.getYear()));
  Serial.println("Current Day Number: " + String(RTC.getDayNumber()));
  Serial.println("Current Day Name: " + RTC.getDayName());
  Serial.println("Current Month Name: " + RTC.getMonthName());
  Serial.println("Current Month Number: " + String(RTC.getMonthNumber()));
  Serial.println("---------------------------------------------------------");

  delay(1000);
}