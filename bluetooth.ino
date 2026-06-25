#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>


BLEScan* pBLEScan;
bool deviceFound = false;
String foundName = "";
String foundAddress = "";
int foundRSSI = 0;
String foundUUID = "";

const int SCAN_TIME = 10; 
bool isScanning = false;


class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.haveName()) {
            String name = String(advertisedDevice.getName().c_str());
            
            if (name.indexOf("SMK25") >= 0 || advertisedDevice.haveServiceUUID()) {
                foundName = name;
                foundAddress = String(advertisedDevice.getAddress().toString().c_str());
                foundRSSI = advertisedDevice.getRSSI();
                
                if (advertisedDevice.haveServiceUUID()) {
                    foundUUID = String(advertisedDevice.getServiceUUID().toString().c_str());
                }
                
                deviceFound = true;
                
                Serial.println("===== MIDI DEVICE FOUND =====");
                Serial.printf("Name: %s\n", foundName.c_str());
                Serial.printf("Address: %s\n", foundAddress.c_str());
                Serial.printf("RSSI: %d\n", foundRSSI);
                Serial.printf("Service UUID: %s\n", foundUUID.c_str());
                Serial.println("============================");
                
                pBLEScan->stop();
                isScanning = false;
            }
        }
    }
};
