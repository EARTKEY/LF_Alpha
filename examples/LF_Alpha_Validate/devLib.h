// Header Files
#include <LF_Alpha.h>

// configurations
#define WiFi_SSID           "yourSSID"
#define WiFi_PASSWORD       "yourPASSWORD"

// Global Variables
uint16_t testResult = 0xFFFF; // 16 bit variable to store the results
unsigned long previousTime = 0;

// Create instances of the peripherals
LF_Alpha_Buzzer buzzer;
LF_Alpha_Buttons buttons;
LF_Alpha_OLED oled;
LF_Alpha_I2CScanner i2cScanner;
LF_Alpha_userLEDs userLEDs;
LF_Alpha_HCSR04 ultrasonicSensor;
LF_Alpha_resPot potentiometer;
LF_Alpha_rgbLEDs rgbLEDs;
LF_Alpha_Relays relays;
LF_Alpha_Servo servoMotor;
LF_Alpha_RTC rtc;
LF_Alpha_AHT20 aht20Sensor;
LF_Alpha_MPU6050 mpuSensor;
LF_Alpha_APDS9960 apds;
LF_Alpha_MAX30102 oximeter;
LF_Alpha_WiFi wifi;

// Function Prototypes
void systemInit(void);
void oledTest(void);
void buttonsTest(void);
void showTestResult(void);
void buzzerTest(void);
void userLEDs_Testing(void);
void ultrasonicTesting(void);
void potentiometerTesting(void);
uint8_t pressedButton(void);
void rgbLEDsTesting(void);
void relaysTesting(void);
void servoMotorTesting(void);
void RTCModuleTesting(void);
void AHT20_Testing(void);
void mpuSensorTesting(void);
void APDS9960_Testing(void);
String getColorNameHSV(uint16_t red, uint16_t green, uint16_t blue, uint16_t clear);
void max30102Testing(void);
void WiFi_HTTP_Testing(void);

// Function Definitions
void systemInit(void) {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("__ Welcome to the LF_ALPHA All Test __");
  delay(1000);

  testResult &= ~(1 << 0);
  pinMode(APDSINT, INPUT_PULLUP);

  // Test the all I2C Devices are present or not?
  Serial.println("Starting I2C Scanner");
  i2cScanner.scan();

  if (i2cScanner.getDeviceCount() == 6)
  {
    Serial.println("All I2C Devices Found!\n");
  }
  else
  {
    Serial.println("Mismatch in the I2C devices from the Hardware...!\n");
  }
  delay(200);

  // Initialize all the peripherals
  buzzer.begin();
  buzzer.beep(500);
  userLEDs.begin(userLED1, userLED2);
  buttons.begin(upButton, downButton, backButton, okayButton);
  ultrasonicSensor.begin();
  potentiometer.begin();
  rgbLEDs.begin();
  relays.begin(RLY1, RLY2);
  servoMotor.begin();
  oled.begin();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(30, 2);
  oled.println("Welcome To,");
  oled.setCursor(25, 16);
  oled.println("LF Alpha Board");
  oled.setCursor(10, 28);
  oled.println("Peripherals Testing");
  oled.setCursor(32, 40);
  oled.println("Process..");
  oled.display();

  if (rtc.begin() == false)
  {
    testResult &= ~(1 << 9);
    Serial.println("RTC Initialization Failed!");
  }
  if (aht20Sensor.begin() == false)
  {
    testResult &= ~(1 << 10);
    Serial.println("AHT20 Initialization Failed!");
  }
  if (mpuSensor.begin() == false)
  {
    testResult &= ~(1 << 12);
    Serial.println("MPU6050 Initialization Failed!");
  }
  if (apds.begin() == false)
  {
    testResult &= ~(1 << 13);
    Serial.println("APDS9960 Initialization Failed!");
  }
  if (oximeter.begin() == false)
  {
    testResult &= ~(1 << 14);
    Serial.println("MAX30102 Initialization Failed!");
  }

  Serial.println("Calibrating MPU6050...\n");
  Serial.println("Keep the Device Stable.");
  Serial.println("Do not touch any peripherals of the board");
  Serial.println("It take approximately 4 minutes");
  delay(3000);

  mpuSensor.deepCalibrateSensor();
  mpuSensor.calibrateSensor();
  Serial.println("MPU6050 Calibration Completed");
  delay(1000);
  Serial.println("System Initialization Complete!");
  Serial.println("-------------------------------------\n");

  Serial.println("Starting All Test...\n");
}

void showTestResult(void) {
  // Show the final result in binary and hexdecimal format
  Serial.print("Test Result (Hex): 0x");
  Serial.println(testResult, HEX);

  Serial.print("Test Result (Binary): 0b");
  for (int i = 15; i >= 0; i--)
  {
    Serial.print((testResult & (1 << i)) ? '1' : '0');
  }

  Serial.print("\nTest Result (Decimal): ");
  Serial.println(testResult);

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(40, 2);
  oled.println("_Result_");

  if (testResult == 65534) {
    Serial.println("Device Working Properly");
    oled.setCursor(20, 20);
    oled.println("Device is Good");
    oled.setCursor(10, 35);
    oled.println("All peripherals are");
    oled.setCursor(15, 47);
    oled.println("Working Properly");
  }
  else {
    Serial.println("Device not Working Properly");
    oled.setCursor(5, 20);
    oled.println("Device Not Working");
    oled.setCursor(20, 35);
    oled.print("Result:  ");
    oled.println(testResult, HEX);
  }
  oled.display();

  Serial.println("\n--------------------------------------\n");
}

void oledTest(void) {
  Serial.println("___OLED Display Testing___");
  Serial.println("Press BACK if display is not working ");
  Serial.println("Press OKAY button if OLED Displays as follows..");
  Serial.println("      Welcome To,   ");
  Serial.println("    LF Alpha Board  ");
  Serial.println(" Peripherals Testing");
  Serial.println("      Process..     ");

  while (1)
  {
    if (buttons.read(backButton) == false)
    {
      testResult &= ~(1 << 11); // Clear OLED bit if not working
      Serial.println("OLED Display Test Failed!\t❌");
      delay(500);
      showTestResult();
      while (1) {
        delay(200);
      }
    }
    if (buttons.read(okayButton) == false)
    {
      testResult |= (1 << 11); // Set OLED bit if working
      Serial.println("OLED Display Test Passed\t✅");
      break;
    }
    delay(100);
  }
  Serial.println("--------------------------------------\n");
}

void buttonsTest(void) {
  bool buttonPass = true;
  previousTime = millis();
  if (buttonPass) {
    Serial.println("___Button Testing___");

    Serial.print("Press BACK button -> ");
    while (buttons.read(backButton) == true) {
      if ((millis() - previousTime) > 120000) {
        buttonPass = false;
        break;
      }
    }
  }
  if (buttonPass) {
    Serial.println("  Pass");
    buzzer.beep(200);

    Serial.print("Press DOWN button -> ");
    while (buttons.read(downButton) == true) {
      if ((millis() - previousTime) > 120000) {
        buttonPass = false;
        break;
      }
    }
  }

  if (buttonPass) {
    Serial.println("  Pass");
    buzzer.beep(200);

    Serial.print("Press UP button -> ");
    while (buttons.read(upButton) == true) {
      if ((millis() - previousTime) > 120000) {
        buttonPass = false;
        break;
      }
    }
  }

  if (buttonPass) {
    Serial.println("  Pass");
    buzzer.beep(200);


    Serial.print("Press OKAY button -> ");
    while (buttons.read(okayButton) == true) {
      if ((millis() - previousTime) > 120000) {
        buttonPass = false;
        break;
      }
    }
  }

  if (buttonPass) {
    Serial.println("  Pass");
    buzzer.beep(200);

    Serial.println("All Buttons Test Passed\t✅");
    Serial.println("--------------------------------------\n");
    return;
  }

  if (buttonPass == false) {
    Serial.println("\nButtons Test Failed\t❌");
    testResult &= ~(1 << 3); // Clear Button bit if not working
    showTestResult();
    while (1) {
      delay(200);
    }
  }
}

void buzzerTest(void) {
  delay(500);
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(25, 2);
  oled.println("Testing Buzzer");
  oled.setCursor(3, 20);
  oled.println("Press OKAY: Beeping");
  oled.setCursor(3, 35);
  oled.println("Press BACK: No Beep");
  oled.display();

  Serial.println("___Buzzer Testing__");
  Serial.println("Press OKAY Button if Beeping, Press BACK if no Beep Sound");
  while (1)
  {
    buzzer.beep(300);

    if (buttons.read(backButton) == false)
    {
      testResult &= ~(1 << 0); // Clear Buzzer bit if not working
      Serial.println("Buzzer Test Failed!\t❌");
      buzzer.write(false);
      oled.clearDisplay();
      oled.setTextSize(1);
      oled.setTextColor(SSD1306_WHITE);
      oled.setCursor(25, 2);
      oled.println("Testing Buzzer");
      oled.setCursor(30, 20);
      oled.println("Buzzer Test");
      oled.setCursor(33, 35);
      oled.println("Completed");
      oled.display();
      break;
    }
    if (buttons.read(okayButton) == false)
    {
      testResult |= (1 << 1); // Set Buzzer bit if working
      Serial.println("Buzzer Test Passed\t✅");
      buzzer.write(false);
      oled.clearDisplay();
      oled.setTextSize(1);
      oled.setTextColor(SSD1306_WHITE);
      oled.setCursor(25, 2);
      oled.println("Testing Buzzer");
      oled.setCursor(30, 20);
      oled.println("Buzzer Test");
      oled.setCursor(33, 35);
      oled.println("Completed");
      oled.display();
      break;
    }
    delay(100);
  }
  Serial.println("--------------------------------------\n");
  delay(1000);
}

void userLEDs_Testing(void) {
  Serial.println("___userLEDs (LED3 & LED4) Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing userLEDs");
  oled.setCursor(0, 20);
  oled.println("Press OKAY: Blinking");
  oled.setCursor(0, 35);
  oled.println("Press BACK: No Blink");
  oled.display();
  Serial.println("If WORKING press OKAY Button, else press BACK Button");

  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("userLEDs : Pass\t✅");
      testResult |= (1 << 2); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("userLEDs : Fail\t❌");
      testResult &= ~(1 << 2); // Clear userLEDs bit if not working
      break;
    }

    userLEDs.writeLED1(HIGH);
    userLEDs.writeLED2(HIGH);
    delay(500);
    userLEDs.writeLED1(LOW);
    userLEDs.writeLED2(LOW);
    delay(500);
  }
  userLEDs.end();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(25, 2);
  oled.println("Testing userLEDs");
  oled.setCursor(30, 20);
  oled.println("userLEDs Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  Serial.println("-------------------------------------");
  delay(1000);
}

void ultrasonicTesting(void) {
  Serial.println("___Ultrasonic Sensor Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(12, 2);
  oled.println("Testing Ultrasonic");
  oled.setCursor(0, 20);
  oled.println("Press OKAY: Working");
  oled.setCursor(0, 35);
  oled.println("Press BACK: Defected");
  oled.setCursor(0, 47);
  oled.println("Distance(cm) : ");
  oled.display();
  Serial.println("If WORKING press OKAY Button, else press BACK Button");

  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("Ultrasonic Sensor Testing : Pass\t✅");
      testResult |= (1 << 4); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("Ultrasonic Sensor Testing : Fail\t❌");
      testResult &= ~(1 << 4); // Clear bit if not working
      break;
    }

    long ultrasnicRawValue = ultrasonicSensor.readCM();
    Serial.print("Distance (cm) : ");
    Serial.println(ultrasnicRawValue);
    oled.setTextSize(1);
    oled.setTextColor(WHITE, BLACK);
    oled.setCursor(94, 47);
    oled.println("      ");
    oled.setCursor(95, 47);
    oled.println(ultrasnicRawValue);
    oled.display();
    delay(500);
  }
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(12, 2);
  oled.println("Testing Ultrasonic");
  oled.setCursor(20, 20);
  oled.println("Ultrasonic Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  Serial.println("-------------------------------------");
  delay(1000);
}

void potentiometerTesting(void) {
  Serial.println("___Potentiometer Sensor Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 2);
  oled.println("Testing Potentiometer");
  oled.setCursor(0, 20);
  oled.println("Press OKAY: Working");
  oled.setCursor(0, 35);
  oled.println("Press BACK: Defected");
  oled.setCursor(5, 47);
  oled.println("Raw value in %: ");
  oled.display();
  Serial.println("If WORKING press OKAY Button, else press BACK Button");

  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("Potentiometer Sensor Testing : Pass\t✅");
      testResult |= (1 << 5); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("Potentiometer Sensor Testing : Fail\t❌");
      testResult &= ~(1 << 5); // Clear bit if not working
      break;
    }
    int potPercentage = potentiometer.readPercentage();
    Serial.print("Potentiometer Value  : ");
    Serial.print(potPercentage);
    Serial.println(" %");
    oled.setTextSize(1);
    oled.setTextColor(WHITE, BLACK);
    oled.setCursor(96, 47);
    oled.println("      ");
    oled.setCursor(97, 47);
    oled.println(potPercentage);
    oled.display();
    delay(500);

  }
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 2);
  oled.println("Testing Potentiometer");
  oled.setCursor(8, 20);
  oled.println("Potentiometer Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  Serial.println("-------------------------------------");
  delay(1000);
}

uint8_t pressedButton(void) {
  if (buttons.read(backButton) == LOW) {
    buzzer.beep(200);
    return 1;
  }
  else if (buttons.read(downButton) == LOW) {
    buzzer.beep(200);
    return 2;
  }
  else if (buttons.read(upButton) == LOW) {
    buzzer.beep(200);
    return 3;
  }
  else if (buttons.read(okayButton) == LOW) {
    buzzer.beep(200);
    return 4;
  }
  else
    return 0;
}

void rgbLEDsTesting(void) {
  Serial.println("___RGB LEDs Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing rgbLEDs");
  oled.setCursor(0, 20);
  oled.println("Press OKAY: Blinking");
  oled.setCursor(0, 35);
  oled.println("Press BACK: No Blink");
  oled.display();
  Serial.println("--Check all LEDs are Turning on with RGB sequnce--");
  Serial.println("If WORKING press OKAY Button, else press BACK Button");

  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("rgbLEDs : Pass\t✅");
      testResult |= (1 << 6); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("rgbLEDs : Fail\t❌");
      testResult &= ~(1 << 6); // Clear userLEDs bit if not working
      break;
    }

    for (uint8_t i = 0; i < 5; i++) {
      rgbLEDs.write(i, 255, 0, 0);
    }
    rgbLEDs.show();
    delay(500);

    buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("rgbLEDs : Pass\t✅");
      testResult |= (1 << 6); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("rgbLEDs : Fail\t❌");
      testResult &= ~(1 << 6); // Clear userLEDs bit if not working
      break;
    }
    for (uint8_t i = 0; i < 5; i++) {
      rgbLEDs.write(i, 0, 255, 0);
    }
    rgbLEDs.show();
    delay(500);

    buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("rgbLEDs : Pass\t✅");
      testResult |= (1 << 6); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("rgbLEDs : Fail\t❌");
      testResult &= ~(1 << 6); // Clear userLEDs bit if not working
      break;
    }
    for (uint8_t i = 0; i < 5; i++) {
      rgbLEDs.write(i, 0, 0, 255);
    }
    rgbLEDs.show();
    delay(500);

  }
  rgbLEDs.clear();
  rgbLEDs.show();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(25, 2);
  oled.println("Testing rgbLEDs");
  oled.setCursor(30, 20);
  oled.println("rgbLEDs Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  Serial.println("-------------------------------------");
  delay(1000);
}

void relaysTesting(void) {
  Serial.println("___Relays Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing Relays");
  oled.setCursor(0, 20);;
  oled.println("Press OKAY: Switching");
  oled.setCursor(0, 35);
  oled.println("Press BACK: No Switch");
  oled.display();
  Serial.println("--Observe the both Relays are switching or Not--");
  Serial.println("If WORKING press OKAY Button, else press BACK Button");

  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("Relays Testing : Pass\t✅");
      testResult |= (1 << 7); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("Relays Testing : Fail\t❌");
      testResult &= ~(1 << 7); // Clear bit if not working
      break;
    }

    relays.writeRLY1(HIGH);
    relays.writeRLY2(LOW);
    delay(500);
    relays.writeRLY2(HIGH);
    relays.writeRLY1(LOW);
    delay(500);
  }
  relays.writeRLY1(LOW);
  relays.writeRLY2(LOW);
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(25, 2);
  oled.println("Testing Relays");
  oled.setCursor(30, 20);
  oled.println("Relays Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  Serial.println("-------------------------------------");
  delay(1000);

}

void servoMotorTesting(void) {
  Serial.println("___Servo Motor Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(5, 2);
  oled.println("Testing Servo Motor");
  oled.setCursor(0, 20);
  oled.println("Press OKAY: Sweeping");
  oled.setCursor(0, 35);
  oled.println("Press BACK: No Sweep");
  oled.display();
  Serial.println("If WORKING press OKAY Button, else press BACK Button");
  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("Servo Motor Testing : Pass\t✅");
      testResult |= (1 << 8); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("Servo Motor Testing : Fail\t❌");
      testResult &= ~(1 << 8); // Clear bit if not working
      break;
    }

    servoMotor.sweep();
    delay(100);
  }
  servoMotor.end();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(5, 2);
  oled.println("Testing Servo Motor");
  oled.setCursor(10, 20);
  oled.println("Servo Motor Test");
  oled.setCursor(35, 35);
  oled.println("Completed");
  oled.display();
  Serial.println("-------------------------------------");
  delay(1000);
}

void RTCModuleTesting(void) {
  Serial.println("___RTC Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(25, 2);
  oled.println("Testing RTC");

  if (rtc.isRunning() == false)
  {
    testResult &= ~(1 << 9);
    Serial.println("RTC Oscillator is NOT running! ❌");
    oled.setCursor(0, 20);
    oled.println("RTC Oscillator");
    oled.setCursor(30, 35);
    oled.println("Failed!");
    oled.display();
    Serial.println("-------------------------------------");
    delay(1000);
    return;
  }
  else {
    // read Date and Time and display it
    // Read the current time from the DS3231 RTC
    String time = rtc.readTime();
    // Read the current date from the DS3231 RTC
    String date = rtc.readDate();

    // Print the current time and date
    Serial.println("Current Time: " + time);
    Serial.println("Current Date: " + date);
    Serial.println("Current Year: " + String(rtc.getYear()));
    Serial.println("Current Day Number: " + String(rtc.getDayNumber()));
    Serial.println("Current Day Name: " + rtc.getDayName());
    Serial.println("Current Month Name: " + rtc.getMonthName());
    Serial.println("Current Month Number: " + String(rtc.getMonthNumber()));
    Serial.println("\n RTC Testing : Pass\t✅");
    Serial.println("---------------------------------------------------------");
    oled.setCursor(0, 20);
    oled.println(" Date : " + date);
    oled.setCursor(0, 35);
    oled.println(" Time : " + time);
    oled.display();
    delay(3000);
    return;
  }
}

void AHT20_Testing(void) {
  Serial.println("___AHT20 Temperature and Humidity Sensor Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(25, 2);
  oled.println("Testing AHT20");
  oled.setCursor(0, 20);
  oled.println("Press OKAY: Working");
  oled.setCursor(0, 35);
  oled.println("Press BACK: Defected");
  oled.setCursor(0, 47);
  oled.println("Temperature(C): ");
  oled.display();
  Serial.println("If WORKING press OKAY Button, else press BACK Button");

  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("AHT20 : Pass\t✅");
      testResult |= (1 << 10); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("AHT20 : Fail\t❌");
      testResult &= ~(1 << 10); // Clear bit if not working
      break;
    }
    float tempC = aht20Sensor.readTempC();
    float humi = aht20Sensor.readHumi();

    Serial.print("Temperature in Celsius : ");
    Serial.print(tempC);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.println(" %");
    oled.setTextSize(1);
    oled.setTextColor(WHITE, BLACK);
    oled.setCursor(94, 47);
    oled.println("      ");
    oled.setCursor(95, 47);
    oled.println(tempC);
    oled.display();
    delay(500);
  }
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(27, 2);
  oled.println("Testing AHT20");
  oled.setCursor(28, 20);
  oled.println("AHT20 Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  Serial.println("-------------------------------------");
  delay(1000);
}

void mpuSensorTesting(void) {
  Serial.println("___MPU6050 Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing MPU6050");
  oled.setCursor(5, 20);
  oled.println("gX: ");
  oled.setCursor(5, 35);
  oled.println("gY: ");
  oled.setCursor(5, 47);
  oled.println("gZ: ");

  oled.setCursor(63, 20);
  oled.println("aX: ");
  oled.setCursor(63, 35);
  oled.println("aY: ");
  oled.setCursor(63, 47);
  oled.println("aZ: ");
  oled.setCursor(28, 57);
  oled.println("Device: ");
  oled.display();
  Serial.println("Press OKAY if Working, Press BACK if Not Working");

  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("MPU6050 : Pass\t✅");
      testResult |= (1 << 12); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("MPU6050 : Fail\t❌");
      testResult &= ~(1 << 12); // Clear bit if not working
      break;
    }

    // Read the acceleration from the MPU6050
    float accX = mpuSensor.readAccX();
    float accY = mpuSensor.readAccY();
    float accZ = mpuSensor.readAccZ();
    Serial.print("Acceleration: ");
    Serial.print(accX);
    Serial.print(", ");
    Serial.print(accY);
    Serial.print(", ");
    Serial.print(accZ);
    Serial.println(" m/s^2");

    // Read the gyroscope from the MPU6050
    float gyroX = mpuSensor.readGyroX();
    float gyroY = mpuSensor.readGyroY();
    float gyroZ = mpuSensor.readGyroZ();
    Serial.print("Gyroscope: ");
    Serial.print(gyroX);
    Serial.print(", ");
    Serial.print(gyroY);
    Serial.print(", ");
    Serial.print(gyroZ);
    Serial.println(" rad/s");

    // Read the motion status from the MPU6050
    byte motionStatus = mpuSensor.getMotionState();
    if (motionStatus == 1)
    {
      oled.setTextSize(1);
      oled.setTextColor(WHITE, BLACK);
      oled.setCursor(75, 57);
      oled.println("          ");
      oled.setCursor(75, 57);
      oled.println("Moving");
      Serial.println("Motion Status: Moving");
    }
    else
    {
      oled.setTextSize(1);
      oled.setTextColor(WHITE, BLACK);
      oled.setCursor(75, 57);
      oled.println("          ");
      oled.setCursor(75, 57);
      oled.println("Idle");
      Serial.println("Motion Status: Stationary/Idle");
    }
    Serial.println("------     -----      -----          -----\n");

    oled.setCursor(25, 20);
    oled.println("      ");
    oled.setCursor(25, 20);
    oled.println(int(gyroX));
    oled.setCursor(25, 35);
    oled.println("      ");
    oled.setCursor(25, 35);
    oled.println(int(gyroY));
    oled.setCursor(25, 47);
    oled.println("      ");
    oled.setCursor(25, 47);
    oled.println(int(gyroZ));
    oled.setCursor(80, 20);
    oled.println("      ");
    oled.setCursor(80, 20);
    oled.println(int(accX));
    oled.setCursor(80, 35);
    oled.println("      ");
    oled.setCursor(80, 35);
    oled.println(int(accY));
    oled.setCursor(80, 47);
    oled.println("      ");
    oled.setCursor(80, 47);
    oled.println(int(accZ));
    oled.display();

    delay(500);
    buttonState = pressedButton();

    if (buttonState == 4) {
      Serial.println("MPU6050 : Pass\t✅");
      testResult |= (1 << 12); // Set bit if working
      break;
    }
    else if (buttonState == 1) {
      Serial.println("MPU6050 : Fail\t❌");
      testResult &= ~(1 << 12); // Clear bit if not working
      break;
    }
    delay(500);
  }
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing MPU6050");
  oled.setCursor(28, 20);
  oled.println("MPU6050 Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  Serial.println("-------------------------------------");
  delay(1000);
}

void APDS9960_Testing(void) {
  Serial.println("___APDS9960 Testing___");
  Serial.println("Press OKAY if Working, Press BACK if Not Working");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing APDS9960");
  oled.setCursor(5, 20);
  oled.println("Color: ");
  oled.setCursor(5, 35);
  oled.println("Proximity: ");
  oled.setCursor(5, 47);
  oled.println("Gesture: ");
  oled.display();

  // Test Color
  apds.enableColor();
  Serial.println("APDS Color Sensing Initiated");
  uint16_t red, green, blue, clear;
  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      testResult |= (1 << 13); // Set bit if working
      Serial.println("Color Sensor Passed");
      break;
    }
    else if (buttonState == 1) {
      testResult &= ~(1 << 13); // Clear bit if not working
      Serial.println("Color Sensor Failed");
      break;
    }
    // Check if color data is ready
    if (apds.colorDataReady())
    {
      // Read color data
      apds.readColors(&red, &green, &blue, &clear);

      // Get color name using both methods
      String colorName = getColorNameHSV(red, green, blue, clear);

      // Print both raw values and color names
      Serial.println("Color Detection Results:");

      // Raw RGB values
      Serial.print("Red: ");
      Serial.print(red);
      Serial.print(" | Green: ");
      Serial.print(green);
      Serial.print(" | Blue: ");
      Serial.print(blue);
      Serial.print(" | Clear: ");
      Serial.println(clear);

      // Color names
      Serial.print("Detected Color (Method 2-HSV): ");
      Serial.println(colorName);
      Serial.println("----    ----    ----    ----    ----");

      //Write Color on OLED Display
      oled.setTextSize(1);
      oled.setTextColor(WHITE, BLACK);
      oled.setCursor(70, 20);
      oled.println("          ");
      oled.setCursor(70, 20);
      oled.println(colorName);
      oled.display();
    }
    delay(500);
  }

  Serial.println("Color Test Completed");
  oled.setTextSize(1);
  oled.setTextColor(WHITE, BLACK);
  oled.setCursor(70, 20);
  oled.println("       ");
  oled.setCursor(70, 20);
  oled.println("PASS");
  oled.display();
  delay(1000);

  // Test Proximity
  Serial.println("APDS Proximity Sensing Initiated");
  apds.enableProximity();
  apds.setProximityInterruptThreshold(0, 175);
  apds.enableProximityInterrupt();
  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      testResult |= (1 << 13); // Set bit if working
      Serial.println("Proximity Sensor Passed");
      break;
    }
    else if (buttonState == 1) {
      testResult &= ~(1 << 13); // Clear bit if not working
      Serial.println("Proximity Sensor Failed");
      break;
    }

    if (digitalRead(APDSINT) == LOW)
    {
      Serial.print("Proximity Sensor Data: ");
      uint8_t proximityData = apds.readProximity();
      Serial.println(proximityData);
      oled.setTextSize(1);
      oled.setTextColor(WHITE, BLACK);
      oled.setCursor(70, 35);
      oled.println("      ");
      oled.setCursor(70, 35);
      oled.println(proximityData);
      oled.display();

      //clear the interrupt flag
      apds.clearInterrupt();
    }

    delay(250);
  }
  Serial.println("Proximity Test Completed");
  oled.setTextSize(1);
  oled.setTextColor(WHITE, BLACK);
  oled.setCursor(70, 35);
  oled.println("      ");
  oled.setCursor(70, 35);
  oled.println("PASS");
  oled.display();
  delay(1000);

  // Test Geture
  Serial.println("APDS Gesture Sensing Initiated");
  apds.enableProximity();
  apds.enableGesture();
  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      testResult |= (1 << 13); // Set bit if working
      Serial.println("Gesture Sensor Passed");
      break;
    }
    else if (buttonState == 1) {
      testResult &= ~(1 << 13); // Clear bit if not working
      Serial.println("Gesture Sensor Failed");
      break;
    }

    uint8_t gestureData = apds.readGesture();
    oled.setTextSize(1);
    oled.setTextColor(WHITE, BLACK);
    if (gestureData == APDS9960_DOWN)
    {
      Serial.println("Down");
      oled.setCursor(70, 47);
      oled.println("       ");
      oled.setCursor(70, 47);
      oled.println("DOWN");
    }
    if (gestureData == APDS9960_UP)
    {
      Serial.println("Up");
      oled.setCursor(70, 47);
      oled.println("        ");
      oled.setCursor(70, 47);
      oled.println("UP");
    }
    if (gestureData == APDS9960_LEFT)
    {
      Serial.println("Left");
      oled.setCursor(70, 47);
      oled.println("        ");
      oled.setCursor(70, 47);
      oled.println("LEFT");
    }
    if (gestureData == APDS9960_RIGHT)
    {
      Serial.println("Right");
      oled.setCursor(70, 47);
      oled.println("        ");
      oled.setCursor(70, 47);
      oled.println("Right");
    }
    oled.display();

    delay(200);
  }
  Serial.println("Gesture Test Completed");
  oled.setTextSize(1);
  oled.setTextColor(WHITE, BLACK);
  oled.setCursor(70, 47);
  oled.println("        ");
  oled.setCursor(70, 47);
  oled.println("PASS");
  oled.display();
  delay(3000);

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing APDS9960");
  oled.setCursor(28, 20);
  oled.println("APDS9960 Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  apds.end();
  Serial.println("-------------------------------------");
  delay(1000);
}

String getColorNameHSV(uint16_t red, uint16_t green, uint16_t blue, uint16_t clear) {
  // Normalize to 0-255 range
  if (clear == 0) return "Dark";

  float r = (float)red / clear * 255.0;
  float g = (float)green / clear * 255.0;
  float b = (float)blue / clear * 255.0;

  r = constrain(r, 0, 255) / 255.0;
  g = constrain(g, 0, 255) / 255.0;
  b = constrain(b, 0, 255) / 255.0;

  // Calculate HSV
  float maxRGB = max(max(r, g), b);
  float minRGB = min(min(r, g), b);
  float delta = maxRGB - minRGB;

  // Value (brightness)
  float v = maxRGB;

  // Saturation
  float s = (maxRGB > 0) ? (delta / maxRGB) : 0;

  // Hue
  float h = 0;
  if (delta != 0) {
    if (maxRGB == r) {
      h = 60.0 * fmod(((g - b) / delta), 6.0);
    } else if (maxRGB == g) {
      h = 60.0 * (((b - r) / delta) + 2.0);
    } else {
      h = 60.0 * (((r - g) / delta) + 4.0);
    }
  }

  if (h < 0) h += 360.0;

  // Determine color name based on HSV
  // Low value = dark colors
  if (v < 0.15) return "Black";

  // Low saturation = grayscale
  if (s < 0.15) {
    if (v > 0.85) return "White";
    if (v > 0.60) return "Light Gray";
    if (v > 0.35) return "Gray";
    return "Dark Gray";
  }

  // High saturation - determine by hue
  if (h < 15 || h >= 345) return "Red";
  if (h < 45) return "Orange";
  if (h < 75) return "Yellow";
  if (h < 150) return "Green";
  if (h < 210) return "Cyan";
  if (h < 270) return "Blue";
  if (h < 315) return "Purple";
  if (h < 345) return "Magenta";

  return "Unknown";
}

void max30102Testing(void) {
  int32_t SPO2 = 0;
  int32_t heartRate = 0;
  int8_t SPO2Valid = 0;
  int8_t heartRateValid = 0;
  oximeter.configure(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4,
                                       /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100,
                                       /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);

  Serial.println("___MAX30102 Testing___");
  Serial.println("Press OKAY if Working, Press BACK if Not Working");
  Serial.println("Place your finger on the sensor");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing MAX30102");
  oled.setCursor(18, 20);
  oled.println("Place Your Finger");
  oled.setCursor(10, 35);
  oled.println("SPO2: ");
  oled.setCursor(10, 47);
  oled.println("HeartRate: ");
  oled.display();

  while (1) {
    uint8_t buttonState = pressedButton();

    if (buttonState == 4) {
      testResult |= (1 << 14); // Set bit if working
      Serial.println("MAX30102 Sensor Passed  ✅");
      break;
    }
    else if (buttonState == 1) {
      testResult &= ~(1 << 14); // Clear bit if not working
      Serial.println("Max30102 Sensor Failed! ❌");
      break;
    }

    oximeter.readSPO2(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);

    Serial.print(F("heartRate :"));
    Serial.println(heartRate, DEC);

    Serial.print(F("heartRateValid :"));
    Serial.println(heartRateValid, DEC);

    Serial.print(F("SPO2 :"));
    Serial.println(SPO2, DEC);

    Serial.print(F("SPO2Valid :"));
    Serial.println(SPO2Valid, DEC);
    Serial.println("----    ----    ----    ----    ----");

    oled.setTextSize(1);
    oled.setTextColor(WHITE, BLACK);
    oled.setCursor(75, 35);
    oled.println("       ");
    oled.setCursor(75, 35);
    oled.println(SPO2, DEC);
    oled.setCursor(75, 47);
    oled.println("       ");
    oled.setCursor(75, 47);
    oled.println(heartRate, DEC);
    oled.display();

    delay(500);

  }
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing MAX30102");
  oled.setCursor(28, 20);
  oled.println("MAX30102 Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  oximeter.end();
  Serial.println("-------------------------------------");
  delay(1000);
}

void WiFi_HTTP_Testing(void) {
  wifi.begin();
  wifi.configure(WIFI_STA);
  Serial.println("___WiFi_HTTP Testing___");
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing WiFi_HTTP");
  oled.setCursor(10, 20);
  oled.println("Conneting to WiFi..");
  oled.display();
  Serial.println("Connecting to WiFi...");
  wifi.connect(WiFi_SSID, WiFi_PASSWORD);

  while (!wifi.readStatus())
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  oled.setTextSize(1);
  oled.setTextColor(WHITE, BLACK);
  oled.setCursor(10, 20);
  oled.println("                   ");
  oled.setCursor(12, 20);
  oled.println(" WiFi Connected ");
  oled.setCursor(12, 35);
  oled.println("Testing Internet");
  oled.display();

  // Check internet connectivity
  if (wifi.checkInternet())
  {
    Serial.println("Internet connection is available");
    oled.setCursor(10, 47);
    oled.println("Internet Available");
  }
  else
  {
    Serial.println("No internet connection");
    oled.setCursor(10, 47);
    oled.println("No Internet!!!");
  }
  oled.display();
  delay(3000);

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 2);
  oled.println("Testing WiFi_HTTP");
  oled.setCursor(28, 20);
  oled.println("WiFi_HTTP Test");
  oled.setCursor(33, 35);
  oled.println("Completed");
  oled.display();
  wifi.end();
  delay(2000);
  Serial.println("-------------------------------------");

}
