#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEAddress.h>

class MyClientCallbacks;
class MyAdvertisedDeviceCallbacks;
void connectToDevice(String device);
void midiNotifyCallback(BLERemoteCharacteristic* pChar, uint8_t* data, size_t length, bool isNotify);

BLEScan* pBLEScan;
bool deviceFound = false;

// BLE data
String deviceName = "";
String deviceAddress = "";
int deviceRSSI = 0;
String deviceUUID = "";

BLERemoteCharacteristic* pMidiCharacteristic = nullptr;
BLEAddress pendingAddress;
bool connectionRequested = false;
int retryCount = 0;
const int MAX_RETRIES = 3;

class MyClientCallbacks : public BLEClientCallbacks {
    void onConnect(BLEClient* pClient) override{
        connectionState = CONNECTED;
        Serial.println("connected to SMK25II");

        BLERemoteService* pMidiService = pClient->getService("03B80E5A-EDE8-4B33-A751-6CE34EC4C700");
        if(pMidiService){
            BLERemoteCharacteristic* pMidiChar = pMidiService->getCharacteristic("7772E5DB-3868-4112-A1A9-F2669D106BF3");
            if(pMidiChar){
                pMidiChar->registerForNotify(midiNotifyCallback);
                Serial.println("Subscribed to MIDI Nofifications");
            }
        }
    
    
    }   

    void onDisconnect(BLEClient* pClient) override{
        connectionState = SCANNING;
        deviceFound = false;
        Serial.println("disconnected - scanning again");
    }
};

// BLE callback class
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
public:
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
        if (advertisedDevice.haveName() && isMidiInstrument(String(advertisedDevice.getName().c_str()))) {  

            deviceName = String(advertisedDevice.getName().c_str());
            deviceAddress = String(advertisedDevice.getAddress().toString().c_str());
            pendingAddress = advertisedDevice.getAddress();
            deviceRSSI = advertisedDevice.getRSSI();
            if (advertisedDevice.haveServiceUUID()) {
                deviceUUID = String(advertisedDevice.getServiceUUID().toString().c_str());
            }
            deviceFound = true;
            Serial.println("Found: " + deviceName + " UUID : " + deviceUUID     );

            connectionRequested = true;        
        }
    }
};

bool isMidiInstrument(String name){
    if (name.indexOf("SMK25") >= 0) return true;
    return false;
}

void updateBluetooth(){
    if(connectionState == SCANNING && connectionRequested ){
        Serial.println("connection request sent");
        connectToDevice(deviceAddress);
        connectionRequested = false;
    }

    if(connectionState == FAILED){
        retryCount++;
        if(retryCount <= MAX_RETRIES){
            Serial.println("Retry " + String(retryCount));
        }
        delay(retryCount*1000);
        connectionState = SCANNING;
        pBLEScan->start(10);
    }
}

void initBluetooth() {
  BLEDevice::init("MIDI-Scanner");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10);
}

void connectToDevice(String pendingAddress){
    connectionState = CONNECTING;
    Serial.println("trying to connect " + pendingAddress);

    BLEClient* pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallbacks());

    if(pClient->connect(pendingAddress)){
        Serial.println("connection successfull");
    } else {
        connectionState = FAILED;
        Serial.println("connection failed");
    }
}