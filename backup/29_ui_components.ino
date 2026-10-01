uint8_t BPM_X = 0.75*Layout::W;
uint8_t BPM_Y = Layout::GAP_LARGE + 0.1* Layout::HEADER_H;

void setBpm(uint8_t value){
  if(value == 0) value = 40;
  bpm = value;
  stepInterval = 60000000 / (bpm * 4);

  bpmSprite.fillSprite(C_BG);
  bpmSprite.setTextColor(C_ORANGE);
  bpmSprite.drawString(String(bpm), 0, 0);
  bpmSprite.pushSprite(BPM_X, BPM_Y);
}

void drawKickSymbol(int x, int y) {
  uint8_t size = 10;
  tft.drawRect(x, y, 2*size, size, C_ORANGE); 
  tft.drawLine(x, y, x+size, y, C_ORANGE);
  tft.drawLine(x, y+size, x+size, y+size, C_ORANGE); 
}

void drawSnareSymbol(int x, int y) {
  uint8_t size = 6;
  tft.drawCircle(x+size, y+size, size, C_ORANGE);
  tft.drawLine(x+2,y+2, x+2*size,y+size, C_ORANGE);
  tft.drawLine(x+2,y+size, x+2*size,y+2, C_ORANGE);
}

void drawHiHatSymbol(int x, int y) {
  uint8_t size = 6;
  tft.drawCircle(x+3, y+5, size, C_ORANGE);
  tft.drawCircle(x+8, y+5, size/2, C_ORANGE);
}

void drawBluetoothScreen() {
  int y = Layout::HEADER_H + Layout::GAP_HUGE;
  if (deviceFound) {
    // Status dot + text
    tft.fillCircle(Layout::BODY_PAD + 5, y + 8, 5, C_GREEN);
    tft.setTextColor(C_GREEN);
    tft.drawString("Device Found!", Layout::BODY_PAD + 18, y, 2);

    y += Layout::GAP_LARGE;

    // === CARD ===
    tft.fillRoundRect(Layout::BODY_PAD, y, Layout::BODY_W, 2*Layout::buttonSize, 8, C_CARD_BG);

    int cy = y + Layout::BODY_PAD;
    int cx = 2*Layout::BODY_PAD;

    // Device name
    tft.setTextColor(C_CYAN);
    tft.drawString(deviceName, cx, cy, 4);
    cy += (Layout::GAP_MEDIUM + Layout::GAP_SMALL) / 2;

    // Signal
    // tft.setTextColor(C_GREY);
    // tft.drawString("Signal:", cx, cy, 2);

    // uint16_t rssiColor = (deviceRSSI > -50) ? C_GREEN :
    //                      (deviceRSSI > -70) ? C_YELLOW : C_DARK_RED;
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
    y = Layout::BODY_Y + Layout::BODY_PAD;
    tft.setTextColor(C_WHITE);
    tft.drawString("Scanning...", Layout::BODY_PAD, y + Layout::GAP_HUGE, 4);
    y += Layout::GAP_HUGE;
    tft.setTextColor(C_GREY);
    tft.drawString("Make sure keyboard is in pairing mode", Layout::BODY_PAD, y + 2*Layout::GAP_HUGE, 2);
    y +=  Layout::GAP_HUGE;
    tft.drawString("(BT LED blinking)", Layout::BODY_PAD, y+2.5*Layout::GAP_HUGE, 2);
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

  tft.setTextColor(C_DARK_RED);
  tft.drawString("connection failed", Layout::BODY_PAD, y, 4);
  y += 30;
  tft.setTextColor(C_GREY);
  tft.drawString(reason, Layout::BODY_PAD, y, 2);
  y += 20;
}

void drawErrorScreen() {
  int y = Layout::BODY_Y + Layout::BODY_PAD;

  tft.setTextColor(C_DARK_RED);
  tft.drawString("error", Layout::BODY_PAD, y, 4);
  y += 30;
  tft.setTextColor(C_GREY);
  tft.drawString("Screen not implemented", Layout::BODY_PAD, y, 2);
  y += 20;
}

void updateAttDisplay() {
    attSprite.fillSprite(C_BG);
    uint8_t x = Layout::GAP_TINY;
    uint8_t y = Layout::H - 1.25*Layout::buttonSize;
    uint  valuePercent  =  100 * attackTime / MAX_ATTACK;

    attSprite.fillSprite(C_BG);
    attSprite.setTextColor(C_ORANGE);
    attSprite.drawString("Att", Layout::GAP_MEDIUM, Layout::GAP_SMALL);
    attSprite.drawString(String(valuePercent), Layout::GAP_LARGE, Layout::GAP_LARGE + Layout::GAP_SMALL);
    attSprite.pushSprite(x, y);

    updateEnvDisplay();
}

void updateDecDisplay() {

    decSprite.fillSprite(C_BG);
    uint8_t x = 2*Layout::GAP_SMALL + 1.5*Layout::buttonSize;
    uint8_t y = Layout::H - 1.25*Layout::buttonSize ;
    uint  valuePercent  =  100 * decayTime   /  MAX_DECAY;

    decSprite.fillSprite(C_BG);
    decSprite.setTextColor(C_ORANGE);
    decSprite.drawString("Dec", Layout::GAP_MEDIUM, Layout::GAP_SMALL);
    decSprite.drawString(String(valuePercent), Layout::GAP_LARGE, Layout::GAP_LARGE + Layout::GAP_SMALL);
    decSprite.pushSprite(x, y);

    updateEnvDisplay();
}

void updateSusDisplay() {
    susSprite.fillSprite(C_BG);
    uint8_t x = 4*Layout::GAP_SMALL + 3*Layout::GAP_HUGE;
    uint8_t y = Layout::H - 1.25*Layout::buttonSize ;
    uint  valuePercent  =  100 * sustainTime   /  MAX_SUSTAIN;

    susSprite.fillSprite(C_BG);
    susSprite.setTextColor(C_ORANGE);
    susSprite.drawString("Sus", Layout::GAP_MEDIUM, Layout::GAP_SMALL);
    susSprite.drawString(String(valuePercent), Layout::GAP_LARGE, Layout::GAP_LARGE + Layout::GAP_SMALL);
    susSprite.pushSprite(x, y);

    updateEnvDisplay();
}

void updateRelDisplay() {
    relSprite.fillSprite(C_BG);
    uint8_t x = 6*Layout::GAP_SMALL + 4.5*Layout::GAP_HUGE;
    uint8_t y = Layout::H - 1.25*Layout::buttonSize ;
    uint  valuePercent  =  100 * releaseTime   /  MAX_RELEASE;

    relSprite.fillSprite(C_BG);
    relSprite.setTextColor(C_ORANGE);
    relSprite.drawString("Rel", Layout::GAP_MEDIUM, Layout::GAP_SMALL);
    relSprite.drawString(String(valuePercent), Layout::GAP_LARGE, Layout::GAP_LARGE + Layout::GAP_SMALL);
    relSprite.pushSprite(x, y);

    updateEnvDisplay();
}

void updateWaveSprite(int w, int h, int16_t* table) {
    waveSprite.fillSprite(C_BG);

    float middleC = 261.6;
    float cycles = 3.0f * (previousFrequency / middleC );
    // if (cycles < 1.0f) cycles = 1.0f;
    // if (cycles > 8.0f) cycles = 8.0f;

    int totalSamples = (int)(TABLE_SIZE * cycles);
    // tft.drawLine(x, y, x, y + h, C_ORANGE);
    // tft.drawLine(x, y + h, x + w, y + h, C_ORANGE);

    float xStep = (float)w / totalSamples;
    int midY = h / 2;
    int amp = h / 2 - Layout::GAP_SMALL;

    int prevScreenX = 0;
    int prevScreenY = midY - (table[0] * amp / WAVE_AMP);

    for (int i = 1; i < totalSamples; i++) {
        int idx = i % TABLE_SIZE;
        int screenX = (int)(i * xStep);
        int screenY = midY - (table[idx] * amp / WAVE_AMP);
        waveSprite.drawLine(prevScreenX, prevScreenY, screenX, screenY, C_ORANGE);
        prevScreenX = screenX;
        prevScreenY = screenY;
    }
    waveSprite.pushSprite(Layout::GAP_HUGE, Layout::HEADER_H + Layout::GAP_LARGE);
}

void updateEnvDisplay() {
    envSprite.fillSprite(C_BG);
    int xStart = Layout::GAP_HUGE;

    int w = 200;
    int h = 60;
    int midY = h / 2;
    int amp = h / 2 ;

    float total = attackTime + decayTime + 0.1f + releaseTime; 
    
    int attackEndX  = xStart + (int)(w * attackTime / total);
    int decayEndX   = attackEndX + (int)(w * decayTime / total);
    int sustainEndX = decayEndX + (int)(w * (0.1f / total));
    int sustainY    = 2*midY  - (int)(sustainTime * 2 * midY);
    
    envSprite.drawLine(xStart, midY + amp, attackEndX, midY - amp, C_ORANGE);
    envSprite.drawLine(attackEndX, midY - amp, decayEndX, sustainY, C_ORANGE);
    envSprite.drawLine(decayEndX, sustainY, sustainEndX, sustainY, C_ORANGE);
    envSprite.drawLine(sustainEndX, sustainY, w, midY + amp, C_ORANGE);

    envSprite.pushSprite(Layout::BODY_PAD, Layout::HEADER_H + Layout::GAP_LARGE);
}
