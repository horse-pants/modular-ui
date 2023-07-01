#include "modular-ui.h"
#include "ui.h"

//UI
static LGFX lcd; // declare display variable

/*** Setup screen resolution for LVGL ***/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 480;
static lv_disp_draw_buf_t draw_buf; 
static lv_color_t buf[screenWidth * 10];

/*** Function declaration ***/
void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void touchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void setupDisplayDriver();
void setupTouchDriver();
void scrollBeginEvent(lv_event_t * e);

void setupScreen()
{
    // UI
  lcd.init(); // Initialize LovyanGFX
  lv_init();  // Initialize lvgl

  lcd.setRotation(0);

  setupDisplayDriver();

  setupTouchDriver();
}

void setupDisplayDriver(){
    /* LVGL : Setting up buffer to use for display */
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  /*** LVGL : Setup & Initialize the display device driver ***/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = displayFlush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
}

void setupTouchDriver(){
  /*** LVGL : Setup & Initialize the input device driver ***/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpadRead;
  lv_indev_drv_register(&indev_drv);
}

void setupUI(void)
{
  /*Create a Tab view object*/
  lv_obj_t * tabview;

  tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 50);
  lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_event_cb(lv_tabview_get_content(tabview), scrollBeginEvent, LV_EVENT_SCROLL_BEGIN, NULL);

  /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
  lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Colour");
  lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Effects");

  setupBrightnessSlider();
  setupVuButton();
  setupColourWheel(tab1);
  setupEffectsList(tab2);
  setupWhiteButton(tab1);
}

/*** Display callback to flush the buffer to screen ***/
void displayFlush(lv_disp_drv_t * disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lcd.startWrite();
  lcd.setAddrWindow(area->x1, area->y1, w, h);
  lcd.pushPixels((uint16_t *)&color_p->full, w * h, true);
  lcd.endWrite();

  lv_disp_flush_ready(disp);
}

/*** Touchpad callback to read the touchpad ***/
void touchpadRead(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
  uint16_t touchX, touchY;
  bool touched = lcd.getTouch(&touchX, &touchY);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void scrollBeginEvent(lv_event_t * e)
{
  /*Disable the scroll animations. Triggered when a tab button is clicked */
  if(lv_event_get_code(e) == LV_EVENT_SCROLL_BEGIN) {
      lv_anim_t * a = (lv_anim_t*) lv_event_get_param(e);
      if(a)  a->time = 0;
  }
}


