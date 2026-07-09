extern bool deviceFound;    
extern bool connectionRequested;

bool newMidiData = false;

enum ConnectionState { SCANNING, CONNECTING, CONNECTED, FAILED };
ConnectionState connectionState = SCANNING;

void setup() {
    Serial.begin(115200);

    initHardware();
    initBluetooth();
    initSound();
}

void loop() {   
    updateBluetooth();
    updateSound();

    if (digitalRead(0) == HIGH) {
        delay(50);  // debounce
        if (digitalRead(0) == HIGH) {  // still pressed
            connectionState = SCANNING;     
            deviceFound = false;
            connectionRequested = false;
            Serial.println("Scanning again");
            updateBluetooth();
        }
        while (digitalRead(0) == HIGH);  // wait for release
    }

    if(newMidiData){
        drawUI(connectionState);
        newMidiData = false;
    } 
} 