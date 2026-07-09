#include <TFT_eSPI.h>
#include <SPI.h>

// Colors
#define C_BG        TFT_BLACK
#define C_HEADER_BG TFT_NAVY
#define C_CARD_BG   TFT_BLACK
#define C_WHITE     TFT_WHITE   
#define C_CYAN      TFT_CYAN
#define C_GREY      TFT_LIGHTGREY
#define C_GREEN     TFT_GREEN
#define C_YELLOW    TFT_YELLOW
#define C_RED       TFT_MAROON

#define MAX_INT     32767
#define WAVE_AMP    2000

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite bluetoothIcon = TFT_eSprite(&tft);
#define TABLE_SIZE 256

int16_t soundTable[TABLE_SIZE]; 

uint8_t MAX_VELOCITY = 127;

float currentFrequency = 0.0f;
uint8_t currentVelocity = 0;
uint8_t currentType = 0;

uint8_t lastNote = 0;
uint8_t lastVelocity = 0;
uint8_t lastType = 0;       // 0x90 = Note On, 0x80 = Note Off
bool noteActive = false;

float waveMix = 0.0f; // 0 sine - 1 square

uint8_t masterVolume = 11;

void initHardware() {
    pinMode(0, INPUT_PULLUP);

    tft.init();
    tft.setRotation(1);
    bluetoothIcon.setColorDepth(16);
    bluetoothIcon.createSprite(16,16);
}