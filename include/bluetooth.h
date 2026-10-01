#pragma once
#include <Arduino.h>
#include <BLEScan.h>
#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>
#include "shared.h"

extern BLEScan* pBLEScan;
extern String pendingAddressString;
extern String deviceName;

extern bool deviceFound;
extern bool connectionRequested;
extern bool newMidiData;

class MyClientCallbacks;
class MyAdvertisedDeviceCallbacks;
void connectToDevice(String device);
void midiNotifyCallback(BLERemoteCharacteristic* pChar, uint8_t* data, size_t length, bool isNotify);

void initBluetooth();
void updateBluetooth();


