#include <TFT_eSPI.h>
#include <SPI.h>

#include "bluetooth.ino"
#include <defines.h>

TFT_eSPI tft = TFT_eSPI();

// ===== DISPLAY FUNCTIONS =====
void drawHeader(const char* title) {
    tft.fillRect(0, 0, 170, HEADER_HEIGHT, HEADER_BG);
    tft.setTextColor(HEADER_TEXT);
    tft.setTextSize(1);
    tft.setFreeFont(0);  // Use default font
    tft.drawString(title, PADDING, 10, 2);
    
    // Draw a subtle line under header
    tft.drawLine(0, HEADER_HEIGHT-1, 170, HEADER_HEIGHT-1, 0x2A2A4A);
}

void drawScanningAnimation(int progress) {
    // Animated dots while scanning
    tft.fillRect(0, BODY_TOP, 170, BODY_HEIGHT, BG_COLOR);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("Scanning", 30, BODY_TOP + 40, 2);
    
    // Animated dots
    for (int i = 0; i < (progress % 4); i++) {
        tft.fillCircle(80 + (i * 20), BODY_TOP + 80, 4, TFT_CYAN);
    }
    
    // Progress bar
    int barWidth = map(progress, 0, 100, 0, 150);
    tft.drawRect(10, BODY_TOP + 100, 150, 10, TFT_WHITE);
    tft.fillRect(10, BODY_TOP + 100, barWidth, 10, TFT_CYAN);
}

void drawDeviceCard(String name, String address, int rssi, String uuid) {
    tft.fillRect(0, BODY_TOP, 170, BODY_HEIGHT, BG_COLOR);
    
    // Device card background
    tft.fillRoundRect(5, BODY_TOP + 5, 160, 180, 8, DEVICE_CARD_BG);
    
    int y = BODY_TOP + 20;
    
    // Status indicator (connected/found)
    tft.fillCircle(20, y + 5, 6, TFT_GREEN);
    tft.setTextColor(TFT_GREEN);
    tft.drawString("Device Found!", 35, y, 2);
    
    y += 30;
    
    // Device name
    tft.setTextColor(DEVICE_NAME_COLOR);
    tft.setTextSize(1);
    tft.setFreeFont(0);
    tft.drawString(name, PADDING + 5, y, 4);
    
    y += 25;
    
    // RSSI with color coding
    tft.setTextColor(DEVICE_INFO_COLOR);
    tft.drawString("Signal:", PADDING + 5, y, 2);
    
    // Color-code RSSI
    uint16_t rssiColor;
    if (rssi > -50) rssiColor = RSSI_GOOD;
    else if (rssi > -70) rssiColor = RSSI_OK;
    else rssiColor = RSSI_BAD;
    
    tft.setTextColor(rssiColor);
    tft.drawString(String(rssi) + " dBm", 80, y, 2);
    
    y += 25;
    
    // Signal strength bar
    tft.setTextColor(DEVICE_INFO_COLOR);
    tft.drawString("Strength:", PADDING + 5, y, 2);
    
    int barLength = map(rssi, -100, -20, 10, 140);
    if (barLength < 10) barLength = 10;
    if (barLength > 140) barLength = 140;
    
    tft.drawRect(80, y + 2, 75, 8, TFT_WHITE);
    tft.fillRect(80, y + 2, barLength, 8, rssiColor);
    
    y += 30;
    
    // Address (truncated)
    tft.setTextColor(DEVICE_INFO_COLOR);
    tft.drawString("MAC:", PADDING + 5, y, 2);
    tft.setTextColor(0x888888);
    tft.drawString(address.substring(0, 17), 50, y, 2);
    
    y += 25;
    
    // Service UUID (for MIDI)
    tft.setTextColor(DEVICE_INFO_COLOR);
    tft.drawString("Service:", PADDING + 5, y, 2);
    
    if (uuid.length() > 0) {
        bool isMIDI = (uuid.indexOf("03b80e5a") >= 0);
        tft.setTextColor(isMIDI ? TFT_CYAN : TFT_WHITE);
        String shortUUID = uuid.substring(0, 8);
        tft.drawString(shortUUID + "...", 75, y, 2);
        
        y += 20;
        if (isMIDI) {
            tft.setTextColor(TFT_CYAN);
            tft.drawString("MIDI Device ✓", PADDING + 5, y, 2);
        }
    }
    
    // Bottom instruction
    y += 40;
    tft.setTextColor(0x666666);
    tft.drawString("Press RST to scan again", 15, y, 2);
}

void drawNoDeviceFound() {
    tft.fillRect(0, BODY_TOP, 170, BODY_HEIGHT, BG_COLOR);
    tft.setTextColor(0xFF4444);
    tft.setTextSize(1);
    tft.drawString("No device found", 25, BODY_TOP + 60, 2);
    
    tft.setTextColor(0x888888);
    tft.drawString("Make sure keyboard", 20, BODY_TOP + 90, 2);
    tft.drawString("is in pairing mode", 25, BODY_TOP + 110, 2);
    tft.drawString("(BT LED blinking)", 25, BODY_TOP + 130, 2);
    
    tft.drawString("Press RST to retry", 30, BODY_TOP + 170, 2);
}

// ===== SETUP =====
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\n=== BLE MIDI Device Scanner ===\n");
    
    // Initialize display
    tft.init();
    tft.setRotation(1);  // Landscape? Adjust as needed
    tft.fillScreen(BG_COLOR);
    
    drawHeader("BLE MIDI Scanner");
    
    // Initialize BLE
    BLEDevice::init("T-Display-Scanner");
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);  // Active scan uses more power but finds devices faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    
    // Start scanning
    startScan();
}

void startScan() {
    Serial.println("Starting BLE scan...");
    deviceFound = false;
    isScanning = true;
    
    // Show scanning animation
    for (int i = 0; i < 100; i += 5) {
        drawScanningAnimation(i);
        delay(50);
    }
    
    pBLEScan->start(SCAN_TIME, false);
}

// ===== LOOP =====
void loop() {
    // If scanning is complete and no device found
    if (!isScanning && !deviceFound) {
        drawNoDeviceFound();
        delay(5000);
        startScan();  // Retry
    }
    
    // If device found, just hold the display
    if (deviceFound) {
        drawDeviceCard(foundName, foundAddress, foundRSSI, foundUUID);
        delay(100);  // Small delay to prevent flickering
    }
    
    // Optional: Press BOOT button (IO0) to re-scan
    // Uncomment if you want manual re-scan capability
    /*
    if (digitalRead(0) == LOW) {
        delay(200);  // Debounce
        startScan();
    }
    */
}