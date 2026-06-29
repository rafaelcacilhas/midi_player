#pragma GCC diagnostic ignored "-Wc++20-extensions"
#include <clay.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// Colors
#define C_BG        TFT_BLACK
#define C_HEADER_BG 0x1A1A2E 
#define C_CARD_BG   TFT_BLACK
#define C_WHITE     TFT_WHITE   
#define C_CYAN      TFT_CYAN
#define C_GREY      0xAAAAAA
#define C_GREEN     TFT_GREEN
#define C_YELLOW    TFT_YELLOW
#define C_RED       0xFF3232

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite bluetoothIcon = TFT_eSprite(&tft);

void initHardware() {
    tft.init();
    tft.setRotation(1);
    bluetoothIcon.setColorDepth(16);
    bluetoothIcon.createSprite(16,16);
}