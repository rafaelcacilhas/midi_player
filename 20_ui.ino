
struct Layout {
    // Screen
    static const int W = 320;
    static const int H = 170;
    
    // Header
    static const int HEADER_H = 20;
    static const int HEADER_PAD = 12;
    
    // Body     
    static const int BODY_Y = HEADER_H;
    static const int BODY_PAD = HEADER_PAD;
    static const int BODY_W = W - (BODY_PAD * 2);
    
    // Card
    static const int CARD_Y() { return BODY_Y + 3*BODY_PAD; } 
    static const int CARD_H = 40;
    static const int CARD_PAD = BODY_PAD;
    static const int CARD_W = BODY_W;
    
    // Card elements
    static const int CARD_CONTENT_X() { return BODY_PAD + CARD_PAD; }
    static const int CARD_CONTENT_W() { return CARD_W - (CARD_PAD * 2); }
    
    // Spacing
    static const int GAP_SMALL = 8;
    static const int GAP_MEDIUM = 14;
    static const int GAP_LARGE = 25;
};

void drawBluetoothIcon() {
    uint16_t iconColor;
    switch (connectionState) {
        case SCANNING:     iconColor = C_YELLOW; break;
        case CONNECTING:   iconColor = C_YELLOW; break;
        case CONNECTED:    iconColor = C_GREEN;  break;
        case FAILED:       iconColor = C_RED;    break;
    }

    bluetoothIcon.fillSprite(C_HEADER_BG); 
    
    bluetoothIcon.drawLine(8, 1, 8, 14, iconColor);
    bluetoothIcon.drawLine(7, 1, 7, 14, iconColor);

    bluetoothIcon.drawLine(2, 3, 8, 9, iconColor);
    bluetoothIcon.drawLine(3, 3, 12, 12, iconColor);
    bluetoothIcon.drawLine(2, 4, 5, 7, iconColor);
    bluetoothIcon.drawLine(8, 7, 12, 11, iconColor);

    bluetoothIcon.drawLine(2, 12, 7, 7, iconColor);
    bluetoothIcon.drawLine(2, 13, 8, 7, iconColor);
    bluetoothIcon.drawLine(3, 13, 9, 7, iconColor);

    bluetoothIcon.drawLine(8, 1, 12, 4, iconColor);
    bluetoothIcon.drawLine(9, 1, 12, 5, iconColor);
    bluetoothIcon.drawLine(8, 2, 12, 6, iconColor);

    bluetoothIcon.drawLine(10, 8, 12, 10, iconColor);
    bluetoothIcon.fillRect(10,6,2,2,    iconColor);

    bluetoothIcon.fillRect(8,14,2,2,iconColor);
    bluetoothIcon.fillRect(9,13,2,2,iconColor);
    bluetoothIcon.fillRect(10,12,2,2,iconColor);
}

void drawUI(ConnectionState connectionState) {
    tft.fillScreen(C_BG);
   
    // === HEADER ===
    tft.fillRect(0, 0, Layout::W, Layout::HEADER_H, C_HEADER_BG);
    tft.setTextColor(C_WHITE);
    tft.drawString("BLE MIDI Scanner", Layout::HEADER_PAD, 2, 2);
    drawBluetoothIcon();
    bluetoothIcon.pushSprite(Layout::W - 16 - Layout::GAP_SMALL/2, Layout::GAP_SMALL/4, C_HEADER_BG );
    
    int y = Layout::BODY_Y + Layout::BODY_PAD;
    
    if (deviceFound) {
        // Status dot + text
        tft.fillCircle(Layout::BODY_PAD + 5, y + 8, 5, C_GREEN);
        tft.setTextColor(C_GREEN);
        tft.drawString("Device Found!", Layout::BODY_PAD + 18, y, 2);
        
        y += Layout::GAP_LARGE;
        
        // === CARD ===
        tft.fillRoundRect(Layout::BODY_PAD, y, Layout::CARD_W, Layout::CARD_H, 8, C_CARD_BG);
        
        int cy = y + Layout::CARD_PAD;
        int cx = Layout::CARD_CONTENT_X();
        
        // Device name
        tft.setTextColor(C_CYAN);
        tft.drawString(deviceName, cx, cy, 4);
        cy += (Layout::GAP_MEDIUM + Layout::GAP_SMALL)/2;

        // Signal
        // tft.setTextColor(C_GREY);
        // tft.drawString("Signal:", cx, cy, 2);
        
        // uint16_t rssiColor = (deviceRSSI > -50) ? C_GREEN : 
        //                      (deviceRSSI > -70) ? C_YELLOW : C_RED;
        // tft.setTextColor(rssiColor);
        // tft.drawString(String(deviceRSSI) + " dBm", cx + 60, cy, 2);
        // cy += Layout::GAP_MEDIUM;
        
        // Signal bar
        int barLen = map(deviceRSSI, -100, -20, 10, 130);
        tft.drawRect(cx + 150, cy, 35, 4, C_WHITE);
        tft.fillRect(cx + 150, cy, barLen, 4, C_GREEN);
        cy += Layout::GAP_LARGE;
        
        // MAC address
        // tft.setTextColor(C_GREY);
        // tft.drawString("MAC: " + deviceAddress, cx, cy, 2);
        // cy += Layout::GAP_MEDIUM;
        
        // UUID
        // if (deviceUUID.length() > 0) {
        //     tft.drawString("UUID: " + deviceUUID.substring(0, 8) + "...", cx, cy, 2);
        //     cy += Layout::GAP_MEDIUM;
            
        //     if (deviceUUID.indexOf("03b80e5a") >= 0) {
        //         tft.setTextColor(C_CYAN);
        //         tft.drawString("MIDI Device ✓", cx, cy, 2);
        //     }
        // }
        
    } else {
        tft.setTextColor(C_WHITE);
        tft.drawString("Scanning...", Layout::BODY_PAD, y, 4);
        y += 30;
        tft.setTextColor(C_GREY);
        tft.drawString("Make sure keyboard is in pairing mode", Layout::BODY_PAD, y, 2);
        y += 20;
        tft.drawString("(BT LED blinking)", Layout::BODY_PAD, y, 2);
    }
}
