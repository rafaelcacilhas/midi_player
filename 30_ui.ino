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
  static const int CARD_Y() {
    return BODY_Y + 3 * BODY_PAD;
  }
  static const int CARD_H = 40;
  static const int CARD_PAD = BODY_PAD;
  static const int CARD_W = BODY_W;

  // Card elements
  static const int CARD_CONTENT_X() {
    return BODY_PAD + CARD_PAD;
  }
  static const int CARD_CONTENT_W() {
    return CARD_W - (CARD_PAD * 2);
  }

  // Spacing
  static const int GAP_SMALL = 8;
  static const int GAP_MEDIUM = 14;
  static const int GAP_LARGE = 25;
};

void drawHeader(ConnectionState connectionState) {
  String title = "Bluetooth";
  int32_t CONNECTION_COLOR = C_YELLOW;

  tft.fillRect(0, 0, Layout::W, Layout::HEADER_H, C_HEADER_BG);
  tft.setTextColor(C_WHITE);

  if (connectionState == SCANNING) {
    title = "Bluetooth";
    CONNECTION_COLOR = C_YELLOW;
  } else if (connectionState == CONNECTING) {
    title = "Connecting to " + deviceName;
    CONNECTION_COLOR = C_WHITE;
  } else if (connectionState == CONNECTED) {
    title = deviceName;
    CONNECTION_COLOR = C_GREEN;
  } else if (connectionState == FAILED) {
    title = "Connection failed";
    CONNECTION_COLOR = C_RED;
  } else {
    title = "not implemented";
  }

  tft.setTextSize(1);
  tft.drawString(title, Layout::HEADER_PAD, 2, 2);
  bluetoothIcon.fillSprite(CONNECTION_COLOR);
  bluetoothIcon.pushSprite(Layout::W - 8 - 2 * Layout::HEADER_PAD, Layout::HEADER_PAD / 8.0);
}

void drawBluetoothScreen() {
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
    cy += (Layout::GAP_MEDIUM + Layout::GAP_SMALL) / 2;

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
    // tft.setTextColor(C   _GREY);
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

void drawConnectingScreen() {
  int y = Layout::BODY_Y + Layout::BODY_PAD;
  tft.setTextColor(C_WHITE);
  tft.drawString("Connecting...", Layout::BODY_PAD, y, 4);
  y += 30;
  tft.setTextColor(C_GREY);
  tft.drawString(deviceName, Layout::BODY_PAD, y, 2);
}

void drawFailedScreen(int reasonNumber) {
  int y = Layout::BODY_Y + Layout::BODY_PAD;

  String reason = "";
  switch (reasonNumber) {
    case 0:
      reason = "No error";
      break;
    case 8:
      reason = "Connection timeout";
      break;
    case 22:
      reason = "Device not found";
      break;
    case 62:
      reason = "Already connected";
      break;
    case 259:
      reason = "No resources (too many connections)";
      break;
  }

  tft.setTextColor(C_RED);
  tft.drawString("connection failed", Layout::BODY_PAD, y, 4);
  y += 30;
  tft.setTextColor(C_GREY);
  tft.drawString(reason, Layout::BODY_PAD, y, 2);
  y += 20;
}

void drawErrorScreen() {
  int y = Layout::BODY_Y + Layout::BODY_PAD;

  tft.setTextColor(C_RED);
  tft.drawString("error", Layout::BODY_PAD, y, 4);
  y += 30;
  tft.setTextColor(C_GREY);
  tft.drawString("Screen not implemented", Layout::BODY_PAD, y, 2);
  y += 20;
}

void drawMainScreen() {
  tft.setTextColor(C_WHITE);
  tft.setTextSize(2);

  int y = Layout::HEADER_H + Layout::GAP_MEDIUM;
  int x = Layout::BODY_PAD;

  if(lastNote){
    float  frequency = 440.0f * powf(2.0f, (lastNote - 69) / 12.0f);
    tft.drawString("last note: " + String(lastNote) + "Freq: %.2f Hz\n" + String(currentFrequency)  + " Vel: " + String(lastVelocity), x, y);
  }

  y+=40;
  updateWaveTable();
  drawWaveForm(Layout::W, y, 160, 90, soundTable);
}

void drawUI(ConnectionState connectionState) {
  tft.fillScreen(C_BG);
  drawHeader(connectionState);

  if (connectionState == SCANNING) {
    drawBluetoothScreen();
  } else if (connectionState == CONNECTING) {
    drawConnectingScreen();
  } else if (connectionState == CONNECTED) {
    drawMainScreen();
  } else if (connectionState == FAILED) {
    drawErrorScreen();
  } else {
    drawErrorScreen();
  }
}

void drawWaveForm(int xCenter, int y, int w, int h, int16_t* table){
  int x = xCenter / 2 - w / 2;
  tft.drawRect(x,y,w,h,C_WHITE); 

  static float scrollOffset = 0;
  scrollOffset += 0.5f; // scroll speed
  if(scrollOffset >= TABLE_SIZE) scrollOffset -= TABLE_SIZE;
  int offset = (int)scrollOffset;
  int idx = offset % TABLE_SIZE;

  float xStep = (float)w / TABLE_SIZE;
  int midY = y + h / 2;
  int amp = h/2 - Layout::GAP_SMALL;

  int prevScreenX = x;
  int prevScreenY = midY - (table[0] * amp / MAX_INT);

  for(int i= 1; i < TABLE_SIZE; i++){
    idx = (offset+1)%TABLE_SIZE;
    int screenX = x + (int)(i*xStep);
    int screenY = midY - table[i] * amp / WAVE_AMP;
    tft.drawLine(prevScreenX, prevScreenY, screenX, screenY, C_CYAN);
    prevScreenX = screenX;
    prevScreenY = screenY;
  }
}

// void drawBluetoothIcon(int32_t  iconColor) {

//   int CIRCLE_RADIUS = (Layout::HEADER_H - 2 * Layout::HEADER_PAD) / 2;
//   //bluetoothIcon.fillSprite(C_HEADER_BG);
//   bluetoothIcon.fillCircle(Layout::W - CIRCLE_RADIUS - 2 * Layout::HEADER_PAD, Layout::HEADER_H / 2, CIRCLE_RADIUS, iconColor);

//   // bluetoothIcon.drawLine(8, 1, 8, 14, iconColor);
//   // bluetoothIcon.drawLine(7, 1, 7, 14, iconColor);

//   // bluetoothIcon.drawLine(2, 3, 8, 9, iconColor);
//   // bluetoothIcon.drawLine(3, 3, 12, 12, iconColor);
//   // bluetoothIcon.drawLine(2, 4, 5, 7, iconColor);
//   // bluetoothIcon.drawLine(8, 7, 12, 11, iconColor);

//   // bluetoothIcon.drawLine(2, 12, 7, 7, iconColor);
//   // bluetoothIcon.drawLine(2, 13, 8, 7, iconColor);
//   // bluetoothIcon.drawLine(3, 13, 9, 7, iconColor);

//   // bluetoothIcon.drawLine(8, 1, 12, 4, iconColor);
//   // bluetoothIcon.drawLine(9, 1, 12, 5, iconColor);
//   // bluetoothIcon.drawLine(8, 2, 12, 6, iconColor);

//   // bluetoothIcon.drawLine(10, 8, 12, 10, iconColor);
//   // bluetoothIcon.fillRect(10,6,2,2,    iconColor);

//   // bluetoothIcon.fillRect(8,14,2,2,iconColor);
//   // bluetoothIcon.fillRect(9,13,2,2,iconColor);
//   // bluetoothIcon.fillRect(10,12,2,2,iconColor);
// }
