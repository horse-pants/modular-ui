#include "modular-ui.h"

uint8_t brightness = 128;
bool showAnimation = false;
bool vu = false;
bool white = true;
animationOptions currentAnimation = RAINBOW;

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

void startup();
void setLedBrightness();
int getAnimationInterval();
void cylon();
void rainbow();
void rgbChaser();

void setupFastLED()
{  
  // FastLED
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
  FastLED.clear();    
  FastLED.setBrightness(0);
  fillWhite();
  startup();
}

void startup(){
  for(int i = 0; i <= brightness; i++){
    FastLED.setBrightness(i);
    FastLED.show();
    delay(25);
  }
}

void handleLEDs()
{
  setLedBrightness();

  EVERY_N_MILLIS_I(thistimer, 100) 
  {
    thistimer.setPeriod(getAnimationInterval());
    if(showAnimation)
    {
      switch(currentAnimation)
      {
        case CYLON: cylon(); break;
        case RAINBOW: rainbow(); break;
        case RGBCHASER: rgbChaser(); break;
        default: break;
      }  
    }
  }
  
  FastLED.show();
}

void setLedBrightness()
{
  if(vu)
  {
    FastLED.setBrightness(audioLevel); 
  }
  else
  {
    FastLED.setBrightness(brightness);  
  }  
}

int getAnimationInterval()
{
  switch(currentAnimation)
  {
    case CYLON: return 85;
    case RAINBOW: return 10;
    case RGBCHASER: return 50;
    default: return 100;
  }  
}

void colorFill(CRGB c){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = c;
  }
}

void fillWhite(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGBW(0, 0, 0, 255);
  }
}



// Animations
void fadeall() 
{ 
  for(int i = 0; i < NUM_LEDS; i++) 
  { 
    CRGBW colour = leds[i];
    int red = colour.r - 50;
    if(red <= 0) 
    {
      red = 0;
    }
    
    colour.r = red;
    leds[i] = colour;
  }
}

void cylon(){
  static bool goingLeft = true;
  static uint8_t animationLed = 0;

  CRGBW snuh = leds[animationLed];
  leds[animationLed] = CRGB::Red;
  leds[((NUM_PER_STRIP * 2) - 1) -  animationLed] = CRGB::Red;
  leds[animationLed + (NUM_PER_STRIP * 2)] = CRGB::Red;
  fadeall();

  if(goingLeft)
  {
    animationLed++;
    if(animationLed == NUM_PER_STRIP - 1)
    {
      goingLeft = false;
    }
  }
  else
  {
    animationLed--;
    if(animationLed == 0)
    {
      goingLeft = true;
    }
  }
}

void rainbow(){
  static uint8_t hue;
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV((i * 256 / NUM_LEDS) + hue, 255, 255);
  }
  hue++;
}

void rgbChaser(){
  static CRGB colour = CRGB::Red;    
  static int colourCount = 0;  
  static uint8_t animationLed = 0;   
  leds[animationLed] = colour;    
  animationLed++;
  if(animationLed == NUM_LEDS)
  {
    animationLed = 0;

    colourCount++;
    if(colourCount == 3) colourCount = 0;
    switch(colourCount)
    {
      case 0: colour = CRGB::Red; break;
      case 1: colour = CRGB::Green; break;
      case 2: colour = CRGB::Blue; break;
      default: break;
    }
  }
}
