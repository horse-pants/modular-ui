#include "modular-ui.h"
#include "ui.h"

lv_obj_t * vuButton;

void vuHandler(lv_event_t * e);

void setupVuButton()
{
  lv_obj_t * label;

  vuButton = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(vuButton, vuHandler, LV_EVENT_ALL, NULL);
  lv_obj_align(vuButton, LV_ALIGN_TOP_RIGHT, -20, 70);
  lv_obj_add_flag(vuButton, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_height(vuButton, LV_SIZE_CONTENT);

  label = lv_label_create(vuButton);
  lv_label_set_text(label, "Follow VU");
  lv_obj_center(label);
}

void vuHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_VALUE_CHANGED) {
    vu = !vu;
    updateWebUi();
  }
}

void toggleVuState(){
  vu = !vu;
  if(vu)
  {
    lv_obj_add_state(vuButton, LV_STATE_CHECKED);
  }
  else
  {
    lv_obj_clear_state(vuButton, LV_STATE_CHECKED);
  }
}