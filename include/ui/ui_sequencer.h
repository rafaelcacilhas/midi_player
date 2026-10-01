#include "common.h"
#include "ui_components.h"

void setBpm();
void drawStep(uint8_t index, uint8_t xStart, uint8_t  yStart);
void selectStep(uint8_t index);
void drawKickSymbol(int x, int y) ;
void drawSnareSymbol(int x, int y) ;
void drawHiHatSymbol(int x, int y) ;
void drawInstrument(String name, uint8_t xStart, uint8_t yStart);
void drawSequencerScreen();
void drawInstrumentList(uint8_t xStart, uint8_t yStart);
