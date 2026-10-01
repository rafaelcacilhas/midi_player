void drawHeader(ConnectionState connectionState, Modes selectedMode) {
  String leftText = "";
  String midText = "";
  String rightText = "";

  int32_t CONNECTION_COLOR = C_YELLOW;
  
  uint16_t x = Layout::GAP_LARGE;
  uint16_t y = Layout::GAP_SMALL;

  tft.fillRect(x, y, Layout::W - Layout::GAP_MEDIUM, Layout::HEADER_H, C_BG);
  tft.setTextColor(COL_ACCENT);
  y += 0.1* Layout::HEADER_H;

  if (connectionState == SCANNING) {
    leftText = "Bluetooth";
    CONNECTION_COLOR = C_YELLOW;
  } else if (connectionState == CONNECTING) {
    leftText = "Connecting to " + deviceName;
    CONNECTION_COLOR = C_WHITE;
  } else if (connectionState == CONNECTED) {
    leftText = deviceName;
    CONNECTION_COLOR = C_GREEN;

    switch(selectedMode){
      case PERFORMANCE:
        leftText = "PERF MODE";
        rightText = "BPM: ";
      break;
      case SEQUENCER:
        leftText = "SEQ MODE";
        midText = "PATTERN: A1";
        rightText = "BPM: ";
      break;
      case ENVELOPE:
        leftText = "ENVELOPE";
        midText = "MODE: SINGLE ";
        rightText = "";
      break;
    }
  } else if (connectionState == FAILED) {
    leftText = "Connection failed";
    CONNECTION_COLOR = C_DARK_RED;
  } else {
    leftText = "not implemented";
  }

  tft.setTextSize(1);
  tft.drawString(leftText,  Layout::GAP_LARGE, y, 2);
  tft.drawString(midText,   0.4*Layout::W, y, 2);
  tft.drawString(rightText, 0.7*Layout::W, y, 2);
  bluetoothIcon.fillSprite(CONNECTION_COLOR);
  bluetoothIcon.pushSprite(Layout::W - Layout::HEADER_PAD, y + 0.5*Layout::GAP_SMALL );
}

void drawFooter(ConnectionState connectionState) {
  tft.setTextColor(C_WHITE);
  tft.fillRect(Layout::GAP_MEDIUM, Layout::H - Layout::HEADER_H, Layout::W - Layout::GAP_MEDIUM, Layout::HEADER_H, C_DARK_RED);

  tft.setTextSize(1);
  tft.drawString("K1: TYPE", Layout::HEADER_PAD, Layout::H - 0.9*Layout::HEADER_H, 2);
  tft.drawString("K2: PIT", Layout::HEADER_PAD + 2*Layout::GAP_HUGE, Layout::H - 0.9*Layout::HEADER_H, 2);
  tft.drawString("K3: CUT", Layout::HEADER_PAD + 4*Layout::GAP_HUGE, Layout::H - 0.9*Layout::HEADER_H, 2);
  tft.drawString("K4: ENV", Layout::HEADER_PAD + 6*Layout::GAP_HUGE, Layout::H - 0.9*Layout::HEADER_H, 2);
  // tft.drawString("K5", Layout::HEADER_PAD + 4*Layout::GAP_HUGE, Layout::H - 0.5*Layout::HEADER_H, 2);
  // tft.drawString("K6", Layout::HEADER_PAD + 5*Layout::GAP_HUGE, Layout::H - 0.5*Layout::HEADER_H, 2);
  // tft.drawString("K7", Layout::HEADER_PAD + 6*Layout::GAP_HUGE, Layout::H - 0.5*Layout::HEADER_H, 2);
  // tft.drawString("K8", Layout::HEADER_PAD + 7*Layout::GAP_HUGE, Layout::H - 0.5*Layout::HEADER_H, 2);
}

void drawMainScreen(Modes selectedMode) {
    switch (selectedMode) {
    case PERFORMANCE:
      drawPerformanceScreen();
      break;
    case SEQUENCER:
      drawSequencerScreen();
      break;
    case ENVELOPE:
      drawEnvelopeScreen();
      break;
  }
}

void drawPerformanceScreen(){
  tft.setTextColor(C_ORANGE);
  tft.setTextSize(1);

  int y = Layout::HEADER_H + Layout::GAP_HUGE;
  int x = Layout::BODY_PAD;

  // if(lastNote){
  //   float  frequency = 440.0f * powf(2.0f, (lastNote - 69) / 12.0f);
  //   tft.drawString("last note: " + String(lastNote) + "Freq: %.2f Hz\n" + String(currentFrequency)  + " Vel: " + String(lastVelocity), x, y);
  // }
  // y += Layout::GAP_HUGE;
  
  updateWaveTable();
  updateWaveSprite(200, 60, soundTable);
  y += 1.5*Layout::GAP_HUGE;

  drawEnvelopeKnobs(x, y);
}

void drawEnvelopeScreen(){
  tft.setTextColor(C_ORANGE);
  tft.setTextSize(1);

  int y = Layout::HEADER_H + Layout::GAP_LARGE;
  int x = Layout::BODY_PAD;
  
  updateEnvDisplay();
  y += Layout::GAP_HUGE;

  drawEnvelopeKnobs(x, y);
}

void drawStep(uint8_t index, uint8_t xStart, uint8_t  yStart){
  if (index%2 == 0)  steps[index].active = true;
  else if(index ==1) steps[index].isCurrent = true;

  uint8_t row = index / 16;
  uint8_t col = index % 16;

  uint16_t x = xStart + Layout::GAP_HUGE + (col * 1.2*Layout::boxSize );
  uint16_t y = yStart +  (row * 1.5*Layout::boxSize );
  uint16_t w = Layout::boxSize;
  uint16_t h = 1.25*Layout::boxSize;

  uint16_t color = TFT_CYAN ; // should never happen

  if(index >= 4 ) x+=Layout::GAP_TINY;
  if(index >= 8 ) x+=Layout::GAP_TINY;
  if(index >= 12) x+=Layout::GAP_TINY;

  steps[index].active? color = C_ORANGE : color = C_BG;
  tft.fillRect(x, y, w, h, color);

  steps[index].isCurrent? color = C_WHITE : color = C_DARK_RED;
  tft.drawRect(x, y, w, h, color);
}

void selectStep(uint8_t index){
  steps[currentStep].isCurrent = false;
  currentStep = index;
  steps[currentStep].isCurrent = true;
}

void drawInstrument(String name, uint8_t xStart, uint8_t yStart){
    if(name == "KICK"){
      drawKickSymbol(xStart, yStart);
    }
    else if(name == "SNARE"){
      drawSnareSymbol(xStart, yStart);
    }
    else if(name == "HIHAT"){
      drawHiHatSymbol(xStart, yStart);
    }

    for (int i = 0; i <= 15; i++) { 
      drawStep(i, xStart,  yStart);
    }
}

void drawSequencerScreen(){
  tft.setTextColor(C_ORANGE);
  tft.setTextSize(2);
  int x = Layout::BODY_PAD + Layout::boxSize;
  int y = Layout::HEADER_H + Layout::GAP_HUGE;

  drawInstrument("KICK", x, y);
  y += Layout::GAP_MEDIUM;

  drawInstrument("SNARE", x, y);
  y += Layout::GAP_MEDIUM;
  
  drawInstrument("HIHAT", x, y);
  y += 1.25*Layout::GAP_MEDIUM;

  drawKnobs(x, y);
}

void drawKnobs(uint8_t xStart, uint8_t yStart){
  uint8_t x = Layout::GAP_HUGE;
  uint8_t y = Layout::H - Layout::HEADER_H - 1.25*Layout::buttonSize ;
  tft.setTextSize(1);

  x += Layout::GAP_SMALL;
  tft.drawString("WAV", x, y + Layout::GAP_SMALL);
  x += 1.5*Layout::GAP_HUGE;

  x += Layout::GAP_SMALL;
  tft.drawString("C2" , x, y + Layout::GAP_SMALL);
  x += 1.5*Layout::GAP_HUGE;

  x += Layout::GAP_SMALL;
  tft.drawString("FILT" , x, y + Layout::GAP_SMALL);
  x += 1.5*Layout::GAP_HUGE;

  x += Layout::GAP_SMALL;
  tft.drawString("ENV" , x, y + Layout::GAP_SMALL);

}

void drawEnvelopeKnobs(uint8_t xStart, uint8_t yStart){
  updateAttDisplay();
  updateDecDisplay();
  updateSusDisplay();
  updateRelDisplay();
}

void drawInstrumentList(uint8_t xStart, uint8_t yStart){
  uint8_t x = Layout::GAP_HUGE;
  uint8_t y = Layout::H - Layout::HEADER_H - 1.25*Layout::buttonSize ;
  tft.setTextSize(1);

  tft.drawString("Instrument  List" , x, y + Layout::GAP_SMALL);
  tft.drawString("Piano" , x, y + Layout::GAP_MEDIUM);
}

void drawUI(ConnectionState connectionState, Modes selectedMode) {
  tft.fillScreen(C_BG);
  drawHeader(connectionState, selectedMode);

  switch (connectionState) {
    case SCANNING:
      drawBluetoothScreen();
      break;
    break;
    case CONNECTING:
      drawConnectingScreen();
      break;
    break;
    case CONNECTED:
      drawMainScreen(selectedMode);
      break;
    break;
    case FAILED:
      drawErrorScreen();
      break;
    break;
  }

  // drawFooter(connectionState);
}
