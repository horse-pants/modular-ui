#include "modular-ui.h"
#include "ui.h"

void rollerEvent(lv_event_t * e);

void setupEffectsList(lv_obj_t * tab)
{
  lv_obj_t *roller = lv_roller_create(tab);
  lv_roller_set_options(roller,
                      "Rainbow\n"
                      "Cylon\n"
                      "RGB Chaser\n",
                      LV_ROLLER_MODE_INFINITE);

  lv_roller_set_visible_row_count(roller, 4);
  lv_obj_set_width(roller, 300);
  lv_obj_set_height(roller, 300);
  lv_obj_center(roller);
  lv_obj_add_event_cb(roller, rollerEvent, LV_EVENT_VALUE_CHANGED, NULL);  

  static lv_style_t style_main;
    lv_style_init(&style_main);
    lv_style_set_text_font(&style_main, &lv_font_montserrat_22);
 lv_obj_add_style(roller, &style_main, LV_PART_MAIN);

   static lv_style_t style_sel;
    lv_style_init(&style_sel);
    lv_style_set_text_font(&style_sel, &lv_font_montserrat_28);
 lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
}

void rollerEvent(lv_event_t * e)
{
  lv_obj_t * slider = lv_event_get_target(e);
  int sliderValue = (int)lv_roller_get_selected(slider);
  showAnimation = true;
  switch(sliderValue)
  {
    case 0: currentAnimation = RAINBOW; break;
    case 1: currentAnimation = CYLON; break;
    case 2: currentAnimation = RGBCHASER; break;
    default: break;
  }

  lv_obj_clear_state(whiteButton, LV_STATE_CHECKED);
  white = false;
  FastLED.clear();    
}

void toggleAnimationState(){
  showAnimation = !showAnimation;    
  if(showAnimation)
  {
    lv_obj_clear_state(whiteButton, LV_STATE_CHECKED);
    white = false;
    FastLED.clear();  
  }
  else
  {
    setRGB();
  }
}

void setAnimation(int animation){
  
   showAnimation = true;

  switch(animation)
  {
    case 0: currentAnimation = RAINBOW; break;
    case 1: currentAnimation = CYLON; break;
    case 2: currentAnimation = RGBCHASER; break;
    default: break;
  }

  lv_obj_clear_state(whiteButton, LV_STATE_CHECKED);
  white = false;
  FastLED.clear();  
  updateWebUi();  
}