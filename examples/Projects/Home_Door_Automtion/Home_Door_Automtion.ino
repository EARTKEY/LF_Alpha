/**
 * ================================================================
 *  ESP32 BLE Dual Relay Controller
 *  Board   : EARTKEY LF Alpha (ESP32-S3 Mini-1)
 *  Library : LF_Alpha (for relays) + ESP32 built-in BLE
 *            (BLEDevice.h ships WITH the ESP32 board package —
 *             NO extra library install required)
 * ================================================================
 *
 *  ► BOARD SELECT IN ARDUINO IDE
 *  ──────────────────────────────────────────────────────────────
 *  Tools → Board   → ESP32 Arduino → "ESP32S3 Dev Module"
 *  Tools → Port    → (your COM port)
 *  Tools → Upload Speed → 921600
 *
 *  ► LF Alpha PIN REFERENCE (from official repo)
 *  ──────────────────────────────────────────────────────────────
 *  Relay 1  →  GPIO 36   (Active HIGH)
 *  Relay 2  →  GPIO 35   (Active HIGH)
 *
 *  ► BLE COMMAND SET  (send from nRF Connect or any BLE terminal)
 *  ──────────────────────────────────────────────────────────────
 *  "R1:ON"   → Relay 1 ON
 *  "R1:OFF"  → Relay 1 OFF
 *  "R2:ON"   → Relay 2 ON
 *  "R2:OFF"  → Relay 2 OFF
 *  "ALL:ON"  → Both Relays ON
 *  "ALL:OFF" → Both Relays OFF
 *  "STATUS"  → Read current relay states
 *
 *  ► BLE RESPONSE (received as Notify on TX characteristic)
 *  ──────────────────────────────────────────────────────────────
 *  "OK:R1=ON,R2=OFF"   → after valid command
 *  "ERR:UNKNOWN_CMD"   → on unrecognised input
 *
 *  ► RECOMMENDED BLE TEST APPS
 *  ──────────────────────────────────────────────────────────────
 *  Android : nRF Connect  |  Serial Bluetooth Terminal
 *  iOS     : nRF Connect  |  LightBlue
 * ================================================================
 */

// ── LF_Alpha for relay hardware control ─────────────────────────
#include <LF_Alpha.h>

// ── ESP32 built-in BLE (part of ESP32 board package, NOT external)
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ================================================================
//  BLE UUIDs  (standard Nordic UART Service UUIDs)
// ================================================================
#define SERVICE_UUID   "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHAR_RX_UUID   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"  // App → ESP32
#define CHAR_TX_UUID   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"  // ESP32 → App

// ================================================================
//  Hardware: LF Alpha relay pins (confirmed from official README)
// ================================================================
#define RELAY1_PIN  36
#define RELAY2_PIN  35

// ================================================================
//  Objects
// ================================================================
LF_Alpha_Relays relays;           // LF_Alpha relay controller

BLEServer*         pServer  = nullptr;
BLECharacteristic* pTxChar  = nullptr;
bool bleConnected            = false;

// ================================================================
//  Relay state tracking
// ================================================================
bool relay1State = false;
bool relay2State = false;

// ================================================================
//  Relay control — uses LF_Alpha_Relays::write(pin, state)
// ================================================================

void setRelay1(bool on) {
    relay1State = on;
    relays.write(RELAY1_PIN, on ? HIGH : LOW);
}

void setRelay2(bool on) {
    relay2State = on;
    relays.write(RELAY2_PIN, on ? HIGH : LOW);
}

// ================================================================
//  Send BLE notification back to connected app
// ================================================================

void sendStatus(const char* prefix = "OK") {
    if (!bleConnected || pTxChar == nullptr) return;

    char buf[32];
    snprintf(buf, sizeof(buf), "%s:R1=%s,R2=%s",
             prefix,
             relay1State ? "ON" : "OFF",
             relay2State ? "ON" : "OFF");

    pTxChar->setValue((uint8_t*)buf, strlen(buf));
    pTxChar->notify();

    Serial.print("[BLE TX] ");
    Serial.println(buf);
}

// ================================================================
//  BLE Server Callbacks — connection events
// ================================================================

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pSvr) override {
        bleConnected = true;
        Serial.println("[BLE] Client connected");
    }
    void onDisconnect(BLEServer* pSvr) override {
        bleConnected = false;
        Serial.println("[BLE] Client disconnected — restarting advertising");
        BLEDevice::startAdvertising();
    }
};

// ================================================================
//  BLE RX Callback — command handler
// ================================================================

class RxCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override {

        String cmd = pChar->getValue().c_str();
        cmd.trim();   // remove any \r \n whitespace

        Serial.print("[BLE RX] ");
        Serial.println(cmd);

        // ── Command Dispatch ────────────────────────────────────
        if      (cmd == "R1:ON")   { setRelay1(true);               sendStatus(); }
        else if (cmd == "R1:OFF")  { setRelay1(false);              sendStatus(); }
        else if (cmd == "R2:ON")   { setRelay2(true);               sendStatus(); }
        else if (cmd == "R2:OFF")  { setRelay2(false);              sendStatus(); }
        else if (cmd == "ALL:ON")  { setRelay1(true);  setRelay2(true);  sendStatus(); }
        else if (cmd == "ALL:OFF") { setRelay1(false); setRelay2(false); sendStatus(); }
        else if (cmd == "STATUS")  { sendStatus(); }
        else {
            if (bleConnected && pTxChar) {
                const char* err = "ERR:UNKNOWN_CMD";
                pTxChar->setValue((uint8_t*)err, strlen(err));
                pTxChar->notify();
            }
            Serial.println("[BLE] Unknown command received");
        }
    }
};

// ================================================================
//  BLE Initialisation
// ================================================================

void initBLE() {
    BLEDevice::init("LF-Alpha-Relay");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    BLEService* pService = pServer->createService(SERVICE_UUID);

    // TX Characteristic: ESP32 → App (Notify)
    pTxChar = pService->createCharacteristic(
        CHAR_TX_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pTxChar->addDescriptor(new BLE2902());

    // RX Characteristic: App → ESP32 (Write)
    BLECharacteristic* pRxChar = pService->createCharacteristic(
        CHAR_RX_UUID,
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_WRITE_NR
    );
    pRxChar->setCallbacks(new RxCallbacks());

    pService->start();

    BLEAdvertising* pAdv = BLEDevice::getAdvertising();
    pAdv->addServiceUUID(SERVICE_UUID);
    pAdv->setScanResponse(true);
    pAdv->setMinPreferred(0x06);   // iOS compatibility
    BLEDevice::startAdvertising();

    Serial.println("[BLE] Advertising as: LF-Alpha-Relay");
}

// ================================================================
//  Arduino Setup
// ================================================================

void setup() {
    Serial.begin(115200);
    delay(300);
    Serial.println("\n=== LF Alpha BLE Relay Controller ===");

    // Init relays with both GPIO pins
    relays.begin(RELAY1_PIN, RELAY2_PIN);
    Serial.printf("[OK] Relays init: GPIO%d (R1), GPIO%d (R2)\n",
                  RELAY1_PIN, RELAY2_PIN);

    // Safety: both relays OFF at boot
    setRelay1(false);
    setRelay2(false);
    Serial.println("[OK] Both relays OFF");

    // Start BLE
    initBLE();

    Serial.println("[SYS] Ready — waiting for BLE connection...\n");
}

// ================================================================
//  Arduino Loop — BLE runs on its own FreeRTOS task, no polling needed
// ================================================================

void loop() {
    delay(1000);   // Nothing to do here — BLE callbacks handle everything
}
