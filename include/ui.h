#include "lvgl.h"

void setupColourWheel(lv_obj_t * tab);
void setupWhiteButton(lv_obj_t * tab);
void setupEffectsList(lv_obj_t * tab);
void setupBrightnessSlider();
void setupVuButton();
void setRGB();
void setColorWheelValue(String hexstring);
String getColorWheelValue();
void toggleVuState();
void toggleWhiteState();
void toggleAnimationState();
void setAnimation(int animation);

extern lv_obj_t * whiteButton;
extern lv_obj_t * vuButton;
