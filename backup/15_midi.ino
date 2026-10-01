void handleNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
  Serial.printf("Note: %d  Freq: %.2f Hz  Vel: %d\n",
                note, 440.0f * powf(2.0f, (note - 69) / 12.0f), velocity);

  lastNote = note;
  lastVelocity = velocity;

  currentFrequency = 440.0f * powf(2.0f, (note - 69) / 12.0f);
  previousFrequency = currentFrequency;
  currentVelocity = velocity;
  noteActive = true;
  newMidiData = true;
}

void handleNoteOff(uint8_t channel, uint8_t note) {
  //Serial.printf("Note OFF: ch=%d note=%d\n", channel, note);
  previousFrequency = currentFrequency;
  currentFrequency = 0;
  currentVelocity = 0;
  noteActive = false;
  newMidiData = true;
}

bool checkPackage(uint8_t* data, size_t length) {
  if (data == nullptr || length == 0) {
    Serial.println("midiNotifyCallback: null data or zero length");
    return false;
  }
  // Serial.print("raw data: ");
  for (int i = 0; i < length; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (length < 2) return false;

  return true;
}

void midiNotifyCallback(BLERemoteCharacteristic* pChar, uint8_t* data, size_t length, bool isNotify) {
  if (checkPackage(data, length)) {
    static uint8_t runningStatus = 0;

    for (int i = 1; i < length;) {
      uint8_t byte = data[i];

      if (byte & 0x80) {  // Isolates byte 7
        runningStatus = byte;
        i++;
        continue;
      }

      uint8_t type = runningStatus & 0xF0;     // isolates the top 4 bits
      uint8_t channel = runningStatus & 0x0F;  // isolates the bottom 4

      if (type == 0x90 && (i + 1) < length && data[i + 1] > 0) {  // Pads
        Serial.printf("handleNoteOn inputs: %d, freq: %d, vol: %d\n", 
                    channel, data[i], data[i + 1]);
        printed = false;
        handleNoteOn(channel, data[i], data[i + 1]);
        if (data[i] == 40) {
          selectedMode = selectedMode == PERFORMANCE ? SEQUENCER : PERFORMANCE;
          newMidiData = true;
        } 
        else if(data[i] == 41) {
          selectedMode = ENVELOPE;
          newMidiData = true;
        } 
        i += 2;
        } else if (type == 0x80 || (type == 0x90 && (i + 1) < length && data[i + 1] == 0)) {
          handleNoteOff(channel, data[i]);
          i += 2;
      } 
      
      else if (type == 0xB0 && (i + 1) < length) {  // Knobs
        switch (data[i]) {
          case 30:  //  Volume
            if (masterVolume == 0 && data[i + 1] == 0) noteActive = false;
            masterVolume = data[i + 1];
            Serial.printf("Volume change: val=%d\n", data[i + 1]);
            break;
          case 31:  // Wave shape
            waveMix = data[i + 1] / 127.0f;
            updateWaveTable();
            break;
          case 32: 
            bpm = map(data[i + 1], 0, 127, 60, 200);
            newMidiData = true;
            Serial.printf("bpm change: val=%d\n", data[i + 1]);
            break;
          case 33: break;

          case 34: 
            setAttack( data[i+1] / 127.0f * MAX_ATTACK);
            break;
          case 35:
            setDecay( data[i+1] / 127.0f * MAX_DECAY);
            break;
          case 36:
            setSustain( data[i+1] / 127.0f * MAX_SUSTAIN);
            break;
          case 37:
            setRelease( data[i+1] / 127.0f * MAX_RELEASE);
            break;

          default:
            Serial.printf("Unmapped CC: ch=%d cc=%d val=%d\n", channel, data[i], data[i + 1]);
        }
        i += 2;
      } 
      
      else if (type == 0xC0) {  // Program Change
        Serial.printf("Program Change: ch=%d prog=%d\n", channel, data[i]);
        i += 1;
      } else if (type == 0xE0) {  //Pitch Bend
        Serial.printf("Pitch Bend", channel, data[i]);
        i++;
      } else if (type == 0xF0) {  // MCP + Pads
        Serial.printf(" MCP + Pads", channel, data[i]);
        i++;
      }
    }
  }
}