//UI
#define LGFX_AUTODETECT // Autodetect board
#define LGFX_USE_V1     // set to use new version of library

#include <Arduino.h>

#include "FastLED.h"
#include "fastled-rgbw.h"

#include "LovyanGFX.hpp"
#include "lvgl.h"
#include "lv_conf.h"

// Audio
void setupAudio();
void handleAudio();

extern int audioLevel;

// FastLED
#define ARC_WIDTH_THICK LV_MAX(LV_DPI_DEF / 5, 5)
#define NUM_LEDS 42
#define NUM_STRIPS 3
#define NUM_PER_STRIP 14
#define DATA_PIN 32
#define MAX_BRIGHTNESS 255

void setupFastLED();
void handleLEDs();
void fillWhite();
void colorFill(CRGB c);

enum animationOptions
{
  RAINBOW, 
  CYLON, 
  RGBCHASER
};

extern uint8_t brightness;
extern bool showAnimation;
extern bool vu;
extern bool white;
extern animationOptions currentAnimation;


// UI
void setupScreen();
void setupUI();  

// Web UI
void setupWebUi();
void webUiLoop();
void updateWebUi();
