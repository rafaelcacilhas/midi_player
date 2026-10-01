void setAttack(float val) {  
    Serial.print("setAttack "); Serial.println(val, 2);
    if (val < 0.001f) val = 0.001f;
    if (val > 5.0f) val = MAX_ATTACK;   
    attackTime = val;
    if(selectedMode == ENVELOPE)  updateAttDisplay();
}
void setDecay(float val) {
    Serial.print("setDecay "); Serial.println(val, 2);
    if (val < 0.001f) val = 0.001f;
    if (val > 5.0f) val = MAX_DECAY;   
    decayTime = val;
    if(selectedMode == ENVELOPE) updateDecDisplay();
}
void setSustain(float val) {
    Serial.print("setSustain "); Serial.println(val, 2);
    if (val < 0.001f) val = 0.001f;
    if (val > 5.0f) val = MAX_SUSTAIN;   
    sustainTime = val;
    if(selectedMode == ENVELOPE) updateSusDisplay();
}
void setRelease(float val) {
    Serial.print("setRelease "); Serial.println(val, 2);
    if (val < 0.001f) val = 0.001f;
    if (val > 5.0f) val = MAX_RELEASE;   
    releaseTime = val;
    if(selectedMode == ENVELOPE) updateRelDisplay();
}