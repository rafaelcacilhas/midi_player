extern bool deviceFound;    
extern bool connectionRequested;

bool newMidiData = false;

enum ConnectionState { SCANNING, CONNECTING, CONNECTED, FAILED };
enum Modes { PERFORMANCE, SEQUENCER, ENVELOPE};

ConnectionState connectionState = SCANNING;
Modes selectedMode = PERFORMANCE;
    static bool printed = false;

void setup() {
    Serial.begin(115200);

    initHardware();
    initBluetooth();
    initSound();
}

void loop() {   
    updateBluetooth();
    updateSound();
    checkInputs();

    if(newMidiData){
        drawUI(connectionState, selectedMode);
        newMidiData = false;
    } 

} 

void checkInputs(){
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

    static bool lastBtn = HIGH;
    bool btn = digitalRead(14);
    if (btn == LOW && lastBtn == HIGH) {
        selectedMode = selectedMode == PERFORMANCE? SEQUENCER : PERFORMANCE;
        newMidiData = true;  // Trigger redraw
    }
    lastBtn = btn;

    if(Serial.available()){
        char c = Serial.read();
        if (c == 'f' || c == 'F'){
            handleNoteOn(0,60,100);
            printed = false;
        } else if(c == 'g' || c == 'G') {
            handleNoteOff(0,60);
        }
    }
}