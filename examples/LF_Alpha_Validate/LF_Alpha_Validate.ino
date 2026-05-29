/*
    LF_Alpha Validate Code will be Testing all the peripherals of the
    LF_Alpha board and provides the encoded data on the OLED Display and the Serial Monitor

    After uploading the following code follow the instructions on the OLED Display or the Serial Monitor to interact with the different peripherals.

    Lets store the test result in 16 bit (2-byte) variable
    15 (MSB)    |   14   |    13    |    12   |  11  |  10   | 09  |  08   |
    WIFI + HTTP | MAX_IC | APDS9960 | MPU6050 | OLED | AHT20 | RTC | SERVO |

      07   |   06     |   05    |     04     |   03    |    02     |   01  | 00 (LSB)
    RELAYs | RGB_LEDs | RES_POT | ULTRASONIC | BUTTONs | USER_LEDs | BUZZER

    Note : set the bit-x if corresponding peripheral is working and clear if not working.

    Before uplaoding the code change the WiFi SSID and PASSWORD in the devLib.h file
*/

// Header Files
#include "devLib.h"

void setup() {
  systemInit();
  buttonsTest();
  oledTest();
}

void loop() {
  buzzerTest();
  userLEDs_Testing();
  ultrasonicTesting();
  potentiometerTesting();
  rgbLEDsTesting();
  relaysTesting();
  servoMotorTesting();
  RTCModuleTesting();
  AHT20_Testing();
  mpuSensorTesting();
  max30102Testing();
  APDS9960_Testing();
  WiFi_HTTP_Testing();
  showTestResult();
  while (1) {
    delay(200);         // some delay to avoid watchdog timer error
  }

}