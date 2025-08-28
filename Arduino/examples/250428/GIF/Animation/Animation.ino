#include "Arduino.h"
#include "lvgl.h"
#include "TFT_eSPI.h"


int AprevTime = 0;
int Anim = 0;
unsigned char key1_sta = 0;
unsigned char key2_sta = 0;
char t_ht=0,t_horse=0,sta=1;
unsigned char t1=0,t2=0,t0=1,t=1;
char t_scanf=1;
char tt=0;

unsigned char key_text=0;

void imgAnim(void);



LV_IMG_DECLARE(scan);  //horse_a planet_a  earth  ht_a
LV_IMG_DECLARE(horse);
LV_IMG_DECLARE(ht);

static const uint16_t screenWidth = 80;
static const uint16_t screenHeight = 160;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenHeight * screenWidth / 10];

TFT_eSPI tft = TFT_eSPI(80, 160);

#define LV_DELAY(x) \
  do { \
    uint32_t t = x; \
    while (t--) { \
      lv_timer_handler(); \
      delay(1); \
    } \
  } while (0);


void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}


static lv_obj_t *logo_imga = NULL;
static lv_obj_t *logo_imgb = NULL;


void setup(){

  Serial.begin(115200);
  lv_init();
  tft.begin();
  tft.setRotation(1);


  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenHeight * screenWidth / 10);


  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = 160;
  disp_drv.ver_res = 80;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;  
  lv_disp_drv_register(&disp_drv);

  //设置黑色的背景
  static lv_style_t style;  
  lv_style_init(&style);
  lv_style_set_bg_color(&style, lv_color_black());
  lv_obj_add_style(lv_scr_act(), &style, 0);

  lv_obj_t *logo_img = lv_gif_create(lv_scr_act());
  lv_obj_center(logo_img);
  // lv_obj_align(logo_img, LV_ALIGN_LEFT_MID, 0, 0);
  lv_gif_set_src(logo_img, &scan);
  LV_DELAY(1500);
  lv_obj_del(logo_img);

  static lv_style_t style1;  
  lv_style_init(&style1);
  lv_style_set_bg_color(&style1, lv_color_black());
  lv_obj_add_style(lv_scr_act(), &style1, 0);


  logo_imga = lv_gif_create(lv_scr_act());
  lv_obj_center(logo_imga);
  lv_gif_set_src(logo_imga, &ht);
  LV_DELAY(2000);
  lv_obj_del(logo_imga);

  logo_imgb = lv_gif_create(lv_scr_act());
  lv_obj_center(logo_imgb);
  lv_gif_set_src(logo_imgb, &horse);
  delay(10);
}

void loop() {
    lv_timer_handler();
    delay(1);
}




