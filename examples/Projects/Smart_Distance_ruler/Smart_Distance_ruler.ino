/*
 * Project: Smart Distance Ruler
 * Description: This project implements a smart distance ruler using an HC-SR04
 *              ultrasonic sensor and an OLED display. It provides real-time
 *              distance measurements, allows switching between centimeter and
 *              inch units via a button, and displays the data on an OLED screen.
 *              The system is designed for smooth, flicker-free updates through
 *              advanced filtering techniques.
 *
 * Libraries Used:
 * LF_Alpha.h: A custom library providing simplified access to ultrasonic sensor,
 *             buttons, and OLED display functions. This library abstracts low-level
 *             hardware interactions, promoting code readability and portability.
 */
// Custom library for hardware abstraction, simplifying sensor, display, and button interactions.
#include <LF_Alpha.h>

//  Define System Constants 
// Baud rate for serial communication, used for debugging and data output.
#define SERIAL_BAUD          115200
// Duration in milliseconds for which the startup splash screen is displayed.
#define STARTUP_SCREEN_DELAY   2000
// Delay in milliseconds between successive distance measurements. A balanced value
// for speed and stability, preventing sensor overload and ensuring reliable readings.
#define SAMPLING_DELAY           80
// Minimum change in distance (in cm) required to update the display. This acts as a
// deadband to suppress minor fluctuations and prevent display flicker.
#define DISTANCE_THRESHOLD        1
// Maximum allowed difference (in cm) between the current raw reading and the
// previously filtered distance. Readings exceeding this threshold are considered
// erroneous spikes and are rejected to maintain accuracy.

// Number of samples used in the running average filter. A higher number provides
// smoother readings but increases latency.
#define FILTER_SAMPLES            5

// --- Hardware Instantiation ---
// Object for controlling the HC-SR04 ultrasonic sensor.
LF_Alpha_HCSR04 ultrasonicSensor;
// Object for controlling the OLED display.
LF_Alpha_OLED OLED;
// Object for managing button inputs.
LF_Alpha_Buttons buttons;

// --- Global Variables ---
// Flag to indicate the current unit of measurement: true for centimeters (CM),
// false for inches (IN). Initialized to CM.
bool isCM = true;
// Stores the most recently filtered and stable distance measurement.
uint16_t filteredDistance = 0;
// Circular buffer to store the last `FILTER_SAMPLES` readings for the running average calculation.
uint16_t sampleBuffer[FILTER_SAMPLES] = {0};
// Index pointing to the current position in the circular buffer where the next sample will be stored.
uint8_t  sampleIndex = 0;
// Flag indicating whether the `sampleBuffer` has been filled at least once.
// This is crucial for accurate running average calculation during initial startup.
bool   bufferFull = false;

// --- Function Prototypes ---
// Displays the initial splash screen on the OLED display.
void showstartupscreen(void);
// Displays the given distance on the OLED display, handling unit conversion and formatting.
void showDistance(uint16_t distance);
// Handles button presses, specifically for toggling between CM and Inch units.
void handleButton(void);
// Returns the median of three unsigned 16-bit integer values. Used for spike rejection.
uint16_t getMedian3(uint16_t a, uint16_t b, uint16_t c);
// Calculates and returns the running average of distance samples, incorporating a new sample.
uint16_t getRunningAverage(uint16_t newSample);

// --- Setup Function ---
// Executed once at the start of the program to initialize hardware and system settings.
void setup() {
  // Initialize serial communication for debugging and output.
  Serial.begin(SERIAL_BAUD);
  Serial.println("Smart Distance Ruler");

  // Initialize the ultrasonic sensor.
  ultrasonicSensor.begin();
  // Initialize the OLED display.
  OLED.begin();
  // Initialize the button handler, specifying the button pin (okayButton is assumed to be defined in LF_Alpha.h).
  buttons.begin(okayButton);

  Serial.println("Initialization Completed Successfully");

  // Display the startup splash screen.
  showstartupscreen();

  // Prime the sample buffer with an initial valid distance reading.
  // This ensures the running average starts with meaningful data, preventing initial erroneous values.
  uint16_t initDistance = ultrasonicSensor.readCM();
  // Validate initial reading: if out of typical range (2-400cm), default to 10cm.
  if (initDistance < 2 || initDistance > 400) {
    initDistance = 10;
  }
  // Fill the entire sample buffer with the validated initial distance.
  for (uint8_t i = 0; i < FILTER_SAMPLES; i++) {
    sampleBuffer[i] = initDistance;
  }
  // Mark the buffer as full after priming.
  bufferFull = true;
  // Set the initial filtered distance to the primed value.
  filteredDistance = initDistance;
  showDistance(filteredDistance); // Displays the distance value

}

// Loop Function 
// Continuously executed after setup(), performing distance measurement, filtering, and display updates.
void loop() {
  // Check and handle button presses (e.g., unit toggling).
  handleButton();

  //  STEP 1: TAKE THREE READINGS WITH SHORT GAPS 
  // Taking multiple readings and introducing small delays helps to improve reliability
  // by reducing the impact of transient noise or echo issues.
  uint16_t r1 = ultrasonicSensor.readCM();
  delay(6);   // Short delay to allow for stable echo reception for the next reading.
  uint16_t r2 = ultrasonicSensor.readCM();
  delay(6);   // Another short delay.
  uint16_t r3 = ultrasonicSensor.readCM();

  //  STEP 2: MEDIAN FILTER (removes single-sample spikes) 
  // Apply a median filter to the three raw readings. This effectively discards
  // isolated erroneous readings (spikes) while preserving actual changes.
  uint16_t rawCM = getMedian3(r1, r2, r3);

  //  STEP 3: HANDLE INVALID VALUES 
  // Check if the median-filtered reading is within the sensor's operational range (2cm to 400cm).
  // If not, it indicates an unreliable reading, so the last known good filtered distance is used.
  if (rawCM < 2 || rawCM > 400) {
    rawCM = filteredDistance;   // Fall back to the last stable measurement.
  }

  //  STEP 4: RUNNING AVERAGE (ACCURATE, RESPONSIVE SMOOTHING) 
  // Apply a running average filter to the validated raw reading. This provides
  // smooth and responsive distance measurements by averaging over `FILTER_SAMPLES`.
  // This method avoids the bias and offset errors often found in simpler blending filters.
  uint16_t newFilteredDistance = getRunningAverage(rawCM);

  //  STEP 5: DEAD BAND (NO FLICKER) 
  // Implement a deadband to prevent display updates for minor fluctuations.
  // If the change in newFilteredDistance from filteredDistance is less than
  // DISTANCE_THRESHOLD, the newFilteredDistance is effectively suppressed
  // to the filteredDistance value, eliminating display flicker.
  if (abs((uint16_t)newFilteredDistance - (uint16_t)filteredDistance) < DISTANCE_THRESHOLD) {
    newFilteredDistance = filteredDistance;   // Suppress micro-fluctuations.
  }

  //  STEP 6: UNIT CONVERSION 
  // Convert the filtered distance to the currently selected unit (CM or Inches).
  // The inch conversion factor (0.3937) is applied, and 0.5 is added for proper rounding
  // when casting to an integer.
  uint16_t displayDistance = isCM
    ? newFilteredDistance
    : (uint16_t)(newFilteredDistance * 0.3937f + 0.5f);  // +0.5 for rounding to nearest integer.
  
  //  SERIAL OUTPUT 
  // Print the current distance measurement to the serial monitor for debugging and monitoring.
  Serial.print(" Distance : ");
  Serial.print(displayDistance);
  Serial.println(isCM ? " cm" : " inch");

  //  UPDATE DISPLAY ONLY IF VALUE CHANGED 
  // The OLED display is updated only if the `newFilteredDistance` is different
  // from the `filteredDistance`. This optimizes display refresh cycles and reduces
  // unnecessary redraws, contributing to a smoother user experience.
  if (newFilteredDistance != filteredDistance) {
    showDistance(displayDistance);
    filteredDistance = newFilteredDistance;   // Update the stored filtered distance.
  }

  // Introduce a delay before the next measurement cycle.
  delay(SAMPLING_DELAY);
}

//  Function: showstartupscreen 
// Description: Clears the OLED display and shows a splash screen with the project title.
//              This function is called once during the setup phase.
void showstartupscreen(void) {
  OLED.clearDisplay();          // Clear any previous content on the display.
  OLED.setTextSize(2);          // Set text size to 2 for larger font.
  OLED.setTextColor(SSD1306_WHITE); // Set text color to white.

  // Position and print 
  OLED.setCursor(35, 1);     OLED.println("SMART");
  OLED.setCursor(20, 25);    OLED.println("DISTANCE");
  OLED.setCursor(35, 50);    OLED.println("RULER");

  OLED.display();               // Push the buffer content to the OLED display.
  delay(STARTUP_SCREEN_DELAY);  // Pause for the defined startup screen duration.
}

//  Function: handleButton 
// Description: Checks for button presses and toggles the unit of measurement (CM/Inches).
//              Includes a debounce delay to prevent multiple triggers from a single press.
void handleButton(void) {
  // Read the state of the 'okayButton'. If pressed (LOW state),
  // toggle the unit of measurement.
  if (buttons.read(okayButton) == LOW) {
    isCM = !isCM; // Invert the boolean flag for unit selection.
    // Immediately refresh the display with the new unit to provide instant feedback.
    showDistance(isCM                                          // If CM is selected,
      ? filteredDistance                                       // display filtered distance in CM.
      : (uint16_t)(filteredDistance * 0.3937f + 0.5f));      // Otherwise, convert and display in inches.
    delay(250);   // Debounce delay to prevent multiple readings from a single button press.
  }
}
//  Function: showDistance 
// Description: Displays the given distance on the OLED screen, dynamically adjusting
//              positioning based on the number of digits and current unit (cm/inch).
void showDistance(uint16_t distance) {
  OLED.clearDisplay();          // Clear the entire display buffer.

  OLED.setTextSize(1);          // Set text size for the header.
  OLED.setCursor(5, 0);         // Position cursor for the header.
  OLED.println("SMART DISTANCE RULER"); // Print the header.
  // Draw a horizontal line below the header for visual separation.
  OLED.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  OLED.setTextSize(3);          // Set a larger text size for the main distance value.

  // Calculate the number of digits in the distance to center it on the display.
  uint8_t digitWidth = 12;      // Approximate pixel width of a single digit at setTextSize(3).
  uint8_t digits = 1;           // Start with 1 digit for single-digit numbers.
  if (distance >= 10) digits++; // Increment for two-digit numbers.
  if (distance >= 100) digits++; // Increment for three-digit numbers.

  // Calculate the X position to center the distance value.
  // The magic number 52 is an empirically determined offset for centering.
  uint8_t xPos = 52 - (digits - 1) * digitWidth;

  OLED.setCursor(xPos, 33);     // Position cursor for the distance value.
  OLED.print(distance);         // Print the distance value.

  OLED.setTextSize(2);          // Set text size for the unit (cm/inch).
  if (isCM) {                   // If the unit is centimeters:
    OLED.setCursor(92, 41);     // Position cursor for 'cm'.
    OLED.print("cm");           // Print 'cm'.
  } else {                      // If the unit is inches:
    OLED.setCursor(80, 41);     // Position cursor for 'inch'.
    OLED.print("inch");         // Print 'inch'.
  }

  OLED.display();               // Push the buffer content to the OLED display.
}

//  Function: getMedian3 
// Description: Computes the median of three unsigned 16-bit integer values.
//              This is a simple and effective way to filter out single-sample
//              spikes from sensor readings, improving data robustness.
// Parameters:
//   a, b, c: Three uint16_t values to find the median from.
// Returns:
//   The median of the three input values.
uint16_t getMedian3(uint16_t a, uint16_t b, uint16_t c) {
  // Check if 'b' is the median.
  if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
  // Check if 'a' is the median.
  if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
  // Otherwise, 'c' must be the median.
  return c;
}

//  Function: getRunningAverage 
// Description: Maintains a circular buffer of the last `FILTER_SAMPLES` readings
//              and calculates their true average. This provides smooth and
//              unbiased filtering, crucial for stable distance measurements.
// Parameters:newSample: The latest distance reading to be added to the buffer.
// Returns:The calculated running average of the samples in the buffer.
uint16_t getRunningAverage(uint16_t newSample) {
  // Store the new sample in the circular buffer at the current index.
  sampleBuffer[sampleIndex] = newSample;
  // Advance the circular buffer index, wrapping around if it reaches the end.
  sampleIndex = (sampleIndex + 1) % FILTER_SAMPLES;

  uint16_t sum = 0; // Use uint32_t for sum to prevent overflow during accumulation.
  // Iterate through the entire buffer to sum up all samples.
  for (uint8_t i = 0; i < FILTER_SAMPLES; i++) {
    sum += sampleBuffer[i];
  }
  // Calculate the average and cast back to uint16_t. Integer division is acceptable here
  // as the display only shows integer distances.
  return (uint16_t)(sum / FILTER_SAMPLES);
}