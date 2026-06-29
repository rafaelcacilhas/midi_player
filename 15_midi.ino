void midiNotifyCallback(BLERemoteCharacteristic* pChar, uint8_t* data, size_t length, bool isNotify) {
    Serial.print(" raw: ");
    for (int i = 0; i < length; i++) {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    if (length < 2) return;

    static uint8_t runningStatus = 0;

    for(int i = 1; i < length;){
      uint8_t byte = data[i];

      if( byte & 0x80){ // Isolates byte 7
        runningStatus = byte;
        i++;
        continue;
      }

      uint8_t type = runningStatus & 0xF0;      // isolates the top 4 bits
      uint8_t channel = runningStatus & 0x0F;  // isolates the bottom 4

      if(type == 0x90 && (i+1) < length && data[i+1] > 0){
        Serial.printf("Note ON: ch=%d note=%d vel=%d\n", channel, data[i], data[i+1]);
        i += 2;
      }
      else if(type == 0x80 || (type == 0x90 && (i + 1) < length && data[i+1] == 0)){
          Serial.printf("Note OFF: ch=%d note=%d\n", channel, data[i]);
          i += 2;
      }
      else if (type == 0xB0 && (i + 1) < length) {
        Serial.printf("CC: ch=%d cc=%d val=%d\n", channel, data[i], data[i+1]);
        i += 2;
      }
      else if (type == 0xC0) {
        Serial.printf("Program Change: ch=%d prog=%d\n", channel, data[i]);
        i += 1;
      }
      else {
        Serial.printf("Unknown: runningStatus=0x%X data=%d\n", runningStatus, data[i]);
        break;
      }

    }
}