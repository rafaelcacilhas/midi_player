#pragma once
#include "common.h"

extern TFT_eSPI tft;
extern TFT_eSprite bluetoothIcon;
extern TFT_eSprite bpmSprite;
extern TFT_eSprite attSprite;
extern TFT_eSprite decSprite;
extern TFT_eSprite susSprite;
extern TFT_eSprite relSprite;
extern TFT_eSprite waveSprite;
extern TFT_eSprite envSprite;

void initHardware();
void initSprites();
void drawUI(ConnectionState connectionState, Modes selectedMode);