#pragma once
#include <Arduino.h>

#define I2S_BCK  10
#define I2S_WS   12
#define I2S_DOUT 11
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 128

#define MAX_VELOCITY  127
#define MAX_INT     32767
#define WAVE_AMP    2000
#define TABLE_SIZE 256

#define MAX_ATTACK 5.0f
#define MAX_DECAY 10.0f
#define MAX_SUSTAIN 1.0f
#define MAX_RELEASE 10.0f


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

namespace Layout {
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


