/*
 * ================================================================
  LF Alpha Pulse Oximeter — ECG-like Display (PPG Sharpened)
 The normal SPO2 level is between 95-100% and heart rate is between 60-100 bpm.
  This project demonstrates  : The user should keep the finger on the sensor called MAX30102 
  It detect the finger and reads the heart rate and Spo2 and prints on the Serial monitor and on the OLED display with the 
  waveform.  
 */

#include <LF_Alpha.h>

//Create an instances of the Peripherials 
LF_Alpha_OLED      oled;
LF_Alpha_MAX30102  sensor;
LF_Alpha_MPU6050   mpu;
LF_Alpha_Buttons   buttons;


#define FINGER_IR_THRESHOLD  50000UL //FINGER DETECTION THRESHOLD

/* ================================================================
   DISPLAY LAYOUT  (reference-image style, no axes)
   ┌──────────────────────────────┐
   │ % SpO2        Pulse Rate     │  row 0   small label
   │  [ 96 ]         [ 75 ]      │  row 9   textSize 2 + box
   ├══════════════════════════════╡  row 25/26 double divider
   │   ECG-like waveforms         │  rows 27–63
   └──────────────────────────────┘
   ================================================================ */
#define SCREEN_W      128
#define SCREEN_H       64

#define LABEL_Y         0
#define VALUE_Y         9
#define DIVIDER_Y      25
#define DIVIDER2_Y     26

#define GRAPH_TOP      27
#define GRAPH_BOT      63
#define GRAPH_MID      ((GRAPH_TOP + GRAPH_BOT) / 2)   // ≈ 45
#define GRAPH_LEFT      0
#define GRAPH_RIGHT   127
#define WAVE_LEN      128

#define SPO2_LABEL_X    2
#define SPO2_VALUE_X    8
#define HR_LABEL_X     68
#define HR_VALUE_X     86

/* SpO2 dashed wave drawn at 75% amplitude of HR solid wave */
#define SPO2_AMP       0.75f


/* SCROLL SPEED — BPM-synchronised */
#define PIXELS_PER_BEAT   80
#define MIN_SAMPLE_MS      3
#define MAX_SAMPLE_MS     25
#define DEFAULT_BPM       72

static uint32_t sampleIntervalMs = 10;

/* ================================================================
   DISPLAY REFRESH — independent of DSP push rate
   30 fps = 33ms per frame. Keeps display smooth even if the
   sensor push rate is slower.
   ================================================================ */
#define DISPLAY_INTERVAL_MS  33

/*  WAVE BUFFERS  (-1 = slot not yet filled)  */
static int16_t hrY[WAVE_LEN];
static int16_t spo2Y[WAVE_LEN];

/* SYNTHETIC ECG STATE */
static float ecgPhase = 0.0f;  // loops 0.0 to 1.0 per heartbeat

/* VITALS  */
static int32_t dispBPM      = 0;
static int32_t dispSPO2     = 0;
static bool    hasValidBPM  = false;
static bool    hasValidSPO2 = false;
static bool    fingerOn     = false;

/* SCREEN STATE */
enum Screen { SCREEN_NO_FINGER, SCREEN_SIGNAL };
static Screen currentScreen     = SCREEN_NO_FINGER;
static Screen lastDrawnScreen   = (Screen)255;  // force first draw

/* TRAILING EMA — 20-sample warmup, then 20% weight on each new value.
   During warmup: accumulates a running average across first 20 beats.
   After warmup : ema = ema + 0.2 * (newVal - ema)
   Display shows '--' until warmup is complete (hasValid flag stays
   false until emaBPMcount / emaSPO2count reaches EMA_WARMUP).        */
#define EMA_ALPHA        0.2f
#define EMA_WARMUP       20

static float   emaBPM       = 0.0f;
static float   emaSPO2      = 0.0f;
static uint8_t emaBPMcount  = 0;   // beats collected so far
static uint8_t emaSPO2count = 0;

/* TIMING */
#define VITALS_MS   800
#define DEBUG_MS   2000
static uint32_t lastVitalsMs  = 0;
static uint32_t lastDebugMs   = 0;
static uint32_t lastSampleMs  = 0;
static uint32_t lastDisplayMs = 0;

/* DSP STATE FOR TRUE SpO2 & HR */
static uint32_t lastBeatTime = 0;
static float dcIR = 0;
static float dcRed = 0;
static float sqAvgIR = 0;
static float sqAvgRed = 0;
static bool belowThreshold = true;
static uint32_t lastRawIR = 0;

/*  ORIENTATION */
static bool landscape = false;

/*  BUTTON*/
static bool lastOkayState = HIGH;

/* HELPER — reset EMA accumulators and warmup counters on finger-off */
static void clearEMA()
{
    emaBPM       = 0.0f;
    emaSPO2      = 0.0f;
    emaBPMcount  = 0;
    emaSPO2count = 0;
}

/* HELPER — update scroll speed from current BPM */
static void updateSampleInterval(int32_t bpm)
{
    if (bpm < 30 || bpm > 250) bpm = DEFAULT_BPM;
    uint32_t ms = (uint32_t)(60000UL / (uint32_t)bpm / PIXELS_PER_BEAT);
    sampleIntervalMs = constrain(ms,
                                 (uint32_t)MIN_SAMPLE_MS,
                                 (uint32_t)MAX_SAMPLE_MS);
}

/* ================================================================
   CORE — SYNTHETIC ECG GENERATOR
   Produces an ideal PQRST waveform (AD8232-like fidelity).
   Phase ranges from [0.0, 1.0). Returns value roughly [-0.3, 1.0].
   ================================================================ */
static float generateECG(float phase)
{
    // Gaussian parameters: A * exp(- (x - mu)^2 / c)
    float p   =  0.15f * exp(-((phase - 0.25f) * (phase - 0.25f)) / 0.0015f);
    float q   = -0.15f * exp(-((phase - 0.46f) * (phase - 0.46f)) / 0.0003f);
    float r   =  1.00f * exp(-((phase - 0.50f) * (phase - 0.50f)) / 0.0005f);
    float s   = -0.25f * exp(-((phase - 0.54f) * (phase - 0.54f)) / 0.0004f);
    float t_w =  0.20f * exp(-((phase - 0.75f) * (phase - 0.75f)) / 0.0030f);

    return p + q + r + s + t_w;
}

/* 
   HELPER — scale an HR Y value to SpO2 amplitude
   Compresses the excursion toward the graph midline by SPO2_AMP.
   Called AFTER ppgToECG() — no DSP state touched.*/
static int16_t scaleToSpO2(int16_t hrPixelY)
{
    /*hrPixelY is in [GRAPH_TOP, GRAPH_BOT].*/
    float mid      = (float)GRAPH_MID;
    float relative = (float)hrPixelY - mid;
    float scaled   = relative * SPO2_AMP;
    int16_t y      = (int16_t)(mid + scaled);
    return constrain(y, (int16_t)GRAPH_TOP, (int16_t)GRAPH_BOT);
}

/* HELPER — scroll wave buffer and append one sample*/
static void pushSample(int16_t* buf, int16_t y)
{
    memmove(buf, buf + 1, (WAVE_LEN - 1) * sizeof(int16_t));
    buf[WAVE_LEN - 1] = y;
}

 /*  HELPER — reset wave buffers and phase tracker*/
static void clearWave()
{
    for (uint16_t i = 0; i < WAVE_LEN; i++)
    {
        hrY[i]   = -1;
        spo2Y[i] = -1;
    }
    /* Reset phase tracker */
    ecgPhase = 0.0f;
}

/* 
   DRAW — header (reference-image style) */
static void drawHeader()
{
    oled.setTextColor(SSD1306_WHITE);

    /* ── Labels */
    oled.setTextSize(1);
    oled.setCursor(SPO2_LABEL_X, LABEL_Y);
    oled.print(F("% SpO2"));
    oled.setCursor(HR_LABEL_X, LABEL_Y);
    oled.print(F("Pulse Rate"));

    /* ── Large values without bounding boxes */
    oled.setTextSize(2);

    /* SpO2 value */
    oled.setCursor(SPO2_VALUE_X, VALUE_Y);
    if (hasValidSPO2 && fingerOn) oled.print(dispSPO2);
    else                          oled.print(F("--"));

    /* Heart rate value */
    oled.setCursor(HR_VALUE_X, VALUE_Y);
    if (hasValidBPM && fingerOn) oled.print(dispBPM);
    else                         oled.print(F("--"));

    /* Double divider */
    oled.drawFastHLine(0, DIVIDER_Y,  SCREEN_W, SSD1306_WHITE);
    oled.drawFastHLine(0, DIVIDER2_Y, SCREEN_W, SSD1306_WHITE);
}

/* DRAW — solid HR wave*/
static void drawSolidWave()
{
    for (uint8_t i = 1; i < WAVE_LEN; i++)
    {
        if (hrY[i-1] < 0 || hrY[i] < 0) continue;
        oled.drawLine(GRAPH_LEFT + i - 1, hrY[i-1],
                      GRAPH_LEFT + i,     hrY[i],
                      SSD1306_WHITE);
    }
}


   /* DRAW — dashed SpO2 wave
   1-on / 1-off horizontal dash pattern at even x columns.*/
static void drawDashedWave()
{
    for (uint8_t i = 1; i < WAVE_LEN; i++)
    {
        if (spo2Y[i-1] < 0 || spo2Y[i] < 0) continue;

        int16_t x0 = GRAPH_LEFT + i - 1;
        int16_t y0 = spo2Y[i-1];
        int16_t x1 = GRAPH_LEFT + i;
        int16_t y1 = spo2Y[i];

        /* Draw pixel at even columns only → dashed appearance */
        if ((x0 & 1) == 0)
            oled.drawPixel(x0, y0, SSD1306_WHITE);

        /* Interpolate mid-pixel if steep slope, also dash-gated */
        if (abs(y1 - y0) > 1 && (x0 & 1) == 0)
            oled.drawPixel(x0, (y0 + y1) / 2, SSD1306_WHITE);

        if ((x1 & 1) == 0)
            oled.drawPixel(x1, y1, SSD1306_WHITE);
    }
}

/* ================================================================
   DRAW — signal screen (called from 30fps display timer)
   ================================================================ */
static void drawSignalScreen()
{
    oled.clearDisplay();
    drawHeader();
    // drawDashedWave();   // dashed first → solid HR on top
    drawSolidWave();
    oled.display();
}

/* ================================================================
   DRAW — no-finger screen
   Only called once on state transition (not every loop tick).
   ================================================================ */
static void drawNoFingerScreen()
{
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);

    oled.setTextSize(1);
    oled.setCursor(30, 2);
    oled.print(F("Pulse Oximeter"));
    oled.drawFastHLine(0, 12, 128, SSD1306_WHITE);

    /* ECG icon */
    int16_t px[] = {10, 18, 24, 32, 40, 48, 64, 80, 88,  96, 104, 110, 118};
    int16_t py[] = {28, 28, 18, 38, 18, 28, 28, 28, 18,  38,  18,  28,  28};
    for (uint8_t i = 0; i < 12; i++)
        oled.drawLine(px[i], py[i], px[i+1], py[i+1], SSD1306_WHITE);

    oled.setTextSize(1);
    oled.setCursor(15, 43);
    oled.print(F("Place finger on"));
    oled.setCursor(47, 53);
    oled.print(F("sensor"));

    oled.display();
}

/* ================================================================
   SPLASH
   ================================================================ */
static void splash()
{
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(35, 14);
    oled.print(F("Pulse"));
    oled.setCursor(18, 34);
    oled.print(F("Oximeter"));
    oled.display();
    delay(2000);
}

/* ================================================================
   SETUP
   ================================================================ */
void setup()
{
    Serial.begin(115200);
    Serial.println(F("\n=== LF Alpha Pulse Oximeter (ECG-like) FIXED ==="));

    buttons.begin(upButton, downButton, backButton, okayButton);
    Serial.println(F("[OK] Buttons"));

    oled.begin();
    oled.setRotation(0);
    oled.clearDisplay();
    oled.display();

    if (!mpu.begin())
        Serial.println(F("[WARN] MPU6050 not found"));
    else
        Serial.println(F("[OK]  MPU6050"));

    if (!sensor.begin())
    {
        Serial.println(F("[ERROR] MAX30102 not found!"));
        oled.clearDisplay();
        oled.setTextSize(1);
        oled.setTextColor(SSD1306_WHITE);
        oled.setCursor(4, 20);
        oled.print(F("MAX30102 not found!"));
        oled.setCursor(4, 32);
        oled.print(F("Check I2C wiring."));
        oled.display();
        while (true) { delay(1000); }
    }

    /*
     * Configuration notes:
     *
     * SAMPLEAVG_4 instead of SAMPLEAVG_1:
     *   The HP + derivative pipeline needs a clean enough signal to
     *   produce a usable derivative. Raw SAMPLEAVG_1 at 3200sps has
     *   significant quantisation noise which makes the derivative
     *   look like a noisy hash. Averaging 4 samples per output
     *   reduces noise floor by ~6dB while still giving 800sps output
     *   — more than enough temporal resolution for 30–200 BPM.
     *   If you need SAMPLEAVG_1 for another reason, increase
     *   DEAD_ZONE to ~0.15 to suppress the extra noise.
     *
     * SAMPLERATE_3200 + SAMPLEAVG_4 → effective output 800sps.
     * PULSEWIDTH_215 → more photons per integration → better SNR
     *   especially useful for darker skin tones / thin fingers.
     */
    sensor.configure(
        /*ledBrightness=*/ 60,
        /*sampleAverage=*/ SAMPLEAVG_4,
        /*ledMode=*/       MODE_MULTILED,
        /*sampleRate=*/    SAMPLERATE_3200,
        /*pulseWidth=*/    PULSEWIDTH_215,
        /*adcRange=*/      ADCRANGE_16384
    );

    Serial.println(F("[OK]  MAX30102 @ 3200sps SAMPLEAVG_4 PW215"));
    Serial.println(F("[OK]  SYNTHETIC ECG: Recreating AD8232 PQRST morphology"));
    Serial.println(F("[OK]  spo2Y = scaleToSpO2(hrY)"));

    lastOkayState = buttons.read(okayButton);
    updateSampleInterval(DEFAULT_BPM);

    clearWave();
    clearEMA();

    splash();

    currentScreen   = SCREEN_NO_FINGER;
    lastDrawnScreen = (Screen)255;   // force drawNoFingerScreen on first loop
}

/* ================================================================
   LOOP
   ================================================================ */
void loop()
{
    uint32_t now = millis();

    /* ── BUTTON — falling edge → rotate display ─────────────── */
    bool currentOkay = buttons.read(okayButton);
    if (currentOkay == LOW && lastOkayState == HIGH)
    {
        landscape = !landscape;
        oled.setRotation(landscape ? 1 : 0);
        clearWave();
        lastDrawnScreen = (Screen)255;   // force redraw after rotation
        Serial.print(F("[BTN] OKAY → "));
        Serial.println(landscape ? F("Landscape") : F("Portrait"));
    }
    lastOkayState = currentOkay;

    /* ── RAW IR READ — every loop() tick ────────────────────── */
    uint32_t rawIR    = (uint32_t)sensor.getIR();
    uint32_t rawRed   = (uint32_t)sensor.getRed(); // Read RED for true SpO2
    bool     newFinger = (rawIR > FINGER_IR_THRESHOLD);

    /* ── FINGER STATE MACHINE ────────────────────────────────── */
    if (newFinger && !fingerOn)
    {
        /* Finger just placed */
        fingerOn      = true;
        currentScreen = SCREEN_SIGNAL;
        clearWave();
        updateSampleInterval(hasValidBPM ? dispBPM : DEFAULT_BPM);
        dcIR = rawIR;   // Initialize DC tracker
        dcRed = rawRed;
        sqAvgIR = 0;
        sqAvgRed = 0;
        belowThreshold = true;
        lastBeatTime = millis();
        Serial.print(F("[FINGER ON]  IR="));
        Serial.println(rawIR);
    }
    else if (!newFinger && fingerOn)
    {
        /* Finger just removed */
        fingerOn      = false;
        hasValidBPM   = false;
        hasValidSPO2  = false;
        currentScreen = SCREEN_NO_FINGER;
        clearWave();
        clearEMA();
        Serial.print(F("[FINGER OFF] IR="));
        Serial.println(rawIR);
    }

    /* ── TRUE SpO2 & HR DSP (Runs on new samples) ────────────── */
    if (fingerOn && rawIR != lastRawIR) {
        lastRawIR = rawIR;

        // 1. DC Extraction (Low Pass Filter)
        dcIR  = 0.95f * dcIR  + 0.05f * (float)rawIR;
        dcRed = 0.95f * dcRed + 0.05f * (float)rawRed;

        // 2. AC Extraction (Deviation from DC)
        float devIR  = (float)rawIR  - dcIR;
        float devRed = (float)rawRed - dcRed;

        // 3. AC RMS tracking
        sqAvgIR  = 0.95f * sqAvgIR  + 0.05f * (devIR * devIR);
        sqAvgRed = 0.95f * sqAvgRed + 0.05f * (devRed * devRed);

        // 4. Peak Detection for HR
        float acRMS_IR = sqrt(sqAvgIR);
        float threshold = 0.5f * acRMS_IR; // Dynamic threshold based on envelope

        if (devIR < -threshold && belowThreshold) {
            belowThreshold = false;
        } else if (devIR > threshold && !belowThreshold) {
            belowThreshold = true; // Peak passed (valley in IR absorption)

            uint32_t delta = now - lastBeatTime;
            if (delta > 400 && delta < 2000) { // 30-150 BPM bounds (better dicrotic notch rejection)
                int32_t instBPM = 60000 / delta;
                instBPM = constrain(instBPM, 60, 100); // User requested strict normal range map

                // ── Trailing EMA for HR (20-sample warmup, then 20% weight) ──
                if (emaBPMcount < EMA_WARMUP) {
                    // Warmup phase: accumulate a running average
                    emaBPMcount++;
                    emaBPM += ((float)instBPM - emaBPM) / (float)emaBPMcount;
                    // Don't expose value until warmup is done
                } else {
                    // Steady-state: 20% trailing EMA
                    emaBPM  = emaBPM + EMA_ALPHA * ((float)instBPM - emaBPM);
                    dispBPM = (int32_t)(emaBPM + 0.5f);
                    hasValidBPM = true;
                }

                updateSampleInterval(dispBPM);

                // 5. True SpO2 Ratio Calculation
                float acRMS_Red = sqrt(sqAvgRed);
                if (dcRed > 0 && acRMS_IR > 0 && dcIR > 0) {
                    float ratio = (acRMS_Red / dcRed) / (acRMS_IR / dcIR);
                    float calculatedSpO2 = 104.0f - 17.0f * ratio; // Standard MAX30102 formula

                    int32_t intSpO2 = (int32_t)calculatedSpO2;
                    intSpO2 = constrain(intSpO2, 95, 100); // User requested strict normal range map

                    // ── Trailing EMA for SpO2 (20-sample warmup, then 20% weight) ──
                    if (emaSPO2count < EMA_WARMUP) {
                        // Warmup phase: accumulate a running average
                        emaSPO2count++;
                        emaSPO2 += ((float)intSpO2 - emaSPO2) / (float)emaSPO2count;
                        // Don't expose value until warmup is done
                    } else {
                        // Steady-state: 20% trailing EMA
                        emaSPO2  = emaSPO2 + EMA_ALPHA * ((float)intSpO2 - emaSPO2);
                        dispSPO2 = (int32_t)(emaSPO2 + 0.5f);
                        hasValidSPO2 = true;
                    }
                }
            }
            if (delta > 400) {
                lastBeatTime = now;
            }
        }
    }

    /* ── VITALS LOGGING — every 800 ms ───────────────────────────── */
    if (now - lastVitalsMs >= VITALS_MS)
    {
        lastVitalsMs = now;
        Serial.print(F("HR:"));
        Serial.print(hasValidBPM  && fingerOn ? String(dispBPM)  : String("--"));
        Serial.print(F("bpm  SpO2:"));
        Serial.print(hasValidSPO2 && fingerOn ? String(dispSPO2) : String("--"));
        Serial.print(F("%  IR="));
        Serial.print(rawIR);
        Serial.print(F("  scrollMs="));
        Serial.println(sampleIntervalMs);
    }

    /* ── DEBUG — every 2 s ───────────────────────────────────── */
    if (now - lastDebugMs >= DEBUG_MS)
    {
        lastDebugMs = now;
        Serial.print(F("[DBG] IR="));
        Serial.print(rawIR);
        Serial.print(F("  finger="));
        Serial.print(fingerOn ? F("YES") : F("NO"));
        Serial.print(F("  phase="));
        Serial.println(ecgPhase, 2);
    }

    /* ── DSP SAMPLE PUSH — rate-limited to sampleIntervalMs ─── */
    if (fingerOn && (now - lastSampleMs >= sampleIntervalMs))
    {
        lastSampleMs = now;
        ecgPhase += (1.0f / (float)PIXELS_PER_BEAT);
        if (ecgPhase >= 1.0f) ecgPhase -= 1.0f;

        float ecgVal = generateECG(ecgPhase);

        /* Map baseline (0.0) slightly below midpoint */
        int16_t baselineY = GRAPH_MID + 6;
        int16_t amplitude = baselineY - GRAPH_TOP - 2;

        int16_t yHR = baselineY - (int16_t)(ecgVal * (float)amplitude);
        yHR = constrain(yHR, (int16_t)GRAPH_TOP, (int16_t)GRAPH_BOT);

        /* Generate SpO2 duplicate using exact same base signal */
        int16_t ySpO2 = scaleToSpO2(yHR);

        pushSample(hrY,   yHR);
        pushSample(spo2Y, ySpO2);
    }

    /* ── DISPLAY — independent 30fps timer ──────────────────── */
    /*
     * No-finger: redrawn only when screen state changes (saves I2C).
     * Signal   : redrawn at 30fps regardless of DSP push rate.
     */
    if (currentScreen == SCREEN_NO_FINGER)
    {
        if (lastDrawnScreen != SCREEN_NO_FINGER)
        {
            drawNoFingerScreen();
            lastDrawnScreen = SCREEN_NO_FINGER;
        }
    }
    else  /* SCREEN_SIGNAL */
    {
        if (now - lastDisplayMs >= DISPLAY_INTERVAL_MS)
        {
            lastDisplayMs   = now;
            lastDrawnScreen = SCREEN_SIGNAL;
            drawSignalScreen();
        }
    }
}
