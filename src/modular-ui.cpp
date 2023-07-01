#include "modular-ui.h"

void setup(void)
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("started...");

  setupScreen();
  setupUI();
  setupFastLED(); 
  setupAudio();
  setupWebUi();  
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  handleAudio();
  handleLEDs();    
  webUiLoop();
}  
