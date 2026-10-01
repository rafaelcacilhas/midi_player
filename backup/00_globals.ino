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
#define C_ORANGE    TFT_ORANGE
#define C_RED       TFT_RED
#define C_DARK_RED  TFT_MAROON

#define COL_BG      TFT_BLACK
#define COL_ACCENT 0xFD20  // Orange
#define COL_WARN    0xE800  // Dark Red
#define COL_TEXT    0xFFE0  // Yellow

#define MAX_INT     32767
#define WAVE_AMP    2000

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite bluetoothIcon = TFT_eSprite(&tft);
TFT_eSprite bpmSprite = TFT_eSprite(&tft);

#define TABLE_SIZE 256

int16_t soundTable[TABLE_SIZE]; 

uint8_t MAX_VELOCITY = 127;

float currentFrequency = 0.0f;
float previousFrequency = 0.0f;
uint8_t currentVelocity = 0;
uint8_t currentType = 0;

uint8_t lastNote = 0;
uint8_t lastVelocity = 0;
uint8_t lastType = 0;       // 0x90 = Note On, 0x80 = Note Off
bool noteActive = false;

float waveMix = 0.0f; // 0 sine - 1 square

uint8_t masterVolume = 127;
uint8_t bpm = 60;

uint32_t stepInterval = 60000000 / (bpm * 4);

struct Step {
    bool active;
    bool isCurrent;
};

Step steps[16] = {};

uint8_t currentStep = 0;

struct Layout {
  // Screen
  static const int W = 320;
  static const int H = 170;

  // Spacing
  static const int GAP_TINY = 4;
  static const int GAP_SMALL = 8;
  static const int GAP_MEDIUM = 14;
  static const int GAP_LARGE = 24;
  static const int GAP_HUGE = 40;

  // Header
  static const int HEADER_H = 20;
  static const int HEADER_PAD = GAP_MEDIUM;

  // Body
  static const int BODY_Y = HEADER_H;
  static const int BODY_PAD = HEADER_PAD;
  static const int BODY_W = W - (BODY_PAD * 2);

  // Sequencer
  static const int boxSize = 10;
  static const int buttonSize = 50;
};


void initHardware() {
    pinMode(0, INPUT_PULLUP);

    tft.init();
    tft.setRotation(1);

    initSprites();
}

TFT_eSprite attSprite = TFT_eSprite(&tft);
TFT_eSprite decSprite = TFT_eSprite(&tft);
TFT_eSprite susSprite = TFT_eSprite(&tft);
TFT_eSprite relSprite = TFT_eSprite(&tft);

TFT_eSprite waveSprite = TFT_eSprite(&tft);
TFT_eSprite envSprite = TFT_eSprite(&tft);

float attackTime   = 0.05f;
float decayTime    = 0.2f;
float sustainTime = 0.4f;
float releaseTime  = 0.3f;

uint8_t currentInstrument = 0;  

void initSprites(){
    bluetoothIcon.setColorDepth(16);
    bluetoothIcon.createSprite(8,8);

    bpmSprite.setColorDepth(16);
    bpmSprite.createSprite(8,8);

    waveSprite.setColorDepth(16);
    waveSprite.createSprite(200,60);

    initEnvSprites();
}

void initEnvSprites(){
    envSprite.createSprite(200, 60);  
    envSprite.setColorDepth(16);

    envSprite.createSprite(Layout::buttonSize, Layout::buttonSize);
    envSprite.setColorDepth(16);

    attSprite.createSprite(Layout::buttonSize, Layout::buttonSize);
    attSprite.setColorDepth(16);
    attSprite.setTextSize(2);

    decSprite.createSprite(Layout::buttonSize, Layout::buttonSize);
    decSprite.setColorDepth(16);
    decSprite.setTextSize(2);

    susSprite.createSprite(Layout::buttonSize, Layout::buttonSize);
    susSprite.setColorDepth(16);
    susSprite.setTextSize(2);

    relSprite.createSprite(2.5*Layout::buttonSize, 1.5*Layout::buttonSize);
    relSprite.setColorDepth(16);
    relSprite.setTextSize(2);

}

#define MAX_ATTACK 5.0f
#define MAX_DECAY 10.0f
#define MAX_SUSTAIN 1.0f
#define MAX_RELEASE 10.0f