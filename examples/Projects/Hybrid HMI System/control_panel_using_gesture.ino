/* Hybrid HMI(Human Machine Interface) system 
  This project demonstrates without the touching the device we can opertate the device 
  How to use this : 
  Up gesture = upwards(arrow goes up) 
  Down gesture = downwards(arrow goes down) 
  Left gesture = Select the option
  Right gesture = Back(Returing to the main menu) 
*/

#include <LF_Alpha.h>

// Create a instances of the used peripherials 
LF_Alpha_APDS9960 gestureSensor;
LF_Alpha_OLED OLED;
LF_Alpha_rgbLEDs leds(5);
LF_Alpha_Buzzer buzzer;
LF_Alpha_RTC rtc;
LF_Alpha_AHT20 tempSensor;
LF_Alpha_Buttons buttons;
 
//Global vairiables
int menuIndex = 0;

String menuItems[4] = {
  "Blink LED",
  "Show Time",
  "Buzzer",
  "Temperature"
};

enum Action {
  ACTION_NONE = 0,
  ACTION_UP,
  ACTION_DOWN,
  ACTION_SELECT,
  ACTION_BACK
};

/* ================= ACTION HELPER (Gestures + Buttons) ================= */

Action getAction() {
  // Read Buttons First (Using the logic requested)
  if (buttons.read(upButton) == LOW) {
    Serial.println("Button: UP");
    delay(200);
    return ACTION_UP;
  }
  if (buttons.read(downButton) == LOW) {
    Serial.println("Button: DOWN");
    delay(200);
    return ACTION_DOWN;
  }
  if (buttons.read(okayButton) == LOW) {
    Serial.println("Button: OKAY (SELECT)");
    delay(200);
    return ACTION_SELECT;
  }
  if (buttons.read(backButton) == LOW) {
    Serial.println("Button: BACK");
    delay(200);
    return ACTION_BACK;
  }


  static unsigned long lastGestureTime = 0;

  // DEBOUNCE: ignore gestures within 400ms of the last accepted one
  if (millis() - lastGestureTime < 400) {
    return ACTION_NONE;
  }

  // Read directly — matches official library example (no gestureValid() needed)
  uint8_t g = gestureSensor.readGesture();

  // VALIDATION: only accept known gesture values, reject noise/0
  if (g != APDS9960_UP    &&
      g != APDS9960_DOWN  &&
      g != APDS9960_LEFT  &&
      g != APDS9960_RIGHT) {
    return ACTION_NONE;
  }

  // Valid gesture — apply debounce timestamp
  lastGestureTime = millis();

  if (g == APDS9960_UP) {
    Serial.println("Gesture: UP");
    return ACTION_UP;
  }
  if (g == APDS9960_DOWN) {
    Serial.println("Gesture: DOWN");
    return ACTION_DOWN;
  }
  if (g == APDS9960_LEFT) {
    Serial.println("Gesture: LEFT -> SELECT");
    return ACTION_SELECT;
  }
  // g == APDS9960_RIGHT
  Serial.println("Gesture: RIGHT -> BACK");
  return ACTION_BACK;
}



/* SPLASH SCREEN  */

void showFirstScreen() {
  OLED.clearDisplay();
  OLED.setTextSize(2);
  OLED.setTextColor(SSD1306_WHITE);

  // New first screen displaying: Gesture Menu
  // Centered on 128x64 OLED, textSize 2 (each char = 12x16px)
  OLED.setCursor(28, 5);   // "Hybrid" = 6×12=72px → (128-72)/2=28
  OLED.println("Hybrid");
  OLED.setCursor(46, 23);  // "HMI"    = 3×12=36px → (128-36)/2=46
  OLED.println("HMI");
  OLED.setCursor(28, 41);  // "System" = 6×12=72px → (128-72)/2=28
  OLED.println("System");

  OLED.display();
  delay(2000); 
}

/* ================= INSTRUCTIONS SCREEN ================= */

void showInstructions() {
  OLED.clearDisplay();
  OLED.setTextSize(1);
  OLED.setTextColor(SSD1306_WHITE);

  OLED.setCursor(15, 0);
  OLED.println("How to use menu");

  OLED.setCursor(0, 20);
  OLED.println("UP    = Upwards");
  OLED.setCursor(0, 30);
  OLED.println("DOWN  = Downwards");
  OLED.setCursor(0, 40);
  OLED.println("LEFT  = Select"); 
  OLED.setCursor(0, 50);
  OLED.println("RIGHT = Back");
  

  OLED.display();

  // Wait max 5 seconds, but exit immediately on any button press
  unsigned long start = millis();
  while (millis() - start < 5000) {
    if (buttons.read(okayButton) == LOW ||
        buttons.read(upButton)   == LOW ||
        buttons.read(downButton) == LOW ||
        buttons.read(backButton) == LOW) {
      delay(200); // button debounce
      break;
    }
    delay(20);
  }
}

/* ================= MENU DRAW ================= */

void drawMenu() {
  OLED.clearDisplay();
  
  OLED.setTextSize(2);
  OLED.setTextColor(SSD1306_WHITE);
  OLED.setCursor(30, 0);
  OLED.println("Menu");

  OLED.setTextSize(1);

  int totalItems = 4;

  for (int i = 0; i < totalItems; i++) {
    OLED.setCursor(0, 20 + (i * 11)); // Starts at Y=20, spacing 11px to fit 4 items

    if (i == menuIndex)
      OLED.print("> ");
    else
      OLED.print("  ");

    OLED.println(menuItems[i]);
  }

  OLED.display();
}

/* ================= LED SCREEN ================= */

void blinkLED() {
  unsigned long lastBlink = 0;
  bool ledState = false;
  const unsigned long BLINK_INTERVAL = 300; 

  OLED.clearDisplay();
  OLED.setTextSize(2);
  OLED.setCursor(35, 5);//(x,y)
  OLED.println("LED");
  OLED.setCursor(25, 25);
  OLED.println("BLINK");
  OLED.setTextSize(1);
  OLED.setCursor(0, 50);
  OLED.println("RIGHT/BACK BTN = BACK");
  OLED.display();

  while (true) {
    if (millis() - lastBlink >= BLINK_INTERVAL) {
      lastBlink = millis();
      ledState = !ledState;

      if (ledState) {
        for (int i = 0; i < 5; i++) leds.write(i, 0, 0, 255);
      } else {
        leds.clear();
      }
      leds.show();
    }

    Action a = getAction();
    if (a == ACTION_BACK) {
      leds.clear();
      leds.show();
      drawMenu();
      return;
    }

    delay(20); 
  }
}

/* ================= TIME SCREEN ================= */

void showTime() {
  unsigned long lastDraw = 0;
  const unsigned long DRAW_INTERVAL = 500;

  while (true) {
    if (millis() - lastDraw >= DRAW_INTERVAL) {
      lastDraw = millis();

      OLED.clearDisplay();
      OLED.setTextSize(2);
      OLED.setCursor(10, 0);
      OLED.println("TIME");
      OLED.setCursor(5, 30);
      OLED.println(rtc.readTime());
      
      OLED.setTextSize(1);
      OLED.setCursor(0, 50);
      OLED.println("RIGHT/BACK BTN = BACK");
      OLED.display();
    }

    Action a = getAction();
    if (a == ACTION_BACK) {
      drawMenu();
      return;
    }

    delay(20);
  }
}

/* ================= BUZZER SCREEN ================= */

void beepBuzzer() {
  unsigned long lastBeep = 0;
  const unsigned long BEEP_INTERVAL = 600;

  OLED.clearDisplay();
  OLED.setTextSize(2);
  OLED.setCursor(25, 5);
  OLED.println("BUZZER");
  OLED.setTextSize(1);
  OLED.setCursor(0, 50);
  OLED.println("RIGHT/BACK BTN = BACK");
  OLED.display();

  while (true) {
    if (millis() - lastBeep >= BEEP_INTERVAL) {
      lastBeep = millis();
      buzzer.beep(80); 
    }

    Action a = getAction();
    if (a == ACTION_BACK) {
      drawMenu();
      return;
    }

    delay(20);
  }
}

/* ================= TEMP SCREEN ================= */

void showTemperature() {
  unsigned long lastDraw = 0;
  const unsigned long DRAW_INTERVAL = 500;

  while (true) {
    if (millis() - lastDraw >= DRAW_INTERVAL) {
      lastDraw = millis();
      float t = tempSensor.readTempC();

      OLED.clearDisplay();
      OLED.setTextSize(2);
      OLED.setCursor(10, 0);
      OLED.println("TEMP");
      OLED.setCursor(10, 30);
      OLED.print(t);
      OLED.println(" C");

      OLED.setTextSize(1);
      OLED.setCursor(0, 50);
      OLED.println("RIGHT/BACK BTN = BACK");
      OLED.display();
    }

    Action a = getAction();
    if (a == ACTION_BACK) {
      drawMenu();
      return;
    }

    delay(20);
  }
}

/* ================= MENU EXECUTION ================= */

void executeMenu() {
  if (menuIndex == 0) blinkLED();
  if (menuIndex == 1) showTime();
  if (menuIndex == 2) beepBuzzer();
  if (menuIndex == 3) showTemperature();
}

/* ================= SETUP ================= */

void setup() {
  Serial.begin(115200);

  // Initialize all the buttons
  buttons.begin(upButton, downButton, backButton, okayButton);

  bool status = gestureSensor.begin();
  if (!status) {
    Serial.println("Gesture sensor failed");
    while (1)
      ;
  }

  gestureSensor.enableProximity();
  gestureSensor.enableGesture();

  // MUST BE AFTER enableGesture() because enableGesture() resets the sensor registers!
  // Lower threshold so sensor is less strict — hand doesn't need to be too close
  gestureSensor.setGestureProximityThreshold(100);
  
  // Increase gain for more stable, reliable gesture detection
  gestureSensor.setGestureGain(2);

  OLED.begin();
  leds.begin();
  buzzer.begin();
  rtc.begin();
  tempSensor.begin();

  Serial.println("Gesture Menu Ready");

  // Show "Gesture Menu" First Screen (Waits for input)
  showFirstScreen();
  
  // Show Instructions (Waits for input)
  showInstructions();
  
  // Draw main menu initially
  drawMenu();
}

/* ================= LOOP ================= */

void loop() {
  Action a = getAction();

  if (a == ACTION_UP) {
    if (menuIndex > 0) menuIndex--;
    drawMenu();
  }

  if (a == ACTION_DOWN) {
    if (menuIndex < 3) menuIndex++;
    drawMenu();
  }

  if (a == ACTION_SELECT) {
    executeMenu();
    drawMenu(); // redraw menu upon returning
  }

  delay(5); 
}
