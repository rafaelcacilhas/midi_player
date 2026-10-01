#pragma once
#include <cstdint>
#include "config.h"

extern float currentFrequency;
extern float previousFrequency;
extern uint8_t currentVelocity;
extern uint8_t lastVelocity;
extern uint8_t lastNote;
extern bool noteActive;

extern int16_t soundTable[TABLE_SIZE];
extern int16_t squareTable[TABLE_SIZE]; 
extern int16_t sineTable[TABLE_SIZE]; 
extern uint32_t phaseAccum;        

extern float waveMix; // 0 sine - 1 square

void initSound();
void updateSound();
void updateWaveTable();