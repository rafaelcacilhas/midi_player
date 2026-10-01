
#include <Arduino.h>
#include "bluetooth.h"

void handleNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);

void handleNoteOff(uint8_t channel, uint8_t note);
bool checkPackage(uint8_t* data, size_t length);
void midiNotifyCallback(BLERemoteCharacteristic* pChar, uint8_t* data, size_t length, bool isNotify);
