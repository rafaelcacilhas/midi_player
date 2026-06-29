extern bool deviceFound;
enum ConnectionState { SCANNING, CONNECTING, CONNECTED, FAILED };
ConnectionState connectionState = SCANNING;

void setup() {
    Serial.begin(115200);

    initHardware();
    initBluetooth();
    
    drawUI(connectionState);
}

void loop() {
    updateBluetooth();

    static ConnectionState lastState = SCANNING;

    if(connectionState != lastState){
        drawUI(connectionState);
        lastState = connectionState;
    }
    delay(1000);
} 