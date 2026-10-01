#pragma once
#include <Arduino.h>

extern uint8_t masterVolume ;
extern uint8_t bpm ;
extern uint32_t stepInterval;
extern  uint8_t currentInstrument;  

extern float waveMix; // 0 sine - 1 square
extern float attackTime;
extern float decayTime;
extern float sustainTime;
extern float releaseTime;

extern uint8_t currentVelocity;
extern uint8_t currentType;

extern uint8_t lastNote;
extern uint8_t lastVelocity;
extern uint8_t lastType;       // 0x90 = Note On, 0x80 = Note Off

extern bool noteActive ;


void setAttack(float val);
void setDecay(float val);
void setSustain(float val);
void setRelease(float val);
void setWaveMix(float val);
void setMasterVolume(uint8_t val);
void setBpm(uint8_t val);

