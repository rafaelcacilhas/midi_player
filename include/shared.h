#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

enum ConnectionState { SCANNING, CONNECTING, CONNECTED, FAILED };
enum Modes { PERFORMANCE, SEQUENCER, ENVELOPE};
extern Modes selectedMode;

extern TFT_eSPI tft;
extern ConnectionState connectionState;
extern bool newMidiData;
extern bool printed;

void drawFailedScreen(int reason);
bool isMidiInstrument(String name);

struct Step {
    bool active;
    bool isCurrent;
};

extern Step steps[16];

extern uint8_t currentStep;


