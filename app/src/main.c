/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/display.h>
#include <zephyr/device.h>


#include <lvgl.h>

#include "BTN.h"
#include "LED.h"
#include "lv_data_obj.h"
#include "scenes.h"

#define SLEEP_MS 1

static const struct device *display_dev;
static lv_obj_t *activeScreen = NULL;
static lv_obj_t *topScreen = NULL;

int main(void) {
  display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
  

  if (!device_is_ready(display_dev)) {
      return -1;
  }

  activeScreen = lv_screen_active();

  topScreen = lv_layer_top();

  if (activeScreen == NULL || topScreen == NULL) {
    return 0;
  }

  display_blanking_off(display_dev);  

  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  scene scene1;

  sceneInit(&scene1, activeScreen);
  lv_obj_set_style_bg_color(scene1.parent, lv_color_hex(0xFF0000), 0); 


  //-----------------------------------------------
  scene scene2;

  sceneInit(&scene2, topScreen);
  lv_obj_set_style_bg_color(scene2.parent, lv_color_hex(0x000000), 0);  // Black background


  while (1) {
    //must call periodically to render changes
    lv_timer_handler();
    k_msleep(SLEEP_MS);

    if (BTN_check_clear_pressed(BTN0)){
      lv_obj_set_parent(scene1.parent, topScreen);
      lv_obj_set_parent(scene2.parent, activeScreen);
    }

    if (BTN_check_clear_pressed(BTN1)){
      lv_obj_set_parent(scene2.parent, topScreen);
      lv_obj_set_parent(scene1.parent, activeScreen);
    }
  }
  return 0;
}
