/*
    The LF_Alpha.cpp is the source file for the LF_Alpha library, which provides
    functionalities to interact with various hardware components of the LF_Alpha
    development board, such as buzzers, user LEDs, buttons, ultrasonic sensors,
    potentiometers, and RGB LEDs.
    
    This file contains the implementation of the classes and methods declared
    in the LF_Alpha.h header file.
*/

// Include the Header File
#include <LF_Alpha.h>

bool i2cInitFlag = false;

// LF_Alpha_Buzzer class implementation
bool LF_Alpha_Buzzer::begin(void)
{
    pinMode(buzzerPin, OUTPUT);
    return true;
}

bool LF_Alpha_Buzzer::begin(uint8_t customGPIO)
{
    if (customGPIO == buzzerPin)
    {
        return begin();
    }

    else
    {
        customBuzzerPin = customGPIO;
        pinMode(customBuzzerPin, OUTPUT);
        return true;
    }
}

void LF_Alpha_Buzzer::write(bool state)
{
    if (customBuzzerPin == buzzerPin)
    {
        digitalWrite(buzzerPin, state);
    }
    else
    {
        digitalWrite(customBuzzerPin, state);
    }
    return;
}

void LF_Alpha_Buzzer::beep(uint16_t duration)
{
    if (customBuzzerPin == buzzerPin)
    {
        digitalWrite(buzzerPin, HIGH);
        delay(duration);
        digitalWrite(buzzerPin, LOW);
    }
    else
    {
        digitalWrite(customBuzzerPin, HIGH);
        delay(duration);
        digitalWrite(customBuzzerPin, LOW);
    }
}

bool LF_Alpha_Buzzer::end(void)
{
    customBuzzerPin = buzzerPin;
    gpio_reset_pin(gpio_num_t(buzzerPin));
    return true;
}

// LF_Alpha_userLEDs class implementation
bool LF_Alpha_userLEDs ::begin(uint8_t pin)
{
    if (pin == userLED1)
    {
        selectLED(1);
        return true;
    }
    else if (pin == userLED2)
    {
        selectLED(2);
        return true;
    }
    else
    {
        pinMode(pin, OUTPUT);
        return true;
    }
}

bool LF_Alpha_userLEDs ::begin(uint8_t led1, uint8_t led2)
{
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    return true;
}

void LF_Alpha_userLEDs ::blink(uint8_t led, uint16_t duration)
{
    if (led == userLED1)
    {
        digitalWrite(userLED1, LOW);
        delay(duration);
        digitalWrite(userLED1, HIGH);
        delay(duration);
    }
    else if (led == userLED2)
    {
        digitalWrite(userLED2, HIGH);
        delay(duration);
        digitalWrite(userLED2, LOW);
        delay(duration);
    }
    else
    {
        digitalWrite(led, HIGH);
        delay(duration);
        digitalWrite(led, LOW);
        delay(duration);
    }
}

void LF_Alpha_userLEDs ::writeLED1(bool state)
{
    digitalWrite(userLED1, state);
    return;
}

void LF_Alpha_userLEDs ::writeLED2(bool state)
{
    digitalWrite(userLED2, state);
    return;
}

void LF_Alpha_userLEDs ::write(int led, bool state)
{
    digitalWrite(led, state);
}

bool LF_Alpha_userLEDs ::end(void)
{
    gpio_reset_pin(gpio_num_t(userLED1));
    gpio_reset_pin(gpio_num_t(userLED2));
    return true;
}

bool LF_Alpha_Buttons ::begin(uint8_t pin)
{
    switch (pin)
    {
    case 11:
    {
        pinMode(backButton, INPUT);
        buttonFlag[pin - 1] = true;
        break;
    }
    case 14:
    {
        pinMode(downButton, INPUT);
        buttonFlag[pin - 1] = true;
        break;
    }
    case 13:
    {
        pinMode(upButton, INPUT);
        buttonFlag[pin - 1] = true;
        break;
    }
    case 12:
    {
        pinMode(okayButton, INPUT);
        buttonFlag[pin - 1] = true;
        break;
    }
    default:
    {
        pinMode(pin, INPUT);
    }
    }

    // Return true if initialization is successful
    return true;
}

bool LF_Alpha_Buttons ::begin(uint8_t pin1, uint8_t pin2)
{
    begin(pin1);
    begin(pin2);
    return true; // Return true if initialization is successful
}

bool LF_Alpha_Buttons ::begin(uint8_t pin1, uint8_t pin2, uint8_t pin3)
{
    begin(pin1);
    begin(pin2);
    begin(pin3);
    return true; // Return true if initialization is successful
}

bool LF_Alpha_Buttons ::begin(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
{
    begin(pin1);
    begin(pin2);
    begin(pin3);
    begin(pin4);
    return true; // Return true if initialization is successful
}

bool LF_Alpha_Buttons ::read(uint8_t button)
{
    // Read the state of the specified switch
    if (button == backButton)
        return digitalRead(backButton);
    else if (button == downButton)
        return digitalRead(downButton);
    else if (button == upButton)
        return digitalRead(upButton);
    else if (button == okayButton)
        return digitalRead(okayButton);
    else
        return digitalRead(button);
}

bool LF_Alpha_Buttons ::end(void)
{
    gpio_reset_pin(gpio_num_t(backButton));
    gpio_reset_pin(gpio_num_t(downButton));
    gpio_reset_pin(gpio_num_t(upButton));
    gpio_reset_pin(gpio_num_t(okayButton));
    return true;
}

// HCSR04 Ultrasonic Sensor class implementation
bool LF_Alpha_HCSR04::begin(void)
{
    pinMode(trig_HCSR04, OUTPUT);
    pinMode(echo_HCSR04, INPUT);
    return true;
}

long LF_Alpha_HCSR04::readCM(void)
{
    digitalWrite(trig_HCSR04, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_HCSR04, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_HCSR04, LOW);
    long duration = pulseIn(echo_HCSR04, HIGH);
    long distance = (duration / 2) / 29.1; // Convert to cm
    return distance;
}

long LF_Alpha_HCSR04::readINCH(void)
{
    digitalWrite(trig_HCSR04, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_HCSR04, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_HCSR04, LOW);
    long duration = pulseIn(echo_HCSR04, HIGH);
    long distance = (duration / 2) / 74; // Convert to inches
    return distance;
}

bool LF_Alpha_HCSR04::end(void)
{
    gpio_reset_pin(gpio_num_t(trig_HCSR04));
    gpio_reset_pin(gpio_num_t(echo_HCSR04));
    return true;
}

// LF_Alpha_resPot class implementation
bool LF_Alpha_resPot::begin(void)
{
    pinMode(potPin, INPUT);
    return true;
}

int16_t LF_Alpha_resPot::readAnalog(void)
{
    return analogRead(potPin);
}

uint8_t LF_Alpha_resPot::readPercentage(void)
{
    int16_t analogValue = readAnalog();
    int16_t maxValue = (1 << getAdcResolution()) - 1; // Calculate max value based on resolution
    return (analogValue * 100) / maxValue;            // Convert to percentage
}

int8_t LF_Alpha_resPot::readVoltage(void)
{
    int16_t analogValue = readAnalog();
    int16_t maxValue = (1 << getAdcResolution()) - 1; // Calculate max value based on resolution
    return (analogValue * 5) / maxValue;              // Convert to voltage (0-5V)
}

void LF_Alpha_resPot ::setAdcResolution(uint8_t res)
{
    configured_ADC_bits = res;
    analogReadResolution(configured_ADC_bits);
}

uint8_t LF_Alpha_resPot ::getAdcResolution()
{
    return configured_ADC_bits;
}

bool LF_Alpha_resPot::end(void)
{
    gpio_reset_pin(gpio_num_t(potPin));
    return true;
}

// rgbLEDs Class implimetation here (WS2812B)
static uint8_t gamma8(uint8_t x)
{
    return pgm_read_byte(&_NeoPixelGammaTable[x]); // 0-255 in, 0-255 out
}

#if defined(ESP32)

#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)

void LF_Alpha_rgbLEDs::espShow()
{
    if (!rmtItems || !pixels || numLEDs == 0)
    {
        return;
    }

#ifdef NEO_KHZ400
    bool is800 = is800KHz;
#else
    bool is800 = true;
#endif

    int itemIdx = 0;
    uint8_t bytesPerPixel = (wOffset == rOffset) ? 3 : 4;
    uint32_t totalBytes = numLEDs * bytesPerPixel;

    // Build RMT items
    for (uint16_t i = 0; i < numLEDs; i++)
    {
        uint8_t *pixelPtr = &pixels[i * bytesPerPixel];

        for (uint8_t byteNum = 0; byteNum < bytesPerPixel; byteNum++)
        {
            uint8_t pixelByte = pixelPtr[byteNum];

            for (int bit = 7; bit >= 0; bit--)
            {
                if (pixelByte & (1 << bit))
                {
                    rmtItems[itemIdx].level0 = 1;
                    rmtItems[itemIdx].duration0 = is800 ? 8 : 16;
                    rmtItems[itemIdx].level1 = 0;
                    rmtItems[itemIdx].duration1 = is800 ? 4 : 8;
                }
                else
                {
                    rmtItems[itemIdx].level0 = 1;
                    rmtItems[itemIdx].duration0 = is800 ? 4 : 8;
                    rmtItems[itemIdx].level1 = 0;
                    rmtItems[itemIdx].duration1 = is800 ? 8 : 16;
                }
                itemIdx++;
            }
        }
    }

    rmtWrite(pin, rmtItems, totalBytes * 8, RMT_WAIT_FOR_EVER);
}

void LF_Alpha_rgbLEDs::espInit()
{
    // Deinit if already initialized to free the channel
    rmtDeinit(pin);

    // Initialize RMT TX mode on the specified pin with 1 memory block and 10MHz frequency
    if (!rmtInit(pin, RMT_TX_MODE, RMT_MEM_NUM_BLOCKS_1, 10000000))
    {
        return;
    }

    // Calculate items needed
    uint8_t bytesPerPixel = (wOffset == rOffset) ? 3 : 4;
    int itemsNeeded = numLEDs * bytesPerPixel * 8;

    // Allocate RMT items using standard malloc
    rmtItems = (rmt_data_t *)malloc(itemsNeeded * sizeof(rmt_data_t));
}

#else // ESP_IDF_VERSION < 5.0.0

void LF_Alpha_rgbLEDs::espShow()
{

    if (!rmtItems || !pixels || numLEDs == 0)
    {
        return;
    }

    // Timing values optimized for ESP32-S3 at 80MHz
    const uint32_t T0H = 26; // ~0.32µs high for '0'
    const uint32_t T0L = 66; // ~0.83µs low for '0'
    const uint32_t T1H = 52; // ~0.65µs high for '1'
    const uint32_t T1L = 40; // ~0.50µs low for '1'

    int itemIdx = 0;
    uint8_t bytesPerPixel = (wOffset == rOffset) ? 3 : 4;

    // Build RMT items
    for (uint16_t i = 0; i < numLEDs; i++)
    {
        uint8_t *pixelPtr = &pixels[i * bytesPerPixel];

        // Process each byte in GRB order for WS2812B
        for (uint8_t byteNum = 0; byteNum < bytesPerPixel; byteNum++)
        {
            uint8_t pixelByte = pixelPtr[byteNum];

            // Process each bit (MSB first)
            for (int bit = 7; bit >= 0; bit--)
            {
                bool bitValue = (pixelByte >> bit) & 1;

                rmtItems[itemIdx].level0 = 1;
                rmtItems[itemIdx].duration0 = bitValue ? T1H : T0H;
                rmtItems[itemIdx].level1 = 0;
                rmtItems[itemIdx].duration1 = bitValue ? T1L : T0L;
                itemIdx++;
            }
        }
    }

    // Add reset/latch pulse (50µs low)
    rmtItems[itemIdx].level0 = 0;
    rmtItems[itemIdx].duration0 = 4000; // 50µs at 80MHz
    rmtItems[itemIdx].level1 = 0;
    rmtItems[itemIdx].duration1 = 0;
    itemIdx++;

    // CRITICAL FIX: Use blocking transmission with proper error handling
    esp_err_t err = rmt_write_items(rmtChannel, rmtItems, itemIdx, true); // BLOCKING
    if (err != ESP_OK)
    {
        return;
    }
}

void LF_Alpha_rgbLEDs::espInit()
{

    rmt_config_t config;
    config.rmt_mode = RMT_MODE_TX;
    config.channel = rmtChannel;       // Use a valid channel (0-7)
    config.gpio_num = (gpio_num_t)pin; // Set the data pin
    config.mem_block_num = 1;          // Use 1 memory block
    config.clk_div = 1;                // 80MHz RMT clock for WS2812/NeoPixel
    config.tx_config.loop_en = false;
    config.tx_config.carrier_en = false;
    config.tx_config.idle_output_en = true;
    config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    config.tx_config.carrier_freq_hz = 0;
    config.tx_config.carrier_duty_percent = 50;
    config.tx_config.carrier_level = RMT_CARRIER_LEVEL_LOW;

    esp_err_t err = rmt_config(&config);
    if (err != ESP_OK)
    {
        return;
    }
    err = rmt_driver_install(rmtChannel, 0, 0);
    if (err != ESP_OK)
    {
        return;
    }

    // Calculate items needed
    uint8_t bytesPerPixel = (wOffset == rOffset) ? 3 : 4;
    int itemsNeeded = (numLEDs * bytesPerPixel * 8) + 2;

    // Allocate RMT items
    rmtItems = (rmt_item32_t *)heap_caps_malloc(itemsNeeded * sizeof(rmt_item32_t), MALLOC_CAP_DMA);

    if (!rmtItems)
    {
        rmt_driver_uninstall(rmtChannel);
        return;
    }
}

#endif // ESP_IDF_VERSION >= 5.0.0

#endif // defined(ESP32)

LF_Alpha_rgbLEDs::LF_Alpha_rgbLEDs(uint16_t n, int16_t pin, neoPixelType type)
    : begun(false), brightness(0), pixels(NULL), endTime(0), rmtItems(nullptr)
{
    pinMode(userLED2, OUTPUT);
    digitalWrite(userLED2, LOW);

    if (i2cInitFlag == false)
    {
        i2cInitFlag = true;
        Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL);
    }

    // CRITICAL: Initialize these in correct order
    updateType(type); // Set up offsets first
    updateLength(n);  // This allocates pixels array and sets numBytes
    setPin(pin);      // Set the pin

#if !defined(ESP_IDF_VERSION) || ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
    // Set default values
    rmtChannel = RMT_CHANNEL_0;
#endif
}

void LF_Alpha_rgbLEDs ::setPin(int16_t p)
{
    if (begun && (pin >= 0))
        pinMode(pin, INPUT); // Disable existing out pin
    pin = p;
    if (begun)
    {
        pinMode(p, OUTPUT);
        digitalWrite(p, LOW);
    }
#if defined(__AVR__)
    port = portOutputRegister(digitalPinToPort(p));
    pinMask = digitalPinToBitMask(p);
#endif
#if defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_ARDUINO_CORE_STM32)
    gpioPort = digitalPinToPort(p);
    gpioPin = STM_LL_GPIO_PIN(digitalPinToPinName(p));
#elif defined(_PY32_DEF_)
    gpioPort = digitalPinToPort(p);
    gpioPin = PY32_LL_GPIO_PIN(digitalPinToPinName(p));
#elif defined(ARDUINO_ARCH_CH32)
    PinName const pin_name = digitalPinToPinName(pin);
    gpioPort = get_GPIO_Port(CH_PORT(pin_name));
    gpioPin = CH_GPIO_PIN(pin_name);
#if defined(CH32V20x_D6)
    if (gpioPort == GPIOC && ((*(volatile uint32_t *)0x40022030) & 0x0F000000) == 0)
    {
        gpioPin = gpioPin >> 13;
    }
#endif
#endif
#if defined(TARGET_GIGA) || defined(TARGET_M4)
    gpio = digitalPinToGpio(pin);
    if (gpio == NULL)
    {
        gpio = new mbed::DigitalInOut(digitalPinToPinName(pin), PIN_OUTPUT, PullNone, 0);
        digitalPinToGpio(pin) = gpio;
    }
#endif
}

uint32_t LF_Alpha_rgbLEDs ::gamma32(uint32_t x)
{
    uint8_t *y = (uint8_t *)&x;
    // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
    // to avoid a bunch of shifting and masking that would be necessary for
    // properly handling different endianisms (and each byte is a fairly
    // trivial operation, so it might not even be wasting cycles vs a check
    // and branch for the RGB case). In theory this might cause trouble *if*
    // someone's storing information in the unused most significant byte
    // of an RGB value, but this seems exceedingly rare and if it's
    // encountered in reality they can mask values going in or coming out.
    for (uint8_t i = 0; i < 4; i++)
        y[i] = gamma8(y[i]);
    return x; // Packed 32-bit return
}

bool LF_Alpha_rgbLEDs::canShow(void)
{
    if (endTime == 0)
        return true; // First call, always allow

    uint32_t now = micros();

    // Handle counter rollover (every ~70 minutes)
    if (endTime > now)
    {
        endTime = now;
        return true;
    }

    return (now - endTime) >= 300L;
}

uint32_t LF_Alpha_rgbLEDs ::ColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
{

    uint8_t r, g, b;

    // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
    // 0 is not the start of pure red, but the midpoint...a few values above
    // zero and a few below 65536 all yield pure red (similarly, 32768 is the
    // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
    // each for red, green, blue) really only allows for 1530 distinct hues
    // (not 1536, more on that below), but the full unsigned 16-bit type was
    // chosen for hue so that one's code can easily handle a contiguous color
    // wheel by allowing hue to roll over in either direction.
    hue = (hue * 1530L + 32768) / 65536;
    // Because red is centered on the rollover point (the +32768 above,
    // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
    // where 0 and 1530 would yield the same thing. Rather than apply a
    // costly modulo operator, 1530 is handled as a special case below.

    // So you'd think that the color "hexcone" (the thing that ramps from
    // pure red, to pure yellow, to pure green and so forth back to red,
    // yielding six slices), and with each color component having 256
    // possible values (0-255), might have 1536 possible items (6*256),
    // but in reality there's 1530. This is because the last element in
    // each 256-element slice is equal to the first element of the next
    // slice, and keeping those in there this would create small
    // discontinuities in the color wheel. So the last element of each
    // slice is dropped...we regard only elements 0-254, with item 255
    // being picked up as element 0 of the next slice. Like this:
    // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
    // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
    // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
    // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
    // the constants below are not the multiples of 256 you might expect.

    // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
    if (hue < 510)
    { // Red to Green-1
        b = 0;
        if (hue < 255)
        { //   Red to Yellow-1
            r = 255;
            g = hue; //     g = 0 to 254
        }
        else
        {                  //   Yellow to Green-1
            r = 510 - hue; //     r = 255 to 1
            g = 255;
        }
    }
    else if (hue < 1020)
    { // Green to Blue-1
        r = 0;
        if (hue < 765)
        { //   Green to Cyan-1
            g = 255;
            b = hue - 510; //     b = 0 to 254
        }
        else
        {                   //   Cyan to Blue-1
            g = 1020 - hue; //     g = 255 to 1
            b = 255;
        }
    }
    else if (hue < 1530)
    { // Blue to Red-1
        g = 0;
        if (hue < 1275)
        {                   //   Blue to Magenta-1
            r = hue - 1020; //     r = 0 to 254
            b = 255;
        }
        else
        { //   Magenta to Red-1
            r = 255;
            b = 1530 - hue; //     b = 255 to 1
        }
    }
    else
    { // Last 0.5 Red (quicker than % operator)
        r = 255;
        g = b = 0;
    }

    // Apply saturation and value to R,G,B, pack into 32-bit result:
    uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
    uint16_t s1 = 1 + sat;  // 1 to 256; same reason
    uint8_t s2 = 255 - sat; // 255 to 0
    return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
           (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
           (((((b * s1) >> 8) + s2) * v1) >> 8);
}

bool LF_Alpha_rgbLEDs::begin()
{

    if (pin < 0)
    {
        return false;
    }

    // Ensure pixels array is allocated
    if (!pixels || numBytes == 0)
    {
        return false;
    }

    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    // Clean up any existing RMT setup
    if (rmtItems)
    {
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
        free(rmtItems);
#else
        heap_caps_free(rmtItems);
#endif
        rmtItems = nullptr;
    }

    // Initialize RMT
    espInit();

    if (!rmtItems)
    {
        return false;
    }

    begun = true;
    return true;
}

void LF_Alpha_rgbLEDs::show()
{

    if (!pixels || numLEDs == 0 || !begun)
    {
        return;
    }

    // Simple timing - NO DELAY
    if (endTime > 0)
    {
        uint32_t elapsed = micros() - endTime;
    }

    // CRITICAL FIX: DO NOT disable interrupts for RMT
    // The ESP32 RMT hardware handles timing automatically
    espShow();

    endTime = micros();
}

void LF_Alpha_rgbLEDs::write(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    if (n >= numLEDs || !pixels)
    {
        return;
    }

    if (brightness)
    { // See notes in setBrightness()
        r = (r * brightness) >> 8;
        g = (g * brightness) >> 8;
        b = (b * brightness) >> 8;
    }

    uint8_t *p;
    if (wOffset == rOffset)
    {                       // Is an RGB-type strip
        p = &pixels[n * 3]; // 3 bytes per pixel
    }
    else
    {                       // Is a WRGB-type strip
        p = &pixels[n * 4]; // 4 bytes per pixel
        p[wOffset] = 0;     // But only R,G,B passed -- set W to 0
    }
    p[rOffset] = r; // R,G,B always stored
    p[gOffset] = g;
    p[bOffset] = b;
}

//  @param   w  White brightness, 0 = minimum (off), 255 = maximum
void LF_Alpha_rgbLEDs ::write(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    if (n < numLEDs)
    {
        if (brightness)
        { // See notes in setBrightness()
            r = (r * brightness) >> 8;
            g = (g * brightness) >> 8;
            b = (b * brightness) >> 8;
            w = (w * brightness) >> 8;
        }
        uint8_t *p;
        if (wOffset == rOffset)
        {                       // Is an RGB-type strip
            p = &pixels[n * 3]; // 3 bytes per pixel (ignore W)
        }
        else
        {                       // Is a WRGB-type strip
            p = &pixels[n * 4]; // 4 bytes per pixel
            p[wOffset] = w;     // Store W
        }
        p[rOffset] = r; // Store R,G,B
        p[gOffset] = g;
        p[bOffset] = b;
    }
}

void LF_Alpha_rgbLEDs ::write(uint16_t n, uint32_t c)
{
    if (n < numLEDs)
    {
        uint8_t *p, r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
        if (brightness)
        { // See notes in setBrightness()
            r = (r * brightness) >> 8;
            g = (g * brightness) >> 8;
            b = (b * brightness) >> 8;
        }
        if (wOffset == rOffset)
        {
            p = &pixels[n * 3];
        }
        else
        {
            p = &pixels[n * 4];
            uint8_t w = (uint8_t)(c >> 24);
            p[wOffset] = brightness ? ((w * brightness) >> 8) : w;
        }
        p[rOffset] = r;
        p[gOffset] = g;
        p[bOffset] = b;
    }
}

void LF_Alpha_rgbLEDs ::setBrightness(uint8_t b)
{
    // Store brightness as b+1: 0=max, 1=off, 255=near max.
    uint8_t newBrightness = b + 1;
    if (newBrightness != brightness)
    { // Scale RAM data if changed (lossy on increase).
        uint8_t c, *ptr = pixels, oldBrightness = brightness - 1;
        uint16_t scale = oldBrightness ? (b == 255 ? 65535 / oldBrightness : (((uint16_t)newBrightness << 8) - 1) / oldBrightness) : 0;
        for (uint16_t i = 0; i < numBytes; i++)
        {
            c = *ptr;
            *ptr++ = (c * scale) >> 8;
        }
        brightness = newBrightness;
    }
}

void LF_Alpha_rgbLEDs::clear()
{
    if (pixels && numBytes > 0)
    {
        memset(pixels, 0, numBytes);
    }
    // Don't call show() here - let user decide when to update
}

void LF_Alpha_rgbLEDs::updateLength(uint16_t n)
{
    if (pixels)
    {
        free(pixels); // Free existing data if any
        pixels = nullptr;
    }

    // Calculate bytes per pixel
    uint8_t bytesPerPixel = (wOffset == rOffset) ? 3 : 4;
    numBytes = n * bytesPerPixel;

    if (numBytes > 0)
    {
        pixels = (uint8_t *)malloc(numBytes);
        if (pixels)
        {
            memset(pixels, 0, numBytes); // Clear the allocated memory
            numLEDs = n;
        }
        else
        {
            numLEDs = 0;
            numBytes = 0;
        }
    }
    else
    {
        numLEDs = 0;
        numBytes = 0;
    }
}

void LF_Alpha_rgbLEDs ::updateType(neoPixelType t)
{
    bool oldThreeBytesPerPixel = (wOffset == rOffset); // false if RGBW

    wOffset = (t >> 6) & 0b11; // See notes in header file
    rOffset = (t >> 4) & 0b11; // regarding R/G/B/W offsets
    gOffset = (t >> 2) & 0b11;
    bOffset = t & 0b11;
#if defined(NEO_KHZ400)
    is800KHz = (t < 256); // 400 KHz flag is 1<<8
#endif

    // If bytes-per-pixel has changed (and pixel data was previously
    // allocated), re-allocate to new size. Will clear any data.
    if (pixels)
    {
        bool newThreeBytesPerPixel = (wOffset == rOffset);
        if (newThreeBytesPerPixel != oldThreeBytesPerPixel)
            updateLength(numLEDs);
    }
}

uint32_t LF_Alpha_rgbLEDs ::getPixelColor(uint16_t n) const
{
    if (n >= numLEDs)
        return 0; // Out of bounds, return no color.

    uint8_t *p;

    if (wOffset == rOffset)
    { // Is RGB-type device
        p = &pixels[n * 3];
        if (brightness)
        {
            // Stored color was decimated by setBrightness(). Returned value
            // attempts to scale back to an approximation of the original 24-bit
            // value used when setting the pixel color, but there will always be
            // some error -- those bits are simply gone. Issue is most
            // pronounced at low brightness levels.
            return (((uint32_t)(p[rOffset] << 8) / brightness) << 16) |
                   (((uint32_t)(p[gOffset] << 8) / brightness) << 8) |
                   ((uint32_t)(p[bOffset] << 8) / brightness);
        }
        else
        {
            // No brightness adjustment has been made -- return 'raw' color
            return ((uint32_t)p[rOffset] << 16) | ((uint32_t)p[gOffset] << 8) |
                   (uint32_t)p[bOffset];
        }
    }
    else
    { // Is RGBW-type device
        p = &pixels[n * 4];
        if (brightness)
        { // Return scaled color
            return (((uint32_t)(p[wOffset] << 8) / brightness) << 24) |
                   (((uint32_t)(p[rOffset] << 8) / brightness) << 16) |
                   (((uint32_t)(p[gOffset] << 8) / brightness) << 8) |
                   ((uint32_t)(p[bOffset] << 8) / brightness);
        }
        else
        { // Return raw color
            return ((uint32_t)p[wOffset] << 24) | ((uint32_t)p[rOffset] << 16) |
                   ((uint32_t)p[gOffset] << 8) | (uint32_t)p[bOffset];
        }
    }
}

void LF_Alpha_rgbLEDs ::rainbow(uint16_t first_hue, int8_t reps, uint8_t saturation, uint8_t brightness, bool gammify)
{
    for (uint16_t i = 0; i < numLEDs; i++)
    {
        uint16_t hue = first_hue + (i * reps * 65536) / numLEDs;
        uint32_t color = ColorHSV(hue, saturation, brightness);
        if (gammify)
            color = gamma32(color);
        write(i, color);
    }
}

void LF_Alpha_rgbLEDs ::blink(uint8_t noOfLED, uint8_t re, uint8_t gr, uint8_t bl, uint16_t duration)
{
    write(noOfLED, re, gr, bl);
    show();
    delay(duration);

    write(noOfLED, 0, 0, 0);
    show();
    delay(duration);
}

bool LF_Alpha_rgbLEDs::end()
{
#ifdef ESP32
    if (rmtItems)
    {
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
        free(rmtItems);
#else
        heap_caps_free(rmtItems); // Use heap_caps_free instead of free
#endif
        rmtItems = nullptr;
    }
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    rmtDeinit(pin);
#else
    // Release RMT driver
    rmt_driver_uninstall(rmtChannel);
#endif
#endif

    if (pixels)
    {
        free(pixels);
        pixels = nullptr;
    }

    if (pin >= 0)
    {
        pinMode(pin, INPUT);
        begun = false;
    }

    numLEDs = 0;
    numBytes = 0;

    return true;
}

// LF_Alpha_Relays class implementation here..
bool LF_Alpha_Relays ::begin(uint8_t pin)
{
    if (pin == RLY1)
    {
        pinMode(RLY1, OUTPUT);
        return true;
    }
    else if (pin == RLY2)
    {
        pinMode(RLY2, OUTPUT);
        return true;
    }
    else
    {
        pinMode(pin, OUTPUT);
        return true;
    }
}

bool LF_Alpha_Relays ::begin(uint8_t pin1, uint8_t pin2)
{
    if (pin1 == RLY1 && pin2 == RLY2)
    {
        pinMode(RLY1, OUTPUT);
        pinMode(RLY2, OUTPUT);
        return true;
    }
    else if (pin1 == RLY2 && pin2 == RLY1)
    {
        pinMode(RLY2, OUTPUT);
        pinMode(RLY1, OUTPUT);
        return true;
    }
    else
    {
        pinMode(pin1, OUTPUT);
        pinMode(pin2, OUTPUT);
        return true;
    }
}

void LF_Alpha_Relays ::writeRLY1(bool state)
{
    digitalWrite(RLY1, state);
    return;
}

void LF_Alpha_Relays ::writeRLY2(bool state)
{
    digitalWrite(RLY2, state);
    return;
}

void LF_Alpha_Relays ::write(int relay, bool state)
{
    digitalWrite(relay, state);
}

bool LF_Alpha_Relays ::end(void)
{
    gpio_reset_pin(gpio_num_t(RLY1));
    gpio_reset_pin(gpio_num_t(RLY2));
    return true;
}

// LF_Alpha_Servo class implementation
LF_Alpha_Servo::LF_Alpha_Servo()
{
    pinMode(userLED2, OUTPUT);
    digitalWrite(userLED2, LOW);
    delay(100);

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    // initialize this channel with plausible values, except pin # (we set pin # when attached)
    REFRESH_CPS = 50;
    this->ticks = DEFAULT_PULSE_WIDTH_TICKS;
    this->timer_width = DEFAULT_TIMER_WIDTH;
    this->pinNumber = -1; // make it clear that we haven't attached a pin to this channel
    this->min = DEFAULT_uS_LOW;
    this->max = DEFAULT_uS_HIGH;
    this->timer_width_ticks = pow(2, this->timer_width);
    this->pwmChannel = -1;
}

void LF_Alpha_Servo::setPeriodHertz(int hertz)
{
    REFRESH_CPS = hertz;
}

bool LF_Alpha_Servo::begin(uint servoGPIO)
{

    // CRITICAL: Set PWM frequency BEFORE attaching
    this->setPeriodHertz(50); // 50Hz for standard servos

    if (servoGPIO == servoPin)
    {

        if (this->attach(servoGPIO, DEFAULT_uS_LOW, DEFAULT_uS_HIGH) != 0)
        { // Use DEFAULT_uS_LOW-DEFAULT_uS_HIGH range like working example
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (this->attach(servoGPIO, DEFAULT_uS_LOW, DEFAULT_uS_HIGH) != 0)
        {                                  
            this->writeMicroseconds((DEFAULT_uS_LOW + DEFAULT_uS_HIGH) / 2); // Center position
            return true;
        }
        else
        {
            return false;
        }
    }
}

void LF_Alpha_Servo::write(int value)
{
    // treat values less than MIN_PULSE_WIDTH (500) as angles in degrees
    // valid values in microseconds are handled as microseconds
    if (value < MIN_PULSE_WIDTH)
    {
        if (value < 0)
            value = 0;
        else if (value > 180)
            value = 180;

        value = map(value, 0, 180, this->min, this->max);
    }
    else
    {
    }

    this->writeMicroseconds(value);
}

int LF_Alpha_Servo::read()
{
    return (map(readMicroseconds(), this->min, this->max, 0, 180));
}

void LF_Alpha_Servo::sweep(uint32_t duration)
{
    if (!this->attached())
        return;

    uint32_t stepDelay = duration / 360; // Calculate delay for each degree step

    // Sweep from 0 to 180 degrees
    for (int angle = 0; angle <= 180; angle++)
    {
        this->write(angle);
        delay(stepDelay);
    }

    // Sweep back from 180 to 0 degrees
    for (int angle = 180; angle >= 0; angle--)
    {
        this->write(angle);
        delay(stepDelay);
    }
}

bool LF_Alpha_Servo::end()
{
    if (this->attached())
    {
        this->detach();
    }
    gpio_reset_pin(gpio_num_t(servoPin));
    return true;
}

// private methods
int LF_Alpha_Servo::attach(int pin)
{
    return this->attach(pin, DEFAULT_uS_LOW, DEFAULT_uS_HIGH);
}

int LF_Alpha_Servo::attach(int pin, int min, int max)
{

    if (pin < 0)
    {
        return 0;
    }

    // first check for new/reuse
    if (this->pinNumber <= 0)
    {
        this->ticks = DEFAULT_PULSE_WIDTH_TICKS;
        this->timer_width = DEFAULT_TIMER_WIDTH;
        this->timer_width_ticks = pow(2, this->timer_width);
    }

    this->pinNumber = pin;

    // min/max checks
    if (min < MIN_PULSE_WIDTH)
        min = MIN_PULSE_WIDTH;
    if (max > MAX_PULSE_WIDTH)
        max = MAX_PULSE_WIDTH;
    this->min = min;
    this->max = max;

    // Set up PWM channel directly using LEDC
#ifdef ARDUINO_ARCH_ESP32
    // Use LEDC channel 0 for simplicity
    this->pwmChannel = 0;
#if defined(ESP_ARDUINO_VERSION) && ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcAttach(this->pinNumber, REFRESH_CPS, this->timer_width);
#else
    ledcSetup(this->pwmChannel, REFRESH_CPS, this->timer_width);
    ledcAttachPin(this->pinNumber, this->pwmChannel);
#endif
    return this->pwmChannel + 1; // non-zero success
#else
    // For non-ESP32 platforms, use the ESP32PWM fallback
    pwm.attachPin(this->pinNumber, REFRESH_CPS, this->timer_width);
    this->pwmChannel = pwm.getChannel();

    if (this->pwmChannel >= 0)
    {
        return this->pwmChannel + 1;
    }
    else
    {
        return 0;
    }
#endif
}

void LF_Alpha_Servo::detach()
{
    if (this->attached())
    {
        // keep track of detached servos channels so we can reuse them if needed
        pwm.detachPin(this->pinNumber);

        this->pinNumber = -1;
    }
}

void LF_Alpha_Servo::writeMicroseconds(int value)
{
    writeTicks(usToTicks(value)); // convert to ticks
}

int LF_Alpha_Servo::readMicroseconds()
{
    int pulsewidthUsec;
    if (this->attached())
    {
        pulsewidthUsec = ticksToUs(this->ticks);
    }
    else
    {
        pulsewidthUsec = 0;
    }

    return (pulsewidthUsec);
}

bool LF_Alpha_Servo::attached()
{
    // Only check that a valid channel is stored
    bool isAttached = (this->pwmChannel >= 0);
    return isAttached;
}

void LF_Alpha_Servo::writeTicks(int value)
{

    if (!attached())
    {
        return;
    }

    // Clamp and convert value...
    this->ticks = value;

#ifdef ESP32
    // Use LEDC directly
#if defined(ESP_ARDUINO_VERSION) && ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcWrite(this->pinNumber, this->ticks);
#else
    ledcWrite(this->pwmChannel, this->ticks);
#endif
#else
    // Fallback
    pwm.write(this->ticks);
#endif
}

void LF_Alpha_Servo::setTimerWidth(int value)
{
    // only allow values between 10 and 14 for ESP32-C3
    // only allow values between 16 and 20 for other ESP32
    if (value < MINIMUM_TIMER_WIDTH)
        value = MINIMUM_TIMER_WIDTH;
    else if (value > MAXIMUM_TIMER_WIDTH)
        value = MAXIMUM_TIMER_WIDTH;

    // Fix the current ticks value after timer width change
    // The user can reset the tick value with a write() or writeUs()
    int widthDifference = this->timer_width - value;
    // if positive multiply by diff; if neg, divide
    if (widthDifference > 0)
    {
        this->ticks = widthDifference * this->ticks;
    }
    else if (widthDifference < 0)
    {
        this->ticks = this->ticks / -widthDifference;
    }

    this->timer_width = value;
    this->timer_width_ticks = pow(2, this->timer_width);

    // If this is an attached servo, clean up
    if (this->attached())
    {
        // detach, setup and attach again to reflect new timer width
        pwm.detachPin(this->pinNumber);
        pwm.attachPin(this->pinNumber, REFRESH_CPS, this->timer_width);
    }
}

int LF_Alpha_Servo::readTimerWidth()
{
    return (this->timer_width);
}

int LF_Alpha_Servo::usToTicks(int us)
{
    return (int)((double)us / ((double)REFRESH_USEC / (double)this->timer_width_ticks) * (((double)REFRESH_CPS) / 50.0));
}

int LF_Alpha_Servo::ticksToUs(int ticks)
{
    return (int)((double)ticks * ((double)REFRESH_USEC / (double)this->timer_width_ticks) / (((double)REFRESH_CPS) / 50.0));
}

// DateTime class implementation here
DateTime::DateTime(int16_t year, int8_t month, int8_t day, int8_t hour, int8_t min, int8_t sec)
{
    _tm.tm_year = year - 1900;
    _tm.tm_mon = month - 1;
    _tm.tm_mday = day;
    _tm.tm_hour = hour;
    _tm.tm_min = min;
    _tm.tm_sec = sec;
}

DateTime::DateTime()
{
    memset(&_tm, 0, sizeof(_tm));
}

// LF_Alpha_RTC class implementation here..

// private methods defined here
LF_Alpha_RTC::LF_Alpha_RTC() : _Wire(Wire)
{
    if (i2cInitFlag == false)
    {
        i2cInitFlag = true;
        Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL); // Initialize I2C for all instances
    }

    pinMode(userLED2, OUTPUT);
    digitalWrite(userLED2, LOW);
}

uint8_t LF_Alpha_RTC::decToBcd(uint8_t val) { return (val / 10 * 16) + (val % 10); }

uint8_t LF_Alpha_RTC::bcdToDec(uint8_t val) { return (val / 16 * 10) + (val % 16); }

byte LF_Alpha_RTC::readControlByte(bool which)
{
    // Read selected control byte
    // first byte (0) is 0x0e, second (1) is 0x0f
    _Wire.beginTransmission(_i2cAddr);
    if (which)
    {
        // second control byte
        _Wire.write(0x0f);
    }
    else
    {
        // first control byte
        _Wire.write(0x0e);
    }
    _Wire.endTransmission();
    _Wire.requestFrom(_i2cAddr, 1);
    return _Wire.read();
}

void LF_Alpha_RTC::writeControlByte(byte control, bool which)
{
    // Write the selected control byte.
    // which=false -> 0x0e, true->0x0f.
    _Wire.beginTransmission(_i2cAddr);
    if (which)
    {
        _Wire.write(0x0f);
    }
    else
    {
        _Wire.write(0x0e);
    }
    _Wire.write(control);
    _Wire.endTransmission();
}

void LF_Alpha_RTC::setTime12(uint8_t hour, uint8_t min, uint8_t sec, bool isPM)
{
    uint8_t h = hour % 12;
    if (h == 0)
        h = 12;
    uint8_t reg = decToBcd(h) | 0b01000000; // 12h mode
    if (isPM)
        reg |= 0b00100000;
    _Wire.beginTransmission(_i2cAddr);
    _Wire.write(0x00);
    _Wire.write(decToBcd(sec));
    _Wire.write(decToBcd(min));
    _Wire.write(reg);
    _Wire.endTransmission();
}

void LF_Alpha_RTC::setClockMode(bool h12)
{
    _Wire.beginTransmission(_i2cAddr);
    _Wire.write(0x02);
    _Wire.endTransmission();
    _Wire.requestFrom(_i2cAddr, 1);
    uint8_t hourReg = _Wire.read();
    if (h12)
        hourReg |= 0b01000000;
    else
        hourReg &= ~0b01000000;
    _Wire.beginTransmission(_i2cAddr);
    _Wire.write(0x02);
    _Wire.write(hourReg);
    _Wire.endTransmission();
}

bool LF_Alpha_RTC::getClockMode()
{
    _Wire.beginTransmission(_i2cAddr);
    _Wire.write(0x02);
    _Wire.endTransmission();
    _Wire.requestFrom(_i2cAddr, 1);
    uint8_t hourReg = _Wire.read();
    return (hourReg & 0b01000000) != 0;
}

DateTime LF_Alpha_RTC::now()
{
    _Wire.beginTransmission(_i2cAddr);
    _Wire.write(0x00);
    _Wire.endTransmission();
    _Wire.requestFrom(_i2cAddr, 7);

    uint8_t sec = bcdToDec(_Wire.read() & 0x7F);
    uint8_t min = bcdToDec(_Wire.read());
    uint8_t hourReg = _Wire.read();
    uint8_t hour, isPM = 0;
    if (hourReg & 0b01000000)
    { // 12h mode
        hour = bcdToDec(hourReg & 0b00011111);
        isPM = hourReg & 0b00100000;
        if (isPM)
            hour += 12;
    }
    else
    {
        hour = bcdToDec(hourReg & 0b00111111);
    }
    _Wire.read(); // skip day of week
    uint8_t day = bcdToDec(_Wire.read());
    uint8_t month = bcdToDec(_Wire.read());
    uint16_t year = bcdToDec(_Wire.read()) + 2000;

    return DateTime(year, month, day, hour, min, sec);
}

// public methods defined here
bool LF_Alpha_RTC::begin(uint clockAddress)
{
    this->_i2cAddr = clockAddress;
    if (i2cInitFlag == false)
    {
        i2cInitFlag = true;
        Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL); // Initialize I2C for all instances
    }
    // see if we can find the RTC
    _Wire.beginTransmission(_i2cAddr);
    if (_Wire.endTransmission() != 0)
    {
        ESP_LOGE(TAG, "Couldn't find RTC");
        return false;
    }
    // Clear OSF flag
    byte temp_buffer = readControlByte(1);
    writeControlByte((temp_buffer & 0b01111111), 1);
    return true;
}

void LF_Alpha_RTC::setDateTime(String date, String time)
{

    int16_t year;
    int8_t month, day, hour, min, sec;
    char monthStr[4];
    sscanf(date.c_str(), "%s %hhd %hd", monthStr, &day, &year);
    sscanf(time.c_str(), "%hhd:%hhd:%hhd", &hour, &min, &sec);

    if (strcmp(monthStr, "Jan") == 0)
        month = 1;
    else if (strcmp(monthStr, "Feb") == 0)
        month = 2;
    else if (strcmp(monthStr, "Mar") == 0)
        month = 3;
    else if (strcmp(monthStr, "Apr") == 0)
        month = 4;
    else if (strcmp(monthStr, "May") == 0)
        month = 5;
    else if (strcmp(monthStr, "Jun") == 0)
        month = 6;
    else if (strcmp(monthStr, "Jul") == 0)
        month = 7;
    else if (strcmp(monthStr, "Aug") == 0)
        month = 8;
    else if (strcmp(monthStr, "Sep") == 0)
        month = 9;
    else if (strcmp(monthStr, "Oct") == 0)
        month = 10;
    else if (strcmp(monthStr, "Nov") == 0)
        month = 11;
    else if (strcmp(monthStr, "Dec") == 0)
        month = 12;

    setDateTime(year, month, day, hour, min, sec);
}

void LF_Alpha_RTC::setDateTime(int16_t year, int8_t month, int8_t day, int8_t hour, int8_t min, int8_t sec)
{
    setClockMode(true); // Set to 12h mode (or false for 24h if you prefer)

    bool is12h = getClockMode(); // <-- Read clock mode BEFORE transmission

    _Wire.beginTransmission(_i2cAddr);
    _Wire.write(0x00); // start at location 0
    _Wire.write(decToBcd(sec));
    _Wire.write(decToBcd(min));

    if (is12h)
    {
        uint8_t h = hour % 12;
        if (h == 0)
            h = 12;
        uint8_t reg = decToBcd(h) | 0b01000000; // 12h mode
        if (hour >= 12)
            reg |= 0b00100000; // PM flag
        _Wire.write(reg);
    }
    else
    {
        _Wire.write(decToBcd(hour));
    }

    _Wire.write(decToBcd(1)); // day of week, not used here
    _Wire.write(decToBcd(day));
    _Wire.write(decToBcd(month));
    _Wire.write(decToBcd(year - 2000)); // year offset from 2000
    _Wire.endTransmission();
}

String LF_Alpha_RTC::readTime()
{
    // return times as HH:MM:SS in 24H format
    DateTime now = this->now();
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    return String(buffer);
}

String LF_Alpha_RTC::readDate()
{
    // return date as DD/MM/YYYY
    DateTime now = this->now();
    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d", now.day(), now.month(), now.year());
    return String(buffer);
}

uint8_t LF_Alpha_RTC::getDayNumber(void)
{
    DateTime now = this->now();
    return now.day();
}

String LF_Alpha_RTC::getDayName(void)
{
    DateTime now = this->now();
    const char *dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    // Calculate day of the week using Zeller's Congruence
    int q = now.day();
    int m = now.month();
    int Y = now.year();
    if (m < 3)
    {
        m += 12;
        Y -= 1;
    }
    int K = Y % 100;
    int J = Y / 100;
    int h = (q + (13 * (m + 1)) / 5 + K + (K / 4) + (J / 4) + (5 * J)) % 7;
    int dayOfWeek = ((h + 5) % 7); // Convert to 0=Sunday, ..., 6=Saturday
    return String(dayNames[dayOfWeek + 1]);
}

String LF_Alpha_RTC::getMonthName(void)
{
    DateTime now = this->now();
    const char *monthNames[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    return String(monthNames[now.month() - 1]);
}

uint8_t LF_Alpha_RTC::getMonthNumber()
{
    DateTime now = this->now();
    return now.month();
}

uint16_t LF_Alpha_RTC::getYear(void)
{
    DateTime now = this->now();
    return now.year();
}

bool LF_Alpha_RTC::isRunning(void)
{
    /**
     * Read Status Register (0x0F) and check Oscillator Stop Flag (OSF, bit 7)
     * OSF = 1 means oscillator has stopped (not running)
     * OSF = 0 means oscillator is running normally
     */
    
    _Wire.beginTransmission(_i2cAddr);
    _Wire.write(0x0F); // Status Register address
    if (_Wire.endTransmission() != 0)
    {
        Serial.println("Failed to write to DS3231 Status Register");
        return false;
    }

    // Request 1 byte from the Status Register
    if (_Wire.requestFrom(_i2cAddr, 1) != 1)
    {
        Serial.println("Failed to read DS3231 Status Register");
        return false;
    }

    uint8_t statusReg = _Wire.read();
    
    // Check bit 7 (OSF - Oscillator Stop Flag)
    // OSF = 1 means stopped (not running)
    // OSF = 0 means running
    bool oscillatorRunning = (statusReg & 0x80) == 0;

    if (!oscillatorRunning)
    {
        Serial.println("WARNING: DS3231 Oscillator Stop Flag set. RTC may have lost power.");
    }

    return oscillatorRunning;
}

bool LF_Alpha_RTC::isLeapYear(int16_t year)
{
    // return true if leap year
    if (year % 4 == 0)
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
                return true;
            else
                return false;
        }
        else
            return true;
    }
    else
        return false;
}

bool LF_Alpha_RTC::end()
{
    // change the value of _i2cAddr to an invalid address
    _i2cAddr = 0x00;
    return true;
}

// LF_Alpha_AHT20 methods defintions
// private methods
bool LF_Alpha_AHT20::getStatus(uint8_t bit)
{
    Wire.requestFrom(_deviceAddress, (uint8_t)1);
    if (Wire.available())
        return ((uint8_t)Wire.read() & (1 << bit));
    return false;
}

bool LF_Alpha_AHT20::initialize()
{
    Wire.beginTransmission(_deviceAddress);
    Wire.write(CMD_INITIALIZE);
    Wire.write((uint8_t)0x08);
    Wire.write((uint8_t)0x00);
    return (Wire.endTransmission() == 0);
}

bool LF_Alpha_AHT20::triggerMeasurement()
{
    Wire.beginTransmission(_deviceAddress);
    Wire.write(CMD_TRIGGER);
    Wire.write((uint8_t)0x33);
    Wire.write((uint8_t)0x00);
    return (Wire.endTransmission() == 0);
}

void LF_Alpha_AHT20::readData()
{
    _sensorData.temperature = 0;
    _sensorData.humidity = 0;
    if (Wire.requestFrom(_deviceAddress, (uint8_t)7) > 0)
    {
        Wire.read(); // status byte

        uint32_t incoming = 0;
        incoming = ((uint32_t)Wire.read() << 16);
        incoming |= ((uint32_t)Wire.read() << 8);
        uint8_t midByte = Wire.read();

        incoming |= midByte;
        _sensorData.humidity = incoming >> 4; // 20bits

        _sensorData.temperature = ((uint32_t)midByte << 16);
        _sensorData.temperature |= ((uint32_t)Wire.read() << 8);
        _sensorData.temperature |= (uint32_t)Wire.read();

        _sensorData.crc = Wire.read();
        // Need to get rid of data in bits > 20
        _sensorData.temperature = _sensorData.temperature & ~(0xFFF00000);

        // Mark data as fresh
        _sensorQueried.temperature = false;
        _sensorQueried.humidity = false;
    }
}

bool LF_Alpha_AHT20::softReset()
{
    Wire.beginTransmission(_deviceAddress);
    Wire.write(CMD_SOFTRESET);
    return (Wire.endTransmission() == 0);
}

// public methods defined here
bool LF_Alpha_AHT20::begin(const uint8_t deviceAddress)
{
    if (deviceAddress != DEFAULT_ADDRESS)
    {
        _deviceAddress = deviceAddress; // Set the device address if provided
    }

    if (i2cInitFlag == false)
    {
        i2cInitFlag = true;
        Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL); // Initialize I2C for all instances
    }
    Wire.beginTransmission(_deviceAddress);
    if (Wire.endTransmission() != 0)
    {
        // If IC failed to respond, give it 20ms more for Power On Startup
        // Datasheet pg 7
        delay(20);
        Wire.beginTransmission(_deviceAddress);
        if (Wire.endTransmission() != 0)
            return false;
    }

    // Wait 40 ms after power-on before reading temp or humidity. Datasheet pg 8
    while (millis() < 40)
        ;

    // Check if the calibrated bit is set. If not, init the sensor.
    if (!getStatus(BIT_CALIBRATED))
    {
        initialize();
        delay(10);
        triggerMeasurement();
        delay(80); // Wait for measurement to complete
        uint8_t counter = 0;
        while (getStatus(BIT_BUSY))
        {
            delay(1);
            if (counter++ > 100)
                return false;
        }
        if (!getStatus(BIT_CALIBRATED))
            return false;
    }

    _sensorQueried.temperature = true;
    _sensorQueried.humidity = true;

    return true;
}

float LF_Alpha_AHT20::readTempC()
{
    if (_sensorQueried.temperature)
    {
        triggerMeasurement();
        delay(80); // Wait for measurement to complete
        uint8_t counter = 0;
        while (getStatus(BIT_BUSY))
        {
            delay(1);
            if (counter++ > 100)
                return false;
        }
        readData();
    }

    // Mark data as old
    _sensorQueried.temperature = true;

    // From datasheet pg 8
    return ((float)_sensorData.temperature / 1048576) * 200 - 50;
}

float LF_Alpha_AHT20::readHumi()
{
    if (_sensorQueried.humidity)
    {
        triggerMeasurement();
        delay(80);
        uint8_t counter = 0;
        while (getStatus(BIT_BUSY))
        {
            delay(1);
            if (counter++ > 100)
                return false;
        }
        readData();
    }

    // Mark data as old
    _sensorQueried.humidity = true;
    // From datasheet pg 8
    return ((float)_sensorData.humidity / 1048576) * 100;
}

bool LF_Alpha_AHT20::end(void)
{
    softReset();
    _deviceAddress = 0x00; // Invalidate the device address
    return true;
}

// LF_Alpha_MPU6050 methods definitions

// constructor
LF_Alpha_MPU6050 ::LF_Alpha_MPU6050(uint8_t address, void *wireObj) : devAddr(address), wireObj(wireObj)
{
    pinMode(userLED2, OUTPUT);
    digitalWrite(userLED2, LOW);

    if (i2cInitFlag == false)
    {
        i2cInitFlag = true;
        Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL); // Initialize I2C for all instances
        delay(1000);
    }

    // Ensure calibration loop counter defaults to a safe value
    N = NFast;        // <-- add this: default smoothing sample count
    LinesOut = LinesBetweenHeaders;
    // initialize other calibration-related vars defensively
    i = 0;
    for (int k = 0; k < 6; ++k) {
        LowOffset[k] = 0;
        HighOffset[k] = 0;
        Smoothed[k] = 0;
        LowValue[k] = 0;
        HighValue[k] = 0;
        Target[k] = 0;
    }
}

// MPU6050 private methods
void LF_Alpha_MPU6050::initialize()
{
    setClockSource(MPU6050_CLOCK_PLL_XGYRO);

    setFullScaleGyroRange(MPU6050_GYRO_FS_250);

    setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

    setSleepEnabled(false); // thanks to Jack Elston for pointing this one out!
}

void LF_Alpha_MPU6050::initialize(ACCEL_FS accelRange, GYRO_FS gyroRange)
{
    setClockSource(MPU6050_CLOCK_PLL_XGYRO);

    switch (accelRange)
    {
    case ACCEL_FS::A2G:
        setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
        break;

    case ACCEL_FS::A4G:
        setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
        break;

    case ACCEL_FS::A8G:
        setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
        break;

    case ACCEL_FS::A16G:
        setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
        break;
    default:
        Serial.println("Init accelRange not valid, setting maximum accel range");
        setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
    }

    switch (gyroRange)
    {
    case GYRO_FS::G250DPS:
        setFullScaleGyroRange(MPU6050_GYRO_FS_250);
        break;

    case GYRO_FS::G500DPS:
        setFullScaleGyroRange(MPU6050_GYRO_FS_500);
        break;

    case GYRO_FS::G1000DPS:
        setFullScaleGyroRange(MPU6050_GYRO_FS_1000);
        break;

    case GYRO_FS::G2000DPS:
        setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
        break;
    default:
        Serial.println("Init gyroRange not valid, setting maximum gyro range");
        setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    }

    setSleepEnabled(false); // thanks to Jack Elston for pointing this one out!
}

/** Get the accelration resolution */
float LF_Alpha_MPU6050::get_acce_resolution()
{
    return accelerationResolution;
}

/** Get the gyroscope resolution.*/
float LF_Alpha_MPU6050::get_gyro_resolution()
{
    return gyroscopeResolution;
}

bool LF_Alpha_MPU6050::testConnection()
{
    uint8_t deviceId = getDeviceID();
    return (deviceId == 0x34) || (deviceId == 0xC) || (deviceId == 0x3A);
}

uint8_t LF_Alpha_MPU6050::getAuxVDDIOLevel()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_YG_OFFS_TC, MPU6050_TC_PWR_MODE_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050::setAuxVDDIOLevel(uint8_t level)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_YG_OFFS_TC, MPU6050_TC_PWR_MODE_BIT, level, wireObj);
}

uint8_t LF_Alpha_MPU6050::getRate()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_SMPLRT_DIV, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050::setRate(uint8_t rate)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_SMPLRT_DIV, rate, wireObj);
}

uint8_t LF_Alpha_MPU6050::getExternalFrameSync()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_EXT_SYNC_SET_BIT, MPU6050_CFG_EXT_SYNC_SET_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050::setExternalFrameSync(uint8_t sync)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_EXT_SYNC_SET_BIT, MPU6050_CFG_EXT_SYNC_SET_LENGTH, sync, wireObj);
}

uint8_t LF_Alpha_MPU6050::getDLPFMode()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050::setDLPFMode(uint8_t mode)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode, wireObj);
}

uint8_t LF_Alpha_MPU6050::getFullScaleGyroRange()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050::setFullScaleGyroRange(uint8_t range)
{

    switch (range)
    {
    case MPU6050_GYRO_FS_250:
        gyroscopeResolution = 250.0 / 32768.0;
        break;
    case MPU6050_GYRO_FS_500:
        gyroscopeResolution = 500.0 / 32768.0;
        break;
    case MPU6050_GYRO_FS_1000:
        gyroscopeResolution = 1000.0 / 32768.0;
        break;
    case MPU6050_GYRO_FS_2000:
        gyroscopeResolution = 2000.0 / 32768.0;
        break;
    default:
        Serial.println("Init gyroRange not valid, setting maximum gyro range");
        range = MPU6050_GYRO_FS_2000;
        gyroscopeResolution = 2000.0 / 32768.0;
        break;
    }

    I2Cdev::writeBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getAccelXSelfTestFactoryTrim()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_SELF_TEST_X, &buffer[0], I2Cdev::readTimeout, wireObj);
    I2Cdev::readByte(devAddr, MPU6050_RA_SELF_TEST_A, &buffer[1], I2Cdev::readTimeout, wireObj);
    return (buffer[0] >> 3) | ((buffer[1] >> 4) & 0x03);
}

uint8_t LF_Alpha_MPU6050 ::getAccelYSelfTestFactoryTrim()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_SELF_TEST_Y, &buffer[0], I2Cdev::readTimeout, wireObj);
    I2Cdev::readByte(devAddr, MPU6050_RA_SELF_TEST_A, &buffer[1], I2Cdev::readTimeout, wireObj);
    return (buffer[0] >> 3) | ((buffer[1] >> 2) & 0x03);
}

uint8_t LF_Alpha_MPU6050 ::getAccelZSelfTestFactoryTrim()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_SELF_TEST_Z, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (buffer[0] >> 3) | (buffer[1] & 0x03);
}

uint8_t LF_Alpha_MPU6050 ::getGyroXSelfTestFactoryTrim()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_SELF_TEST_X, buffer, I2Cdev::readTimeout, wireObj);
    return (buffer[0] & 0x1F);
}

uint8_t LF_Alpha_MPU6050 ::getGyroYSelfTestFactoryTrim()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_SELF_TEST_Y, buffer, I2Cdev::readTimeout, wireObj);
    return (buffer[0] & 0x1F);
}

uint8_t LF_Alpha_MPU6050 ::getGyroZSelfTestFactoryTrim()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_SELF_TEST_Z, buffer, I2Cdev::readTimeout, wireObj);
    return (buffer[0] & 0x1F);
}

bool LF_Alpha_MPU6050 ::getAccelXSelfTest()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_XA_ST_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setAccelXSelfTest(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_XA_ST_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getAccelYSelfTest()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_YA_ST_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setAccelYSelfTest(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_YA_ST_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getAccelZSelfTest()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_ZA_ST_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setAccelZSelfTest(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_ZA_ST_BIT, enabled, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getFullScaleAccelRange()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setFullScaleAccelRange(uint8_t range)
{

    switch (range)
    {
    case MPU6050_ACCEL_FS_2:
        accelerationResolution = 2.0 / 32768.0;
        break;
    case MPU6050_ACCEL_FS_4:
        accelerationResolution = 4.0 / 32768.0;
        break;
    case MPU6050_ACCEL_FS_8:
        accelerationResolution = 8.0 / 32768.0;
        break;
    case MPU6050_ACCEL_FS_16:
        accelerationResolution = 16.0 / 32768.0;
        break;
    default:
        Serial.println("Init accelRange not valid, setting maximum accel range");
        range = MPU6050_ACCEL_FS_16;
        accelerationResolution = 16.0 / 32768.0;
        break;
    }

    I2Cdev::writeBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getDHPFMode()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_ACCEL_HPF_BIT, MPU6050_ACONFIG_ACCEL_HPF_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setDHPFMode(uint8_t bandwidth)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_ACCEL_HPF_BIT, MPU6050_ACONFIG_ACCEL_HPF_LENGTH, bandwidth, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getFreefallDetectionThreshold()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_FF_THR, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setFreefallDetectionThreshold(uint8_t threshold)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_FF_THR, threshold, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getFreefallDetectionDuration()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_FF_DUR, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setFreefallDetectionDuration(uint8_t duration)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_FF_DUR, duration, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getMotionDetectionThreshold()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_MOT_THR, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setMotionDetectionThreshold(uint8_t threshold)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_MOT_THR, threshold, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getMotionDetectionDuration()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_MOT_DUR, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setMotionDetectionDuration(uint8_t duration)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_MOT_DUR, duration, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getZeroMotionDetectionThreshold()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_ZRMOT_THR, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setZeroMotionDetectionThreshold(uint8_t threshold)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_ZRMOT_THR, threshold, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getZeroMotionDetectionDuration()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_ZRMOT_DUR, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setZeroMotionDetectionDuration(uint8_t duration)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_ZRMOT_DUR, duration, wireObj);
}

bool LF_Alpha_MPU6050 ::getTempFIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_TEMP_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setTempFIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_TEMP_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getXGyroFIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_XG_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setXGyroFIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_XG_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getYGyroFIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_YG_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setYGyroFIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_YG_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getZGyroFIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_ZG_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setZGyroFIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_ZG_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getAccelFIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_ACCEL_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setAccelFIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_ACCEL_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlave2FIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_SLV2_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave2FIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_SLV2_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlave1FIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_SLV1_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave1FIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_SLV1_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlave0FIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_SLV0_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave0FIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_FIFO_EN, MPU6050_SLV0_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getMultiMasterEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_MULT_MST_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setMultiMasterEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_MULT_MST_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getWaitForExternalSensorEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_WAIT_FOR_ES_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setWaitForExternalSensorEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_WAIT_FOR_ES_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlave3FIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_SLV_3_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave3FIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_SLV_3_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlaveReadWriteTransitionEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_I2C_MST_P_NSR_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveReadWriteTransitionEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_I2C_MST_P_NSR_BIT, enabled, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getMasterClockSpeed()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_I2C_MST_CLK_BIT, MPU6050_I2C_MST_CLK_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setMasterClockSpeed(uint8_t speed)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_I2C_MST_CTRL, MPU6050_I2C_MST_CLK_BIT, MPU6050_I2C_MST_CLK_LENGTH, speed, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getSlaveAddress(uint8_t num)
{
    if (num > 3)
        return 0;
    I2Cdev::readByte(devAddr, MPU6050_RA_I2C_SLV0_ADDR + num * 3, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveAddress(uint8_t num, uint8_t address)
{
    if (num > 3)
        return;
    I2Cdev::writeByte(devAddr, MPU6050_RA_I2C_SLV0_ADDR + num * 3, address, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getSlaveRegister(uint8_t num)
{
    if (num > 3)
        return 0;
    I2Cdev::readByte(devAddr, MPU6050_RA_I2C_SLV0_REG + num * 3, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveRegister(uint8_t num, uint8_t reg)
{
    if (num > 3)
        return;
    I2Cdev::writeByte(devAddr, MPU6050_RA_I2C_SLV0_REG + num * 3, reg, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlaveEnabled(uint8_t num)
{
    if (num > 3)
        return 0;
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveEnabled(uint8_t num, bool enabled)
{
    if (num > 3)
        return;
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlaveWordByteSwap(uint8_t num)
{
    if (num > 3)
        return 0;
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_BYTE_SW_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveWordByteSwap(uint8_t num, bool enabled)
{
    if (num > 3)
        return;
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_BYTE_SW_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlaveWriteMode(uint8_t num)
{
    if (num > 3)
        return 0;
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_REG_DIS_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveWriteMode(uint8_t num, bool mode)
{
    if (num > 3)
        return;
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_REG_DIS_BIT, mode, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlaveWordGroupOffset(uint8_t num)
{
    if (num > 3)
        return 0;
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_GRP_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveWordGroupOffset(uint8_t num, bool enabled)
{
    if (num > 3)
        return;
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_GRP_BIT, enabled, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getSlaveDataLength(uint8_t num)
{
    if (num > 3)
        return 0;
    I2Cdev::readBits(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_LEN_BIT, MPU6050_I2C_SLV_LEN_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveDataLength(uint8_t num, uint8_t length)
{
    if (num > 3)
        return;
    I2Cdev::writeBits(devAddr, MPU6050_RA_I2C_SLV0_CTRL + num * 3, MPU6050_I2C_SLV_LEN_BIT, MPU6050_I2C_SLV_LEN_LENGTH, length, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getSlave4Address()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_I2C_SLV4_ADDR, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave4Address(uint8_t address)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_I2C_SLV4_ADDR, address, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getSlave4Register()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_I2C_SLV4_REG, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave4Register(uint8_t reg)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_I2C_SLV4_REG, reg, wireObj);
}

void LF_Alpha_MPU6050 ::setSlave4OutputByte(uint8_t data)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_I2C_SLV4_DO, data, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlave4Enabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_SLV4_CTRL, MPU6050_I2C_SLV4_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave4Enabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_SLV4_CTRL, MPU6050_I2C_SLV4_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlave4InterruptEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_SLV4_CTRL, MPU6050_I2C_SLV4_INT_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave4InterruptEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_SLV4_CTRL, MPU6050_I2C_SLV4_INT_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlave4WriteMode()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_SLV4_CTRL, MPU6050_I2C_SLV4_REG_DIS_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave4WriteMode(bool mode)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_SLV4_CTRL, MPU6050_I2C_SLV4_REG_DIS_BIT, mode, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getSlave4MasterDelay()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_I2C_SLV4_CTRL, MPU6050_I2C_SLV4_MST_DLY_BIT, MPU6050_I2C_SLV4_MST_DLY_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlave4MasterDelay(uint8_t delay)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_I2C_SLV4_CTRL, MPU6050_I2C_SLV4_MST_DLY_BIT, MPU6050_I2C_SLV4_MST_DLY_LENGTH, delay, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getSlate4InputByte()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_I2C_SLV4_DI, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getPassthroughStatus()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_STATUS, MPU6050_MST_PASS_THROUGH_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getSlave4IsDone()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_STATUS, MPU6050_MST_I2C_SLV4_DONE_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getLostArbitration()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_STATUS, MPU6050_MST_I2C_LOST_ARB_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getSlave4Nack()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_STATUS, MPU6050_MST_I2C_SLV4_NACK_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getSlave3Nack()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_STATUS, MPU6050_MST_I2C_SLV3_NACK_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getSlave2Nack()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_STATUS, MPU6050_MST_I2C_SLV2_NACK_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getSlave1Nack()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_STATUS, MPU6050_MST_I2C_SLV1_NACK_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getSlave0Nack()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_STATUS, MPU6050_MST_I2C_SLV0_NACK_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getInterruptMode()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setInterruptMode(bool mode)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, mode, wireObj);
}

bool LF_Alpha_MPU6050 ::getInterruptDrive()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_OPEN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setInterruptDrive(bool drive)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_OPEN_BIT, drive, wireObj);
}

bool LF_Alpha_MPU6050 ::getInterruptLatch()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_LATCH_INT_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setInterruptLatch(bool latch)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_LATCH_INT_EN_BIT, latch, wireObj);
}

bool LF_Alpha_MPU6050 ::getInterruptLatchClear()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_RD_CLEAR_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setInterruptLatchClear(bool clear)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_RD_CLEAR_BIT, clear, wireObj);
}

bool LF_Alpha_MPU6050 ::getFSyncInterruptLevel()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setFSyncInterruptLevel(bool level)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT, level, wireObj);
}

bool LF_Alpha_MPU6050 ::getFSyncInterruptEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_FSYNC_INT_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setFSyncInterruptEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_FSYNC_INT_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getI2CBypassEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setI2CBypassEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getClockOutputEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_CLKOUT_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setClockOutputEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_CLKOUT_EN_BIT, enabled, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getIntEnabled()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_INT_ENABLE, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setIntEnabled(uint8_t enabled)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_INT_ENABLE, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getIntFreefallEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_FF_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setIntFreefallEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_FF_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getIntMotionEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_MOT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setIntMotionEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_MOT_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getIntZeroMotionEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_ZMOT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setIntZeroMotionEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_ZMOT_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getIntFIFOBufferOverflowEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_FIFO_OFLOW_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setIntFIFOBufferOverflowEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_FIFO_OFLOW_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getIntI2CMasterEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_I2C_MST_INT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setIntI2CMasterEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_I2C_MST_INT_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getIntDataReadyEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DATA_RDY_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setIntDataReadyEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DATA_RDY_BIT, enabled, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getIntStatus()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_INT_STATUS, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getIntFreefallStatus()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_FF_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getIntMotionStatus()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_MOT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getIntZeroMotionStatus()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_ZMOT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getIntFIFOBufferOverflowStatus()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_FIFO_OFLOW_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getIntI2CMasterStatus()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_I2C_MST_INT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getIntDataReadyStatus()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_DATA_RDY_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::getMotion9(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz, int16_t *mx, int16_t *my, int16_t *mz)
{
    (void)mx; // unused parameter
    (void)my; // unused parameter
    (void)mz; // unused parameter

    getMotion6(ax, ay, az, gx, gy, gz);
    // TODO: magnetometer integration
}

void LF_Alpha_MPU6050 ::getMotion6(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz)
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, buffer, I2Cdev::readTimeout, wireObj);
    *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    *az = (((int16_t)buffer[4]) << 8) | buffer[5];
    *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}

void LF_Alpha_MPU6050 ::getAcceleration(int16_t *x, int16_t *y, int16_t *z)
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 6, buffer, I2Cdev::readTimeout, wireObj);
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

int16_t LF_Alpha_MPU6050 ::getAccelerationX()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t LF_Alpha_MPU6050 ::getAccelerationY()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_YOUT_H, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t LF_Alpha_MPU6050 ::getAccelerationZ()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_ZOUT_H, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t LF_Alpha_MPU6050 ::getTemperature()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_TEMP_OUT_H, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

void LF_Alpha_MPU6050 ::getRotation(int16_t *x, int16_t *y, int16_t *z)
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 6, buffer, I2Cdev::readTimeout, wireObj);
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

int16_t LF_Alpha_MPU6050 ::getRotationX()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t LF_Alpha_MPU6050 ::getRotationY()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_GYRO_YOUT_H, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t LF_Alpha_MPU6050 ::getRotationZ()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_GYRO_ZOUT_H, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

uint8_t LF_Alpha_MPU6050 ::getExternalSensorByte(int position)
{
    I2Cdev::readByte(devAddr, MPU6050_RA_EXT_SENS_DATA_00 + position, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

uint16_t LF_Alpha_MPU6050 ::getExternalSensorWord(int position)
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_EXT_SENS_DATA_00 + position, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((uint16_t)buffer[0]) << 8) | buffer[1];
}

uint32_t LF_Alpha_MPU6050 ::getExternalSensorDWord(int position)
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_EXT_SENS_DATA_00 + position, 4, buffer, I2Cdev::readTimeout, wireObj);
    return (((uint32_t)buffer[0]) << 24) | (((uint32_t)buffer[1]) << 16) | (((uint16_t)buffer[2]) << 8) | buffer[3];
}

uint8_t LF_Alpha_MPU6050 ::getMotionStatus()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_MOT_DETECT_STATUS, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getXNegMotionDetected()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_MOT_DETECT_STATUS, MPU6050_MOTION_MOT_XNEG_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getXPosMotionDetected()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_MOT_DETECT_STATUS, MPU6050_MOTION_MOT_XPOS_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getYNegMotionDetected()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_MOT_DETECT_STATUS, MPU6050_MOTION_MOT_YNEG_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getYPosMotionDetected()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_MOT_DETECT_STATUS, MPU6050_MOTION_MOT_YPOS_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getZNegMotionDetected()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_MOT_DETECT_STATUS, MPU6050_MOTION_MOT_ZNEG_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getZPosMotionDetected()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_MOT_DETECT_STATUS, MPU6050_MOTION_MOT_ZPOS_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getZeroMotionDetected()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_MOT_DETECT_STATUS, MPU6050_MOTION_MOT_ZRMOT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveOutputByte(uint8_t num, uint8_t data)
{
    if (num > 3)
        return;
    I2Cdev::writeByte(devAddr, MPU6050_RA_I2C_SLV0_DO + num, data, wireObj);
}

bool LF_Alpha_MPU6050 ::getExternalShadowDelayEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_DELAY_CTRL, MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setExternalShadowDelayEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_MST_DELAY_CTRL, MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getSlaveDelayEnabled(uint8_t num)
{
    // MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT is 4, SLV3 is 3, etc.
    if (num > 4)
        return 0;
    I2Cdev::readBit(devAddr, MPU6050_RA_I2C_MST_DELAY_CTRL, num, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSlaveDelayEnabled(uint8_t num, bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_I2C_MST_DELAY_CTRL, num, enabled, wireObj);
}

void LF_Alpha_MPU6050 ::resetGyroscopePath()
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_SIGNAL_PATH_RESET, MPU6050_PATHRESET_GYRO_RESET_BIT, true, wireObj);
}

void LF_Alpha_MPU6050 ::resetAccelerometerPath()
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_SIGNAL_PATH_RESET, MPU6050_PATHRESET_ACCEL_RESET_BIT, true, wireObj);
}

void LF_Alpha_MPU6050 ::resetTemperaturePath()
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_SIGNAL_PATH_RESET, MPU6050_PATHRESET_TEMP_RESET_BIT, true, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getAccelerometerPowerOnDelay()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_ACCEL_ON_DELAY_BIT, MPU6050_DETECT_ACCEL_ON_DELAY_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setAccelerometerPowerOnDelay(uint8_t delay)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_ACCEL_ON_DELAY_BIT, MPU6050_DETECT_ACCEL_ON_DELAY_LENGTH, delay, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getFreefallDetectionCounterDecrement()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_FF_COUNT_BIT, MPU6050_DETECT_FF_COUNT_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setFreefallDetectionCounterDecrement(uint8_t decrement)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_FF_COUNT_BIT, MPU6050_DETECT_FF_COUNT_LENGTH, decrement, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getMotionDetectionCounterDecrement()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_MOT_COUNT_BIT, MPU6050_DETECT_MOT_COUNT_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setMotionDetectionCounterDecrement(uint8_t decrement)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_MOT_DETECT_CTRL, MPU6050_DETECT_MOT_COUNT_BIT, MPU6050_DETECT_MOT_COUNT_LENGTH, decrement, wireObj);
}

bool LF_Alpha_MPU6050 ::getFIFOEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setFIFOEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_EN_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getI2CMasterModeEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setI2CMasterModeEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled, wireObj);
}

void LF_Alpha_MPU6050 ::switchSPIEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_IF_DIS_BIT, enabled, wireObj);
}

void LF_Alpha_MPU6050 ::resetFIFO()
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_RESET_BIT, true, wireObj);
}

void LF_Alpha_MPU6050 ::resetI2CMaster()
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_RESET_BIT, true, wireObj);
}

void LF_Alpha_MPU6050 ::resetSensors()
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_SIG_COND_RESET_BIT, true, wireObj);
}

void LF_Alpha_MPU6050 ::reset()
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, true, wireObj);
}

bool LF_Alpha_MPU6050 ::getSleepEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setSleepEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getWakeCycleEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CYCLE_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setWakeCycleEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CYCLE_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getTempSensorEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_TEMP_DIS_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0] == 0; // 1 is actually disabled here
}

void LF_Alpha_MPU6050 ::setTempSensorEnabled(bool enabled)
{
    // 1 is actually disabled here
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_TEMP_DIS_BIT, !enabled, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getClockSource()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setClockSource(uint8_t source)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getWakeFrequency()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_LP_WAKE_CTRL_BIT, MPU6050_PWR2_LP_WAKE_CTRL_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setWakeFrequency(uint8_t frequency)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_LP_WAKE_CTRL_BIT, MPU6050_PWR2_LP_WAKE_CTRL_LENGTH, frequency, wireObj);
}

bool LF_Alpha_MPU6050 ::getStandbyXAccelEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_XA_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setStandbyXAccelEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_XA_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getStandbyYAccelEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_YA_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setStandbyYAccelEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_YA_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getStandbyZAccelEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_ZA_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setStandbyZAccelEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_ZA_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getStandbyXGyroEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_XG_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setStandbyXGyroEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_XG_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getStandbyYGyroEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_YG_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setStandbyYGyroEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_YG_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getStandbyZGyroEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_ZG_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setStandbyZGyroEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_ZG_BIT, enabled, wireObj);
}

uint16_t LF_Alpha_MPU6050 ::getFIFOCount()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_FIFO_COUNTH, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((uint16_t)buffer[0]) << 8) | buffer[1];
}

uint8_t LF_Alpha_MPU6050 ::getFIFOByte()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_FIFO_R_W, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::getFIFOBytes(uint8_t *data, uint8_t length)
{
    if (length > 0)
    {
        I2Cdev::readBytes(devAddr, MPU6050_RA_FIFO_R_W, length, data, I2Cdev::readTimeout, wireObj);
    }
    else
    {
        *data = 0;
    }
}

uint32_t LF_Alpha_MPU6050 ::getFIFOTimeout()
{
    return fifoTimeout;
}

void LF_Alpha_MPU6050 ::setFIFOTimeout(uint32_t fifoTimeout)
{
    this->fifoTimeout = fifoTimeout;
}

int8_t LF_Alpha_MPU6050 ::GetCurrentFIFOPacket(uint8_t *data, uint8_t length)
{ // overflow proof
    int16_t fifoC;
    // This section of code is for when we allowed more than 1 packet to be acquired
    uint32_t BreakTimer = micros();
    bool packetReceived = false;
    do
    {
        if ((fifoC = getFIFOCount()) > length)
        {

            if (fifoC > 200)
            {                // if you waited to get the FIFO buffer to > 200 bytes it will take longer to get the last packet in the FIFO Buffer than it will take to  reset the buffer and wait for the next to arrive
                resetFIFO(); // Fixes any overflow corruption
                fifoC = 0;
                while (!(fifoC = getFIFOCount()) && ((micros() - BreakTimer) <= (getFIFOTimeout())))
                    ; // Get Next New Packet
            }
            else
            { // We have more than 1 packet but less than 200 bytes of data in the FIFO Buffer
                uint8_t Trash[I2CDEVLIB_WIRE_BUFFER_LENGTH];
                while ((fifoC = getFIFOCount()) > length)
                {                           // Test each time just in case the MPU is writing to the FIFO Buffer
                    fifoC = fifoC - length; // Save the last packet
                    uint16_t RemoveBytes;
                    while (fifoC)
                    {                                                                                                // fifo count will reach zero so this is safe
                        RemoveBytes = (fifoC < I2CDEVLIB_WIRE_BUFFER_LENGTH) ? fifoC : I2CDEVLIB_WIRE_BUFFER_LENGTH; // Buffer Length is different than the packet length this will efficiently clear the buffer
                        getFIFOBytes(Trash, (uint8_t)RemoveBytes);
                        fifoC -= RemoveBytes;
                    }
                }
            }
        }
        if (!fifoC)
            return 0; // Called too early no data or we timed out after FIFO Reset
        // We have 1 packet
        packetReceived = fifoC == length;
        if (!packetReceived && (micros() - BreakTimer) > (getFIFOTimeout()))
            return 0;
    } while (!packetReceived);
    getFIFOBytes(data, length); // Get 1 packet
    return 1;
}

void LF_Alpha_MPU6050 ::setFIFOByte(uint8_t data)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_FIFO_R_W, data, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getDeviceID()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setDeviceID(uint8_t id)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, id, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getOTPBankValid()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_XG_OFFS_TC, MPU6050_TC_OTP_BNK_VLD_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setOTPBankValid(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_XG_OFFS_TC, MPU6050_TC_OTP_BNK_VLD_BIT, enabled, wireObj);
}

int8_t LF_Alpha_MPU6050 ::getXGyroOffsetTC()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_XG_OFFS_TC, MPU6050_TC_OFFSET_BIT, MPU6050_TC_OFFSET_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setXGyroOffsetTC(int8_t offset)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_XG_OFFS_TC, MPU6050_TC_OFFSET_BIT, MPU6050_TC_OFFSET_LENGTH, offset, wireObj);
}

int8_t LF_Alpha_MPU6050 ::getYGyroOffsetTC()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_YG_OFFS_TC, MPU6050_TC_OFFSET_BIT, MPU6050_TC_OFFSET_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setYGyroOffsetTC(int8_t offset)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_YG_OFFS_TC, MPU6050_TC_OFFSET_BIT, MPU6050_TC_OFFSET_LENGTH, offset, wireObj);
}

int8_t LF_Alpha_MPU6050 ::getZGyroOffsetTC()
{
    I2Cdev::readBits(devAddr, MPU6050_RA_ZG_OFFS_TC, MPU6050_TC_OFFSET_BIT, MPU6050_TC_OFFSET_LENGTH, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setZGyroOffsetTC(int8_t offset)
{
    I2Cdev::writeBits(devAddr, MPU6050_RA_ZG_OFFS_TC, MPU6050_TC_OFFSET_BIT, MPU6050_TC_OFFSET_LENGTH, offset, wireObj);
}

int8_t LF_Alpha_MPU6050 ::getXFineGain()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_X_FINE_GAIN, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setXFineGain(int8_t gain)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_X_FINE_GAIN, gain, wireObj);
}

int8_t LF_Alpha_MPU6050 ::getYFineGain()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_Y_FINE_GAIN, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setYFineGain(int8_t gain)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_Y_FINE_GAIN, gain, wireObj);
}

int8_t LF_Alpha_MPU6050 ::getZFineGain()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_Z_FINE_GAIN, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setZFineGain(int8_t gain)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_Z_FINE_GAIN, gain, wireObj);
}

int16_t LF_Alpha_MPU6050 ::getXAccelOffset()
{
    uint8_t SaveAddress = ((getDeviceID() < 0x38) ? MPU6050_RA_XA_OFFS_H : 0x77); // MPU6050,MPU9150 Vs MPU6500,MPU9250
    I2Cdev::readBytes(devAddr, SaveAddress, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

void LF_Alpha_MPU6050 ::setXAccelOffset(int16_t offset)
{
    uint8_t SaveAddress = ((getDeviceID() < 0x38) ? MPU6050_RA_XA_OFFS_H : 0x77); // MPU6050,MPU9150 Vs MPU6500,MPU9250
    I2Cdev::writeWord(devAddr, SaveAddress, offset, wireObj);
}

int16_t LF_Alpha_MPU6050 ::getYAccelOffset()
{
    uint8_t SaveAddress = ((getDeviceID() < 0x38) ? MPU6050_RA_YA_OFFS_H : 0x7A); // MPU6050,MPU9150 Vs MPU6500,MPU9250
    I2Cdev::readBytes(devAddr, SaveAddress, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

void LF_Alpha_MPU6050 ::setYAccelOffset(int16_t offset)
{
    uint8_t SaveAddress = ((getDeviceID() < 0x38) ? MPU6050_RA_YA_OFFS_H : 0x7A); // MPU6050,MPU9150 Vs MPU6500,MPU9250
    I2Cdev::writeWord(devAddr, SaveAddress, offset, wireObj);
}

int16_t LF_Alpha_MPU6050 ::getZAccelOffset()
{
    uint8_t SaveAddress = ((getDeviceID() < 0x38) ? MPU6050_RA_ZA_OFFS_H : 0x7D); // MPU6050,MPU9150 Vs MPU6500,MPU9250
    I2Cdev::readBytes(devAddr, SaveAddress, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

void LF_Alpha_MPU6050 ::setZAccelOffset(int16_t offset)
{
    uint8_t SaveAddress = ((getDeviceID() < 0x38) ? MPU6050_RA_ZA_OFFS_H : 0x7D); // MPU6050,MPU9150 Vs MPU6500,MPU9250
    I2Cdev::writeWord(devAddr, SaveAddress, offset, wireObj);
}

int16_t LF_Alpha_MPU6050 ::getXGyroOffset()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_XG_OFFS_USRH, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

void LF_Alpha_MPU6050 ::setXGyroOffset(int16_t offset)
{
    I2Cdev::writeWord(devAddr, MPU6050_RA_XG_OFFS_USRH, offset, wireObj);
}

int16_t LF_Alpha_MPU6050 ::getYGyroOffset()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_YG_OFFS_USRH, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

void LF_Alpha_MPU6050 ::setYGyroOffset(int16_t offset)
{
    I2Cdev::writeWord(devAddr, MPU6050_RA_YG_OFFS_USRH, offset, wireObj);
}

int16_t LF_Alpha_MPU6050 ::getZGyroOffset()
{
    I2Cdev::readBytes(devAddr, MPU6050_RA_ZG_OFFS_USRH, 2, buffer, I2Cdev::readTimeout, wireObj);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

void LF_Alpha_MPU6050 ::setZGyroOffset(int16_t offset)
{
    I2Cdev::writeWord(devAddr, MPU6050_RA_ZG_OFFS_USRH, offset, wireObj);
}

bool LF_Alpha_MPU6050 ::getIntPLLReadyEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_PLL_RDY_INT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setIntPLLReadyEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_PLL_RDY_INT_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getIntDMPEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DMP_INT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setIntDMPEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DMP_INT_BIT, enabled, wireObj);
}

bool LF_Alpha_MPU6050 ::getDMPInt5Status()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_5_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getDMPInt4Status()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_4_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getDMPInt3Status()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_3_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getDMPInt2Status()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_2_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getDMPInt1Status()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_1_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getDMPInt0Status()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_DMP_INT_STATUS, MPU6050_DMPINT_0_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getIntPLLReadyStatus()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_PLL_RDY_INT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getIntDMPStatus()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_DMP_INT_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

bool LF_Alpha_MPU6050 ::getDMPEnabled()
{
    I2Cdev::readBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_DMP_EN_BIT, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setDMPEnabled(bool enabled)
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_DMP_EN_BIT, enabled, wireObj);
}

void LF_Alpha_MPU6050 ::resetDMP()
{
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_DMP_RESET_BIT, true, wireObj);
}

void LF_Alpha_MPU6050 ::setMemoryBank(uint8_t bank, bool prefetchEnabled, bool userBank)
{
    bank &= 0x1F;
    if (userBank)
        bank |= 0x20;
    if (prefetchEnabled)
        bank |= 0x40;
    I2Cdev::writeByte(devAddr, MPU6050_RA_BANK_SEL, bank, wireObj);
}

void LF_Alpha_MPU6050 ::setMemoryStartAddress(uint8_t address)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_MEM_START_ADDR, address, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::readMemoryByte()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_MEM_R_W, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::writeMemoryByte(uint8_t data)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_MEM_R_W, data, wireObj);
}

void LF_Alpha_MPU6050 ::readMemoryBlock(uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address)
{
    setMemoryBank(bank);
    setMemoryStartAddress(address);
    uint8_t chunkSize;
    for (uint16_t i = 0; i < dataSize;)
    {
        // determine correct chunk size according to bank position and data size
        chunkSize = MPU6050_DMP_MEMORY_CHUNK_SIZE;

        // make sure we don't go past the data size
        if (i + chunkSize > dataSize)
            chunkSize = dataSize - i;

        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address)
            chunkSize = 256 - address;

        // read the chunk of data as specified
        I2Cdev::readBytes(devAddr, MPU6050_RA_MEM_R_W, chunkSize, data + i, I2Cdev::readTimeout, wireObj);

        // increase byte index by [chunkSize]
        i += chunkSize;

        // uint8_t automatically wraps to 0 at 256
        address += chunkSize;

        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize)
        {
            if (address == 0)
                bank++;
            setMemoryBank(bank);
            setMemoryStartAddress(address);
        }
    }
}

bool LF_Alpha_MPU6050 ::writeMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify, bool useProgMem)
{
    setMemoryBank(bank);
    setMemoryStartAddress(address);
    uint8_t chunkSize;
    uint8_t *verifyBuffer = 0;
    uint8_t *progBuffer = 0;
    uint16_t i;
    uint8_t j;
    if (verify)
        verifyBuffer = (uint8_t *)malloc(MPU6050_DMP_MEMORY_CHUNK_SIZE);
    if (useProgMem)
        progBuffer = (uint8_t *)malloc(MPU6050_DMP_MEMORY_CHUNK_SIZE);
    for (i = 0; i < dataSize;)
    {
        // determine correct chunk size according to bank position and data size
        chunkSize = MPU6050_DMP_MEMORY_CHUNK_SIZE;

        // make sure we don't go past the data size
        if (i + chunkSize > dataSize)
            chunkSize = dataSize - i;

        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address)
            chunkSize = 256 - address;

        if (useProgMem)
        {
            // write the chunk of data as specified
            for (j = 0; j < chunkSize; j++)
                progBuffer[j] = pgm_read_byte(data + i + j);
        }
        else
        {
            // write the chunk of data as specified
            progBuffer = (uint8_t *)data + i;
        }

        I2Cdev::writeBytes(devAddr, MPU6050_RA_MEM_R_W, chunkSize, progBuffer, wireObj);

        // verify data if needed
        if (verify && verifyBuffer)
        {
            setMemoryBank(bank);
            setMemoryStartAddress(address);
            I2Cdev::readBytes(devAddr, MPU6050_RA_MEM_R_W, chunkSize, verifyBuffer, I2Cdev::readTimeout, wireObj);
            if (memcmp(progBuffer, verifyBuffer, chunkSize) != 0)
            {
                /*Serial.print("Block write verification error, bank ");
                Serial.print(bank, DEC);
                Serial.print(", address ");
                Serial.print(address, DEC);
                Serial.print("!\nExpected:");
                for (j = 0; j < chunkSize; j++) {
                    Serial.print(" 0x");
                    if (progBuffer[j] < 16) Serial.print("0");
                    Serial.print(progBuffer[j], HEX);
                }
                Serial.print("\nReceived:");
                for (uint8_t j = 0; j < chunkSize; j++) {
                    Serial.print(" 0x");
                    if (verifyBuffer[i + j] < 16) Serial.print("0");
                    Serial.print(verifyBuffer[i + j], HEX);
                }
                Serial.print("\n");*/
                free(verifyBuffer);
                if (useProgMem)
                    free(progBuffer);
                return false; // uh oh.
            }
        }

        // increase byte index by [chunkSize]
        i += chunkSize;

        // uint8_t automatically wraps to 0 at 256
        address += chunkSize;

        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize)
        {
            if (address == 0)
                bank++;
            setMemoryBank(bank);
            setMemoryStartAddress(address);
        }
    }
    if (verify)
        free(verifyBuffer);
    if (useProgMem)
        free(progBuffer);
    return true;
}

bool LF_Alpha_MPU6050 ::writeProgMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify)
{
    return writeMemoryBlock(data, dataSize, bank, address, verify, true);
}

bool LF_Alpha_MPU6050 ::writeDMPConfigurationSet(const uint8_t *data, uint16_t dataSize, bool useProgMem)
{
    uint8_t *progBuffer = 0;
    uint8_t success, special;
    uint16_t i, j;
    if (useProgMem)
    {
        progBuffer = (uint8_t *)malloc(8); // assume 8-byte blocks, realloc later if necessary
    }

    // config set data is a long string of blocks with the following structure:
    // [bank] [offset] [length] [byte[0], byte[1], ..., byte[length]]
    uint8_t bank, offset, length;
    for (i = 0; i < dataSize;)
    {
        if (useProgMem)
        {
            bank = pgm_read_byte(data + i++);
            offset = pgm_read_byte(data + i++);
            length = pgm_read_byte(data + i++);
        }
        else
        {
            bank = data[i++];
            offset = data[i++];
            length = data[i++];
        }

        // write data or perform special action
        if (length > 0)
        {
            // regular block of data to write
            /*Serial.print("Writing config block to bank ");
            Serial.print(bank);
            Serial.print(", offset ");
            Serial.print(offset);
            Serial.print(", length=");
            Serial.println(length);*/
            if (useProgMem)
            {
                if (sizeof(progBuffer) < length)
                    progBuffer = (uint8_t *)realloc(progBuffer, length);
                for (j = 0; j < length; j++)
                    progBuffer[j] = pgm_read_byte(data + i + j);
            }
            else
            {
                progBuffer = (uint8_t *)data + i;
            }
            success = writeMemoryBlock(progBuffer, length, bank, offset, true);
            i += length;
        }
        else
        {
            // special instruction
            // NOTE: this kind of behavior (what and when to do certain things)
            // is totally undocumented. This code is in here based on observed
            // behavior only, and exactly why (or even whether) it has to be here
            // is anybody's guess for now.
            if (useProgMem)
            {
                special = pgm_read_byte(data + i++);
            }
            else
            {
                special = data[i++];
            }
            /*Serial.print("Special command code ");
            Serial.print(special, HEX);
            Serial.println(" found...");*/
            if (special == 0x01)
            {
                // enable DMP-related interrupts

                // setIntZeroMotionEnabled(true);
                // setIntFIFOBufferOverflowEnabled(true);
                // setIntDMPEnabled(true);
                I2Cdev::writeByte(devAddr, MPU6050_RA_INT_ENABLE, 0x32, wireObj); // single operation

                success = true;
            }
            else
            {
                // unknown special command
                success = false;
            }
        }

        if (!success)
        {
            if (useProgMem)
                free(progBuffer);
            return false; // uh oh
        }
    }
    if (useProgMem)
        free(progBuffer);
    return true;
}

bool LF_Alpha_MPU6050 ::writeProgDMPConfigurationSet(const uint8_t *data, uint16_t dataSize)
{
    return writeDMPConfigurationSet(data, dataSize, true);
}

uint8_t LF_Alpha_MPU6050 ::getDMPConfig1()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_DMP_CFG_1, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setDMPConfig1(uint8_t config)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_DMP_CFG_1, config, wireObj);
}

uint8_t LF_Alpha_MPU6050 ::getDMPConfig2()
{
    I2Cdev::readByte(devAddr, MPU6050_RA_DMP_CFG_2, buffer, I2Cdev::readTimeout, wireObj);
    return buffer[0];
}

void LF_Alpha_MPU6050 ::setDMPConfig2(uint8_t config)
{
    I2Cdev::writeByte(devAddr, MPU6050_RA_DMP_CFG_2, config, wireObj);
}

void LF_Alpha_MPU6050 ::CalibrateGyro(uint8_t Loops)
{
    double kP = 0.3;
    double kI = 90;
    float x;
    x = (100 - map(Loops, 1, 5, 20, 0)) * .01;
    kP *= x;
    kI *= x;

    PID(0x43, kP, kI, Loops);
}

void LF_Alpha_MPU6050 ::CalibrateAccel(uint8_t Loops)
{

    float kP = 0.3;
    float kI = 20;
    float x;
    x = (100 - map(Loops, 1, 5, 20, 0)) * .01;
    kP *= x;
    kI *= x;
    PID(0x3B, kP, kI, Loops);
}

void LF_Alpha_MPU6050 ::PID(uint8_t ReadAddress, float kP, float kI, uint8_t Loops)
{
    uint8_t SaveAddress = (ReadAddress == 0x3B) ? ((getDeviceID() < 0x38) ? 0x06 : 0x77) : 0x13;

    int16_t Data;
    float Reading;
    int16_t BitZero[3];
    uint8_t shift = (SaveAddress == 0x77) ? 3 : 2;
    float Error, PTerm, ITerm[3];
    int16_t eSample;
    uint32_t eSum;
    uint16_t gravity = 8192; // prevent uninitialized compiler warning
    if (ReadAddress == 0x3B)
        gravity = 32768 >> getFullScaleAccelRange();
    Serial.write('>');
    for (int i = 0; i < 3; i++)
    {
        I2Cdev::readWords(devAddr, SaveAddress + (i * shift), 1, (uint16_t *)&Data, I2Cdev::readTimeout, wireObj); // reads 1 or more 16 bit integers (Word)
        Reading = Data;
        if (SaveAddress != 0x13)
        {
            BitZero[i] = Data & 1; // Capture Bit Zero to properly handle Accelerometer calibration
            ITerm[i] = ((float)Reading) * 8;
        }
        else
        {
            ITerm[i] = Reading * 4;
        }
    }
    for (int L = 0; L < Loops; L++)
    {
        eSample = 0;
        for (int c = 0; c < 100; c++)
        { // 100 PI Calculations
            eSum = 0;
            for (int i = 0; i < 3; i++)
            {
                I2Cdev::readWords(devAddr, ReadAddress + (i * 2), 1, (uint16_t *)&Data, I2Cdev::readTimeout, wireObj); // reads 1 or more 16 bit integers (Word)
                Reading = Data;
                if ((ReadAddress == 0x3B) && (i == 2))
                    Reading -= gravity; // remove Gravity
                Error = -Reading;
                eSum += abs(Reading);
                PTerm = kP * Error;
                ITerm[i] += (Error * 0.001) * kI; // Integral term 1000 Calculations a second = 0.001
                if (SaveAddress != 0x13)
                {
                    Data = round((PTerm + ITerm[i]) / 8);  // Compute PID Output
                    Data = ((Data) & 0xFFFE) | BitZero[i]; // Insert Bit0 Saved at beginning
                }
                else
                    Data = round((PTerm + ITerm[i]) / 4); // Compute PID Output
                I2Cdev::writeWords(devAddr, SaveAddress + (i * shift), 1, (uint16_t *)&Data, wireObj);
            }
            if ((c == 99) && eSum > 1000)
            { // Error is still to great to continue
                c = 0;
                Serial.write('*');
            }
            if ((eSum * ((ReadAddress == 0x3B) ? .05 : 1)) < 5)
                eSample++; // Successfully found offsets prepare to  advance
            if ((eSum < 100) && (c > 10) && (eSample >= 10))
                break; // Advance to next Loop
            delay(1);
        }
        Serial.write('.');
        kP *= .75;
        kI *= .75;
        for (int i = 0; i < 3; i++)
        {
            if (SaveAddress != 0x13)
            {
                Data = round((ITerm[i]) / 8);          // Compute PID Output
                Data = ((Data) & 0xFFFE) | BitZero[i]; // Insert Bit0 Saved at beginning
            }
            else
                Data = round((ITerm[i]) / 4);
            I2Cdev::writeWords(devAddr, SaveAddress + (i * shift), 1, (uint16_t *)&Data, wireObj);
        }
    }
    resetFIFO();
    resetDMP();
}

int16_t *LF_Alpha_MPU6050 ::GetActiveOffsets()
{
    uint8_t AOffsetRegister = (getDeviceID() < 0x38) ? MPU6050_RA_XA_OFFS_H : 0x77;
    if (AOffsetRegister == 0x06)
        I2Cdev::readWords(devAddr, AOffsetRegister, 3, (uint16_t *)offsets, I2Cdev::readTimeout, wireObj);
    else
    {
        I2Cdev::readWords(devAddr, AOffsetRegister, 1, (uint16_t *)offsets, I2Cdev::readTimeout, wireObj);
        I2Cdev::readWords(devAddr, AOffsetRegister + 3, 1, (uint16_t *)(offsets + 1), I2Cdev::readTimeout, wireObj);
        I2Cdev::readWords(devAddr, AOffsetRegister + 6, 1, (uint16_t *)(offsets + 2), I2Cdev::readTimeout, wireObj);
    }
    I2Cdev::readWords(devAddr, 0x13, 3, (uint16_t *)(offsets + 3), I2Cdev::readTimeout, wireObj);
    return offsets;
}

void LF_Alpha_MPU6050 ::PrintActiveOffsets()
{
    GetActiveOffsets();
    //	A_OFFSET_H_READ_A_OFFS(Data);
    Serial.print((float)offsets[0], 5);
    Serial.print(",\t");
    Serial.print((float)offsets[1], 5);
    Serial.print(",\t");
    Serial.print((float)offsets[2], 5);
    Serial.print(",\t");

    //	XG_OFFSET_H_READ_OFFS_USR(Data);
    Serial.print((float)offsets[3], 5);
    Serial.print(",\t");
    Serial.print((float)offsets[4], 5);
    Serial.print(",\t");
    Serial.print((float)offsets[5], 5);
    Serial.print("\n\n");
}

// MPU6050 public methods definitions
bool LF_Alpha_MPU6050 ::begin()
{
    initialize();
    return testConnection();
}

void LF_Alpha_MPU6050 ::deepCalibrateSensor()
{
    for (i = iAx; i <= iGz; i++)
    {
        Target[i] = 0; // Fix for ZAccel
        HighOffset[i] = 0;
        LowOffset[i] = 0;
    }
    Target[iAz] = 16384; // Set the taget for Z axes
    this->PullBracketsOut();
    this->PullBracketsIn();
    this->calibrateSensor();
}

void LF_Alpha_MPU6050 ::calibrateSensor()
{
    int16_t gyroX, gyroY, gyroZ;
    int tempGyroData = 0;
    gyroOffsetValue = 0;

    for (uint8_t i = 0; i < 5; i++)
    {
        getRotation(&gyroX, &gyroY, &gyroZ);
        tempGyroData = (sqrt(pow(gyroX, 2) + pow(gyroY, 2) + pow(gyroZ, 2))) / 100;
        if (tempGyroData < 0)
        {
            tempGyroData = -(tempGyroData);
        }
        gyroOffsetValue = gyroOffsetValue + tempGyroData;
        delay(500);
    }
    gyroOffsetValue = gyroOffsetValue / 5;
}

bool LF_Alpha_MPU6050 ::getMotionState()
{
    int16_t gyroX, gyroY, gyroZ;
    getRotation(&gyroX, &gyroY, &gyroZ);
    int16_t gyroMagnitude = sqrt(pow(gyroX, 2) + pow(gyroY, 2) + pow(gyroZ, 2));

    if (gyroMagnitude < 0)
    {
        gyroMagnitude = -(gyroMagnitude);
    }

    gyroMagnitude = gyroMagnitude - (gyroOffsetValue * 100);
    if (gyroMagnitude < idleThresholdGyro)
    {
        return false; // Device is idle
    }

    else
    {
        return true; // Device is in motion
    }
}

void LF_Alpha_MPU6050 ::setIdleThresholdGyro(int16_t threshold)
{
    idleThresholdGyro = threshold;
}

int16_t LF_Alpha_MPU6050 ::readAccX()
{
    int16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ;
    getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
    return accelX;
}

int16_t LF_Alpha_MPU6050 ::readAccY()
{
    int16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ;
    getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
    return accelY;
}

int16_t LF_Alpha_MPU6050 ::readAccZ()
{
    int16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ;
    getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
    return accelZ;
}

int16_t LF_Alpha_MPU6050 ::readGyroX()
{
    int16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ;
    getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
    return gyroX;
}

int16_t LF_Alpha_MPU6050 ::readGyroY()
{
    int16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ;
    getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
    return gyroY;
}

int16_t LF_Alpha_MPU6050 ::readGyroZ()
{
    int16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ;
    getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
    return gyroZ;
}

bool LF_Alpha_MPU6050 ::end()
{
    return true;
}

// MPU6050 supporting functions
void LF_Alpha_MPU6050 ::PullBracketsOut()
{
    bool Done = false;
    int NextLowOffset[6];
    int NextHighOffset[6];

    ForceHeader();

    while (!Done)
    {
        Done = true;
        SetOffsets(LowOffset); // Set low offsets
        GetSmoothed();
        for (i = 0; i <= 5; i++)
        { // Get low values
            LowValue[i] = Smoothed[i];
            if (LowValue[i] >= Target[i])
            {
                Done = false;
                NextLowOffset[i] = LowOffset[i] - 1000;
            }
            else
            {
                NextLowOffset[i] = LowOffset[i];
            }
        }
        SetOffsets(HighOffset);
        GetSmoothed();
        for (i = 0; i <= 5; i++)
        { // Get high values
            HighValue[i] = Smoothed[i];
            if (HighValue[i] <= Target[i])
            {
                Done = false;
                NextHighOffset[i] = HighOffset[i] + 1000;
            }
            else
            {
                NextHighOffset[i] = HighOffset[i];
            }
        }
        for (int i = 0; i <= 5; i++)
        {
            LowOffset[i] = NextLowOffset[i];
            HighOffset[i] = NextHighOffset[i];
        }
    }
}

void LF_Alpha_MPU6050 ::PullBracketsIn()
{
    bool AllBracketsNarrow;
    bool StillWorking;
    int NewOffset[6];

    AllBracketsNarrow = false;
    ForceHeader();
    StillWorking = true;
    while (StillWorking)
    {
        StillWorking = false;
        if (AllBracketsNarrow && (N == NFast))
        {
            N = NSlow;
        }
        else
        {
            AllBracketsNarrow = true;
        }
        for (int i = 0; i <= 5; i++)
        {
            if (HighOffset[i] <= (LowOffset[i] + 1))
            {
                NewOffset[i] = LowOffset[i];
            }
            else
            { // Binary search
                StillWorking = true;
                NewOffset[i] = (LowOffset[i] + HighOffset[i]) / 2;
                if (HighOffset[i] > (LowOffset[i] + 10))
                {
                    AllBracketsNarrow = false;
                }
            }
        }
        SetOffsets(NewOffset);
        GetSmoothed();
        for (i = 0; i <= 5; i++)
        { // Closing in
            if (Smoothed[i] > Target[i])
            { // Use lower half
                HighOffset[i] = NewOffset[i];
                HighValue[i] = Smoothed[i];
            }
            else
            { // Use upper half
                LowOffset[i] = NewOffset[i];
                LowValue[i] = Smoothed[i];
            }
        }

    }
}

void LF_Alpha_MPU6050 ::ForceHeader()
{
    LinesOut = 99;
}

/*Function to smooth the read values*/
void LF_Alpha_MPU6050 ::GetSmoothed()
{
    // Defensive: ensure N is valid to prevent division by zero
    if (N <= 0) {
        N = NFast;
    }

    int16_t RawValue[6];
    long Sums[6];
    for (i = 0; i <= 5; i++)
    {
        Sums[i] = 0;
    }

    /* Get Sums*/
    for (i = 1; i <= N; i++)
    {
        getMotion6(&RawValue[iAx], &RawValue[iAy], &RawValue[iAz], &RawValue[iGx], &RawValue[iGy], &RawValue[iGz]);
        delayMicroseconds(usDelay);
        for (int j = 0; j <= 5; j++)
        {
            Sums[j] = Sums[j] + RawValue[j];
        }
    }
    for (i = 0; i <= 5; i++)
    {
        // extra guard - N guaranteed > 0 above
        Smoothed[i] = (Sums[i] + (N / 2)) / N;
    }
}

/*Function for configure the oba=tained offsets*/
void LF_Alpha_MPU6050 ::SetOffsets(int TheOffsets[6])
{
    setXAccelOffset(TheOffsets[iAx]);
    setYAccelOffset(TheOffsets[iAy]);
    setZAccelOffset(TheOffsets[iAz]);
    setXGyroOffset(TheOffsets[iGx]);
    setYGyroOffset(TheOffsets[iGy]);
    setZGyroOffset(TheOffsets[iGz]);
}

// LF_Alpha_APDS9960 methods definitions

float powf(const float x, const float y)
{
    return (float)(pow((double)x, (double)y));
}

// private methods
uint32_t LF_Alpha_APDS9960 ::read32(uint8_t reg)
{
    uint8_t ret[4];
    uint32_t ret32;
    this->read(reg, ret, 4);
    ret32 = ret[3];
    ret32 |= (uint32_t)ret[2] << 8;
    ret32 |= (uint32_t)ret[1] << 16;
    ret32 |= (uint32_t)ret[0] << 24;
    return ret32;
}

uint16_t LF_Alpha_APDS9960 ::read16(uint8_t reg)
{
    uint8_t ret[2];
    this->read(reg, ret, 2);

    return (ret[0] << 8) | ret[1];
}

uint16_t LF_Alpha_APDS9960 ::read16R(uint8_t reg)
{
    uint8_t ret[2];
    this->read(reg, ret, 2);

    return (ret[1] << 8) | ret[0];
}

void LF_Alpha_APDS9960 ::write8(byte reg, byte value)
{
    this->write(reg, &value, 1);
}

uint8_t LF_Alpha_APDS9960 ::read8(byte reg)
{
    uint8_t ret;
    this->read(reg, &ret, 1);

    return ret;
}

uint8_t LF_Alpha_APDS9960 ::read(uint8_t reg, uint8_t *buf, uint8_t num)
{
    buf[0] = reg;
    i2c_dev->write_then_read(buf, 1, buf, num);
    return num;
}

void LF_Alpha_APDS9960 ::write(uint8_t reg, uint8_t *buf, uint8_t num)
{
    uint8_t prefix[1] = {reg};
    i2c_dev->write(buf, num, true, prefix, 1);
}

// public method defintions
bool LF_Alpha_APDS9960 ::begin(uint16_t iTimeMS, apds9960AGain_t aGain, uint8_t addr, TwoWire *theWire)
{

    if (i2c_dev)
        delete i2c_dev;

    if (i2cInitFlag == false)
    {
        i2cInitFlag = true;
        Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL); // Initialize I2C for all instances
    }
    theWire = theWire ? theWire : &Wire;

    i2c_dev = new Adafruit_I2CDevice(addr, theWire);
    if (!i2c_dev->begin())
    {
        return false;
    }

    /* Make sure we're actually connected */
    uint8_t x = read8(APDS9960_ID);
    if (x != 0xAB)
    {
        return false;
    }

    /* Set default integration time and gain */
    setADCIntegrationTime(iTimeMS);
    setADCGain(aGain);

    // disable everything to start
    enableGesture(false);
    enableProximity(false);
    enableColor(false);

    disableColorInterrupt();
    disableProximityInterrupt();
    clearInterrupt();

    /* Note: by default, the device is in power down mode on bootup */
    enable(false);
    delay(10);
    enable(true);
    delay(10);

    // default to all gesture dimensions
    setGestureDimensions(APDS9960_DIMENSIONS_ALL);
    setGestureFIFOThreshold(APDS9960_GFIFO_4);
    setGestureGain(APDS9960_GGAIN_4);
    setGestureProximityThreshold(50);
    resetCounts();

    _gpulse.GPLEN = APDS9960_GPULSE_32US;
    _gpulse.GPULSE = 9; // 10 pulses
    this->write8(APDS9960_GPULSE, _gpulse.get());

    return true;
}

void LF_Alpha_APDS9960 ::setADCIntegrationTime(uint16_t iTimeMS)
{
    float temp;

    // convert ms into 2.78ms increments
    temp = iTimeMS;
    temp /= 2.78;
    temp = 256 - temp;
    if (temp > 255)
        temp = 255;
    if (temp < 0)
        temp = 0;

    /* Update the timing register */
    write8(APDS9960_ATIME, (uint8_t)temp);
}

float LF_Alpha_APDS9960 ::getADCIntegrationTime()
{
    float temp;

    temp = read8(APDS9960_ATIME);

    // convert to units of 2.78 ms
    temp = 256 - temp;
    temp *= 2.78;
    return temp;
}

void LF_Alpha_APDS9960 ::setADCGain(apds9960AGain_t aGain)
{
    _control.AGAIN = aGain;

    /* Update the timing register */
    write8(APDS9960_CONTROL, _control.get());
}

apds9960AGain_t LF_Alpha_APDS9960 ::getADCGain()
{
    return (apds9960AGain_t)(read8(APDS9960_CONTROL) & 0x03);
}

void LF_Alpha_APDS9960 ::setLED(apds9960LedDrive_t drive, apds9960LedBoost_t boost)
{
    // set BOOST
    _config2.LED_BOOST = boost;
    write8(APDS9960_CONFIG2, _config2.get());

    _control.LDRIVE = drive;
    write8(APDS9960_CONTROL, _control.get());
}

// proximity methods
void LF_Alpha_APDS9960::enableProximity(boolean en)
{
    _enable.PEN = en;

    write8(APDS9960_ENABLE, _enable.get());
}

void LF_Alpha_APDS9960 ::setProxGain(apds9960PGain_t pGain)
{
    _control.PGAIN = pGain;

    /* Update the timing register */
    write8(APDS9960_CONTROL, _control.get());
}

apds9960PGain_t LF_Alpha_APDS9960 ::getProxGain()
{
    return (apds9960PGain_t)((read8(APDS9960_CONTROL) & 0x0C) >> 2);
}

void LF_Alpha_APDS9960 ::setProxPulse(apds9960PPulseLen_t pLen, uint8_t pulses)
{
    if (pulses < 1)
        pulses = 1;
    if (pulses > 64)
        pulses = 64;
    pulses--;

    _ppulse.PPLEN = pLen;
    _ppulse.PPULSE = pulses;

    write8(APDS9960_PPULSE, _ppulse.get());
}

void LF_Alpha_APDS9960 ::enableProximityInterrupt()
{
    _enable.PIEN = 1;
    write8(APDS9960_ENABLE, _enable.get());
    clearInterrupt();
}

void LF_Alpha_APDS9960 ::disableProximityInterrupt()
{
    _enable.PIEN = 0;
    write8(APDS9960_ENABLE, _enable.get());
}

uint8_t LF_Alpha_APDS9960 ::readProximity() { return read8(APDS9960_PDATA); }

void LF_Alpha_APDS9960 ::setProximityInterruptThreshold(uint8_t low, uint8_t high, uint8_t persistence)
{
    write8(APDS9960_PILT, low);
    write8(APDS9960_PIHT, high);

    if (persistence > 7)
        persistence = 7;
    _pers.PPERS = persistence;
    write8(APDS9960_PERS, _pers.get());
}

bool LF_Alpha_APDS9960 ::getProximityInterrupt()
{
    _status.set(this->read8(APDS9960_STATUS));
    return _status.PINT;
}

// Gesture methods
void LF_Alpha_APDS9960 ::enableGesture(boolean en)
{
    if (!en)
    {
        _gconf4.GMODE = 0;
        write8(APDS9960_GCONF4, _gconf4.get());
    }
    _enable.GEN = en;
    write8(APDS9960_ENABLE, _enable.get());
    resetCounts();
    // update to enable gesture sensing without proximity sensing
    if (en)
    { // start the gesture engine here without a prox. gesture
        write8(APDS9960_GCONF4, 0x01);
    }
}

bool LF_Alpha_APDS9960 ::gestureValid()
{
    _gstatus.set(this->read8(APDS9960_GSTATUS));
    return _gstatus.GVALID;
}

void LF_Alpha_APDS9960 ::setGestureDimensions(uint8_t dims)
{
    _gconf3.GDIMS = dims;
    this->write8(APDS9960_GCONF3, _gconf3.get());
}

void LF_Alpha_APDS9960 ::setGestureFIFOThreshold(uint8_t thresh)
{
    _gconf1.GFIFOTH = thresh;
    this->write8(APDS9960_GCONF1, _gconf1.get());
}

void LF_Alpha_APDS9960 ::setGestureGain(uint8_t gain)
{
    _gconf2.GGAIN = gain;
    this->write8(APDS9960_GCONF2, _gconf2.get());
}

void LF_Alpha_APDS9960 ::setGestureProximityThreshold(uint8_t thresh)
{
    this->write8(APDS9960_GPENTH, thresh);
}

void LF_Alpha_APDS9960 ::setGestureOffset(uint8_t offset_up, uint8_t offset_down, uint8_t offset_left, uint8_t offset_right)
{
    this->write8(APDS9960_GOFFSET_U, offset_up);
    this->write8(APDS9960_GOFFSET_D, offset_down);
    this->write8(APDS9960_GOFFSET_L, offset_left);
    this->write8(APDS9960_GOFFSET_R, offset_right);
}

uint8_t LF_Alpha_APDS9960 ::readGesture()
{
    uint8_t toRead;
    uint8_t buf[256];
    unsigned long t = 0;
    uint8_t gestureReceived;
    while (1)
    {
        int up_down_diff = 0;
        int left_right_diff = 0;
        gestureReceived = 0;
        if (!gestureValid())
            return 0;

        toRead = this->read8(APDS9960_GFLVL);

        // produces sideffects needed for readGesture to work
        this->read(APDS9960_GFIFO_U, buf, toRead);

        // Process ALL 4-byte datasets [U, D, L, R] in the FIFO buffer
        for (int i = 0; i < toRead; i += 4)
        {
            if (abs((int)buf[i] - (int)buf[i + 1]) > 13)
                up_down_diff += (int)buf[i] - (int)buf[i + 1];

            if (abs((int)buf[i + 2] - (int)buf[i + 3]) > 13)
                left_right_diff += (int)buf[i + 2] - (int)buf[i + 3];
        }

        if (up_down_diff != 0)
        {
            if (up_down_diff < 0)
            {
                if (DCount > 0)
                {
                    gestureReceived = APDS9960_UP;
                }
                else
                    UCount++;
            }
            else if (up_down_diff > 0)
            {
                if (UCount > 0)
                {
                    gestureReceived = APDS9960_DOWN;
                }
                else
                    DCount++;
            }
        }

        if (left_right_diff != 0)
        {
            if (left_right_diff < 0)
            {
                if (RCount > 0)
                {
                    gestureReceived = APDS9960_LEFT;
                }
                else
                    LCount++;
            }
            else if (left_right_diff > 0)
            {
                if (LCount > 0)
                {
                    gestureReceived = APDS9960_RIGHT;
                }
                else
                    RCount++;
            }
        }

        if (up_down_diff != 0 || left_right_diff != 0)
            t = millis();

        if (gestureReceived || millis() - t > 150)
        {
            resetCounts();
            return gestureReceived;
        }
    }
}

void LF_Alpha_APDS9960 ::resetCounts()
{
    gestCnt = 0;
    UCount = 0;
    DCount = 0;
    LCount = 0;
    RCount = 0;
}

// Color methods
void LF_Alpha_APDS9960 ::enableColor(boolean en)
{
    _enable.AEN = en;
    write8(APDS9960_ENABLE, _enable.get());
}

bool LF_Alpha_APDS9960 ::colorDataReady()
{
    _status.set(this->read8(APDS9960_STATUS));
    return _status.AVALID;
}

void LF_Alpha_APDS9960 ::readColors(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
    *c = read16R(APDS9960_CDATAL);
    *r = read16R(APDS9960_RDATAL);
    *g = read16R(APDS9960_GDATAL);
    *b = read16R(APDS9960_BDATAL);
}

uint16_t LF_Alpha_APDS9960 ::calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b)
{
    float X, Y, Z; /* RGB to XYZ correlation      */
    float xc, yc;  /* Chromaticity co-ordinates   */
    float n;       /* McCamy's formula            */
    float cct;

    /* 1. Map RGB values to their XYZ counterparts.    */
    /* Based on 6500K fluorescent, 3000K fluorescent   */
    /* and 60W incandescent values for a wide range.   */
    /* Note: Y = Illuminance or lux                    */
    X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
    Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
    Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

    /* 2. Calculate the chromaticity co-ordinates      */
    xc = (X) / (X + Y + Z);
    yc = (Y) / (X + Y + Z);

    /* 3. Use McCamy's formula to determine the CCT    */
    n = (xc - 0.3320F) / (0.1858F - yc);

    /* Calculate the final CCT */
    cct =
        (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

    /* Return the results in degrees Kelvin */
    return (uint16_t)cct;
}

uint16_t LF_Alpha_APDS9960 ::calculateLux(uint16_t r, uint16_t g, uint16_t b)
{
    float illuminance;

    /* This only uses RGB ... how can we integrate clear or calculate lux */
    /* based exclusively on clear since this might be more reliable?      */
    illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

    return (uint16_t)illuminance;
}

void LF_Alpha_APDS9960 ::enableColorInterrupt()
{
    _enable.AIEN = 1;
    write8(APDS9960_ENABLE, _enable.get());
}

void LF_Alpha_APDS9960 ::disableColorInterrupt()
{
    _enable.AIEN = 0;
    write8(APDS9960_ENABLE, _enable.get());
}

void LF_Alpha_APDS9960 ::clearInterrupt()
{
    this->write(APDS9960_AICLEAR, NULL, 0);
}

void LF_Alpha_APDS9960 ::setIntLimits(uint16_t low, uint16_t high)
{
    write8(APDS9960_AILTIL, low & 0xFF);
    write8(APDS9960_AILTH, low >> 8);
    write8(APDS9960_AIHTL, high & 0xFF);
    write8(APDS9960_AIHTH, high >> 8);
}

void LF_Alpha_APDS9960 ::enable(boolean en)
{
    _enable.PON = en;
    this->write8(APDS9960_ENABLE, _enable.get());
}

// LF_Alpha_MAX30102 methods definitions
// private methods
void LF_Alpha_MAX30102::softReset(void)
{
    sMode_t modeReg;
    readReg(MAX30102_MODECONFIG, &modeReg, 1);
    modeReg.reset = 1;
    writeReg(MAX30102_MODECONFIG, &modeReg, 1);
    uint32_t startTime = millis();
    while (millis() - startTime < 100)
    {
        readReg(MAX30102_MODECONFIG, &modeReg, 1);
        if (modeReg.reset == 0)
            break;
        delay(1);
    }
}

void LF_Alpha_MAX30102::shutDown(void)
{
    sMode_t modeReg;
    readReg(MAX30102_MODECONFIG, &modeReg, 1);
    modeReg.shutDown = 1;
    writeReg(MAX30102_MODECONFIG, &modeReg, 1);
}

void LF_Alpha_MAX30102::wakeUp(void)
{
    sMode_t modeReg;
    readReg(MAX30102_MODECONFIG, &modeReg, 1);
    modeReg.shutDown = 0;
    writeReg(MAX30102_MODECONFIG, &modeReg, 1);
}

void LF_Alpha_MAX30102::setLEDMode(uint8_t ledMode)
{
    sMode_t modeReg;
    readReg(MAX30102_MODECONFIG, &modeReg, 1);
    modeReg.ledMode = ledMode;
    writeReg(MAX30102_MODECONFIG, &modeReg, 1);
}

void LF_Alpha_MAX30102::setADCRange(uint8_t adcRange)
{
    sParticle_t particleReg;
    readReg(MAX30102_PARTICLECONFIG, &particleReg, 1);
    particleReg.adcRange = adcRange;
    writeReg(MAX30102_PARTICLECONFIG, &particleReg, 1);
}

void LF_Alpha_MAX30102::setSampleRate(uint8_t sampleRate)
{
    sParticle_t particleReg;
    readReg(MAX30102_PARTICLECONFIG, &particleReg, 1);
    particleReg.sampleRate = sampleRate;
    writeReg(MAX30102_PARTICLECONFIG, &particleReg, 1);
}

void LF_Alpha_MAX30102::setPulseWidth(uint8_t pulseWidth)
{
    sParticle_t particleReg;
    readReg(MAX30102_PARTICLECONFIG, &particleReg, 1);
    particleReg.pulseWidth = pulseWidth;
    writeReg(MAX30102_PARTICLECONFIG, &particleReg, 1);
}

void LF_Alpha_MAX30102::setPulseAmplitudeRed(uint8_t amplitude)
{
    uint8_t byteTemp = amplitude;
    writeReg(MAX30102_LED1_PULSEAMP, &byteTemp, 1);
}

void LF_Alpha_MAX30102::setPulseAmplitudeIR(uint8_t amplitude)
{
    uint8_t byteTemp = amplitude;
    writeReg(MAX30102_LED2_PULSEAMP, &byteTemp, 1);
}

void LF_Alpha_MAX30102::enableSlot(uint8_t slotNumber, uint8_t device)
{
    sMultiLED_t multiLEDReg;
    switch (slotNumber)
    {
    case (1):
        readReg(MAX30102_MULTILEDCONFIG1, &multiLEDReg, 1);
        multiLEDReg.SLOT1 = device;
        writeReg(MAX30102_MULTILEDCONFIG1, &multiLEDReg, 1);
        break;
    case (2):
        readReg(MAX30102_MULTILEDCONFIG1, &multiLEDReg, 1);
        multiLEDReg.SLOT2 = device;
        writeReg(MAX30102_MULTILEDCONFIG1, &multiLEDReg, 1);
        break;
    default:
        break;
    }
}

void LF_Alpha_MAX30102::disableAllSlots(void)
{
    sMultiLED_t multiLEDReg;
    multiLEDReg.SLOT1 = 0;
    multiLEDReg.SLOT2 = 0;
    writeReg(MAX30102_MULTILEDCONFIG1, &multiLEDReg, 1);
}

void LF_Alpha_MAX30102::enableAlmostFull(void)
{
    sEnable_t enableReg;
    readReg(MAX30102_INTENABLE1, &enableReg, 2);
    enableReg.almostFull = 1;
    writeReg(MAX30102_INTENABLE1, &enableReg, 2);
}

void LF_Alpha_MAX30102::disableAlmostFull(void)
{
    sEnable_t enableReg;
    readReg(MAX30102_INTENABLE1, &enableReg, 2);
    enableReg.almostFull = 0;
    writeReg(MAX30102_INTENABLE1, &enableReg, 2);
}

void LF_Alpha_MAX30102::enableDataReady(void)
{
    sEnable_t enableReg;
    readReg(MAX30102_INTENABLE1, &enableReg, 2);
    enableReg.dataReady = 1;
    writeReg(MAX30102_INTENABLE1, &enableReg, 2);
}

void LF_Alpha_MAX30102::disableDataReady(void)
{
    sEnable_t enableReg;
    readReg(MAX30102_INTENABLE1, &enableReg, 2);
    enableReg.dataReady = 0;
    writeReg(MAX30102_INTENABLE1, &enableReg, 2);
}

void LF_Alpha_MAX30102::enableALCOverflow(void)
{
    sEnable_t enableReg;
    readReg(MAX30102_INTENABLE1, &enableReg, 2);
    enableReg.ALCOverflow = 1;
    writeReg(MAX30102_INTENABLE1, &enableReg, 2);
}

void LF_Alpha_MAX30102::disableALCOverflow(void)
{
    sEnable_t enableReg;
    readReg(MAX30102_INTENABLE1, &enableReg, 2);
    enableReg.ALCOverflow = 0;
    writeReg(MAX30102_INTENABLE1, &enableReg, 2);
}

void LF_Alpha_MAX30102::enableDieTempReady(void)
{
    sEnable_t enableReg;
    readReg(MAX30102_INTENABLE1, &enableReg, 2);
    enableReg.dieTemp = 1;
    writeReg(MAX30102_INTENABLE1, &enableReg, 2);
}

void LF_Alpha_MAX30102::disableDieTempReady(void)
{
    sEnable_t enableReg;
    readReg(MAX30102_INTENABLE1, &enableReg, 2);
    enableReg.dieTemp = 0;
    writeReg(MAX30102_INTENABLE1, &enableReg, 2);
}

void LF_Alpha_MAX30102::setFIFOAverage(uint8_t numberOfSamples)
{
    sFIFO_t FIFOReg;
    readReg(MAX30102_FIFOCONFIG, &FIFOReg, 1);
    FIFOReg.sampleAverag = numberOfSamples;
    writeReg(MAX30102_FIFOCONFIG, &FIFOReg, 1);
}

void LF_Alpha_MAX30102::enableFIFORollover(void)
{
    sFIFO_t FIFOReg;
    readReg(MAX30102_FIFOCONFIG, &FIFOReg, 1);
    FIFOReg.RollOver = 1;
    writeReg(MAX30102_FIFOCONFIG, &FIFOReg, 1);
}

void LF_Alpha_MAX30102::disableFIFORollover(void)
{
    sFIFO_t FIFOReg;
    readReg(MAX30102_FIFOCONFIG, &FIFOReg, 1);
    FIFOReg.RollOver = 0;
    writeReg(MAX30102_FIFOCONFIG, &FIFOReg, 1);
}

void LF_Alpha_MAX30102::setFIFOAlmostFull(uint8_t numberOfSamples)
{
    sFIFO_t FIFOReg;
    readReg(MAX30102_FIFOCONFIG, &FIFOReg, 1);
    FIFOReg.almostFull = numberOfSamples;
    writeReg(MAX30102_FIFOCONFIG, &FIFOReg, 1);
}

uint8_t LF_Alpha_MAX30102::getPartID()
{
    uint8_t byteTemp;
    readReg(MAX30102_PARTID, &byteTemp, 1);
    return byteTemp;
}

uint8_t LF_Alpha_MAX30102::getWritePointer(void)
{
    uint8_t byteTemp;
    readReg(MAX30102_FIFOWRITEPTR, &byteTemp, 1);
    return byteTemp;
}

uint8_t LF_Alpha_MAX30102::getReadPointer(void)
{
    uint8_t byteTemp;
    readReg(MAX30102_FIFOREADPTR, &byteTemp, 1);
    return byteTemp;
}

void LF_Alpha_MAX30102::resetFIFO(void)
{
    uint8_t byteTemp = 0;
    writeReg(MAX30102_FIFOWRITEPTR, &byteTemp, 1);
    writeReg(MAX30102_FIFOOVERFLOW, &byteTemp, 1);
    writeReg(MAX30102_FIFOREADPTR, &byteTemp, 1);
}

void LF_Alpha_MAX30102::getNewData(void)
{
    int32_t numberOfSamples = 0;
    uint8_t readPointer = 0;
    uint8_t writePointer = 0;
    while (1)
    {
        readPointer = getReadPointer();
        writePointer = getWritePointer();

        if (readPointer == writePointer)
        {
            return;
        }
        else
        {
            numberOfSamples = writePointer - readPointer;
            if (numberOfSamples < 0)
                numberOfSamples += 32;
            int32_t bytesNeedToRead = numberOfSamples * _activeLEDs * 3;

            while (bytesNeedToRead > 0)
            {
                senseBuf.head++;
                senseBuf.head %= MAX30102_SENSE_BUF_SIZE;
                uint32_t tempBuf = 0;
                if (_activeLEDs > 1)
                {
                    uint8_t temp[6];
                    uint8_t tempex;

                    readReg(MAX30102_FIFODATA, temp, 6);

                    for (uint8_t i = 0; i < 3; i++)
                    {
                        tempex = temp[i];
                        temp[i] = temp[5 - i];
                        temp[5 - i] = tempex;
                    }

                    memcpy(&tempBuf, temp, 3 * sizeof(temp[0]));
                    tempBuf &= 0x3FFFF;
                    senseBuf.IR[senseBuf.head] = tempBuf;
                    memcpy(&tempBuf, temp + 3, 3 * sizeof(temp[0]));
                    tempBuf &= 0x3FFFF;
                    senseBuf.red[senseBuf.head] = tempBuf;
                }
                else
                {
                    uint8_t temp[3];
                    uint8_t tempex;

                    readReg(MAX30102_FIFODATA, temp, 3);
                    tempex = temp[0];
                    temp[0] = temp[2];
                    temp[2] = tempex;

                    memcpy(&tempBuf, temp, 3 * sizeof(temp[0]));
                    tempBuf &= 0x3FFFF;
                    senseBuf.red[senseBuf.head] = tempBuf;
                }
                bytesNeedToRead -= _activeLEDs * 3;
            }
            return;
        }
        delay(1);
    }
}

void LF_Alpha_MAX30102::writeReg(uint8_t reg, const void *pBuf, uint8_t size)
{
    if (pBuf == NULL)
    {
        return;
    }
    uint8_t *_pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_i2cAddr);
    _pWire->write(&reg, 1);

    for (uint16_t i = 0; i < size; i++)
    {
        _pWire->write(_pBuf[i]);
    }
    _pWire->endTransmission();
}

uint8_t LF_Alpha_MAX30102::readReg(uint8_t reg, const void *pBuf, uint8_t size)
{
    if (pBuf == NULL)
    {
        return 0;
    }

    uint8_t *_pBuf = (uint8_t *)pBuf;
    _pWire->beginTransmission(_i2cAddr);
    _pWire->write(&reg, 1);

    if (_pWire->endTransmission() != 0)
    {
        return 0;
    }

    _pWire->requestFrom(_i2cAddr, size);
    for (uint16_t i = 0; i < size; i++)
    {
        _pBuf[i] = _pWire->read();
    }
    return size;
}

// public methods
bool LF_Alpha_MAX30102::begin(uint8_t i2cAddr, TwoWire *pWire)
{
    if (i2cInitFlag == false)
    {
        i2cInitFlag = true;
        Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL); // Initialize I2C for all instances
    }

    _i2cAddr = i2cAddr;
    _pWire = pWire;
    _pWire->begin();

    softReset();
    return true;
}

void LF_Alpha_MAX30102::configure(uint8_t ledBrightness, uint8_t sampleAverage, uint8_t ledMode, uint8_t sampleRate, uint8_t pulseWidth, uint8_t adcRange)
{

    setFIFOAverage(sampleAverage);

    setADCRange(adcRange);

    setSampleRate(sampleRate);

    setPulseWidth(pulseWidth);

    setPulseAmplitudeRed(ledBrightness);
    setPulseAmplitudeIR(ledBrightness);

    enableSlot(1, SLOT_RED_LED);
    if (ledMode > MODE_REDONLY)
        enableSlot(2, SLOT_IR_LED);

    setLEDMode(ledMode);

    if (ledMode == MODE_REDONLY)
    {
        _activeLEDs = 1;
    }
    else
    {
        _activeLEDs = 2;
    }

    enableFIFORollover();
    resetFIFO();
}

void LF_Alpha_MAX30102 ::readSPO2(int32_t *SPO2, int8_t *SPO2Valid, int32_t *heartRate, int8_t *heartRateValid)
{
    uint32_t irBuffer[100];
    uint32_t redBuffer[100];

    int32_t bufferLength = 100;

    for (uint8_t i = 0; i < bufferLength;)
    {
        getNewData();

        int8_t numberOfSamples = senseBuf.head - senseBuf.tail;
        if (numberOfSamples < 0)
        {
            numberOfSamples += MAX30102_SENSE_BUF_SIZE;
        }

        while (numberOfSamples--)
        {
            redBuffer[i] = senseBuf.red[senseBuf.tail];
            irBuffer[i] = senseBuf.IR[senseBuf.tail];

            senseBuf.tail++;
            senseBuf.tail %= MAX30102_SENSE_BUF_SIZE;
            i++;
            if (i == bufferLength)
                break;
        }
    }

    maxim_heart_rate_and_oxygen_saturation(/**pun_ir_buffer=*/irBuffer, /*n_ir_buffer_length=*/bufferLength, /**pun_red_buffer=*/redBuffer,
                                           /**pn_spo2=*/SPO2, /**pch_spo2_valid=*/SPO2Valid, /**pn_heart_rate=*/heartRate, /**pch_hr_valid=*/heartRateValid);
}

uint32_t LF_Alpha_MAX30102 ::readBPM()
{
    return getRed();
}

float LF_Alpha_MAX30102::readTempC()
{

    uint8_t byteTemp = 0x01;
    writeReg(MAX30102_DIETEMPCONFIG, &byteTemp, 1);

    uint32_t startTime = millis();
    while (millis() - startTime < 100)
    {
        readReg(MAX30102_DIETEMPCONFIG, &byteTemp, 1);
        if ((byteTemp & 0x01) == 0)
            break;
        delay(1);
    }

    uint8_t tempInt;
    readReg(MAX30102_DIETEMPINT, &tempInt, 1);

    uint8_t tempFrac;
    readReg(MAX30102_DIETEMPFRAC, &tempFrac, 1);

    return (float)tempInt + ((float)tempFrac * 0.0625);
}

float LF_Alpha_MAX30102::readTempF()
{
    float temp = readTempC();
    if (temp != -999.0)
        temp = temp * 1.8 + 32.0;
    return (temp);
}

uint32_t LF_Alpha_MAX30102::getRed(void)
{
    getNewData();
    return (senseBuf.red[senseBuf.head]);
}

uint32_t LF_Alpha_MAX30102::getIR(void)
{
    getNewData();
    return (senseBuf.IR[senseBuf.head]);
}

bool LF_Alpha_MAX30102::end(void)
{
    _i2cAddr = 0x00; // Invalidate the sensor address
    return true;
}

// LF_Alpha_WiFI methods definitions
// public methods
LF_Alpha_WiFi ::LF_Alpha_WiFi()
{
    ssid = "";
    password = "";
    wifiStatus = false;
    
    pinMode(userLED2, OUTPUT);
    digitalWrite(userLED2, LOW);
}

bool LF_Alpha_WiFi ::readStatus()
{
    if (ssid.isEmpty() || password.isEmpty())
    {
        return false; // Not connected if ssid or password is empty
    }
    else if (WiFi.status() == WL_CONNECTED)
    {
        wifiStatus = true;
    }
    else
    {
        wifiStatus = false;
    }
    return wifiStatus;
}

String LF_Alpha_WiFi::getIPAddress()
{
    if (readStatus())
    {
        return WiFi.localIP().toString();
    }
    return "";
}

bool LF_Alpha_WiFi::checkInternet()
{
    if (readStatus())
    {
        const char *test_url = "http://www.google.com"; // Test URL
        // HTTPClient http;
        LF_Alpha_HTTPClient http;
        http.begin(test_url);
        // int httpCode = http.GET();
        int httpCode = http.get();
        if (httpCode > 0)
        {
            if (httpCode == 200)
                return true;
            else
                return false;
        }
        else
        {
            return false;
        }
        http.end();
    }
    return false;
}

void LF_Alpha_WiFi::begin()
{
    // do nothing here..
}

void LF_Alpha_WiFi ::begin(const String &SSID, const String &PASSWORD)
{
    connect(SSID, PASSWORD);
}

void LF_Alpha_WiFi ::disconnectWiFi()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    ssid = "";
    password = "";
    wifiStatus = false;
}

void LF_Alpha_WiFi ::configure(wifi_mode_t mode)
{
    WiFi.mode(mode);
}

uint8_t LF_Alpha_WiFi ::getCurrentMode()
{
    return WiFi.getMode();
}

void LF_Alpha_WiFi ::connect(const String &SSID, const String &PASSWORD)
{
    ssid = SSID;
    password = PASSWORD;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    return;
}

bool LF_Alpha_WiFi::startAccessPoint(const String &SSID, const String &PASSWORD)
{
    ssid = SSID;
    password = PASSWORD;
    WiFi.mode(WIFI_AP_STA);
    return WiFi.softAP(ssid.c_str(), password.c_str());
}

bool LF_Alpha_WiFi::stopAccessPoint()
{
    if (WiFi.getMode() == WIFI_AP_STA)
    {
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_OFF);
        ssid = "";
        password = "";
        wifiStatus = false;
        return true;
    }
    return false;
}

bool LF_Alpha_WiFi ::end()
{
    disconnectWiFi();
    return true;
}

// LF_Alpha_HTTPClient definitions
// public methods
bool LF_Alpha_HTTPClient::begin(const String &url)
{
    serverUrl = url;
    http.begin(serverUrl);
    return true; // Return true if initialization is successful
}

bool LF_Alpha_HTTPClient::configure(const String &url)
{
    serverUrl = url;
    http.begin(serverUrl);
    return true; // Return true if initialization is successful
}

int LF_Alpha_HTTPClient::get(const String payload)
{
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.GET();
    if (httpCode > 0)
    {
        return httpCode; // Return HTTP status code
    }
    return -1; // Return -1 on failure
}

int LF_Alpha_HTTPClient::post(const String payload)
{
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(payload);
    if (httpCode > 0)
    {
        return httpCode; // Return HTTP status code
    }
    return -1; // Return -1 on failure
}

int LF_Alpha_HTTPClient::put(const String payload)
{
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.PUT(payload);
    if (httpCode > 0)
    {
        return httpCode; // Return HTTP status code
    }
    return -1; // Return -1 on failure
}

int LF_Alpha_HTTPClient::del(const String payload)
{
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.sendRequest("DELETE", payload);
    if (httpCode > 0)
    {
        return httpCode; // Return HTTP status code
    }
    return -1; // Return -1 on failure
}

void LF_Alpha_HTTPClient::end()
{
    http.end(); // End the HTTP connection
}

String LF_Alpha_HTTPClient::getResponse()
{
    return http.getString(); // Return the response as a String
}

void LF_Alpha_HTTPClient::addHeader(const String &name, const String &value)
{
    http.addHeader(name, value); // Add a header to the HTTP request
}

// LF_Alpha_I2CScanner definitions
// public methods
LF_Alpha_I2CScanner::LF_Alpha_I2CScanner()
{

    if (i2cInitFlag == false)
    {
        i2cInitFlag = true;
        Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL); // Initialize I2C for all instances
    }
    pinMode(userLED2, OUTPUT);
    digitalWrite(userLED2, LOW);
    allAddresses = "";
    nDevices = 0;
}

void LF_Alpha_I2CScanner::scan()
{
    allAddresses = "";
    nDevices = 0;
    for (uint8_t addr = 1; addr < 127; addr++)
    {
        Wire.beginTransmission(addr);
        error = Wire.endTransmission();
        if (error == 0)
        {
            allAddresses += i2cAddressToString(addr) + ", ";
            nDevices++;
        }
        else if (error == 4)
        {
            allAddresses += "Error at:[0x" + i2cAddressToString(addr) + "], ";
        }
    }
}
