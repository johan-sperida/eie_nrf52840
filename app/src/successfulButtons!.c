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

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

//the device
static const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

//the canvas
static lv_obj_t *screen = NULL;

int main(void) {

  //check if it's ready
  if (!device_is_ready(display_dev)) {
        return 0;
    }

  //returns pointer to active screan (root container where UI elements are drawn) (canvas)
  screen = lv_screen_active();
  if (screen == NULL) {
      return 0;
  }

  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  //creating the objects
  lv_obj_t *label1 = lv_label_create(screen);

  lv_obj_t *label2 = lv_label_create(screen);

  //content
  // lv_image_set_src(image, &IMG_4439);
  // lv_obj_align(image, LV_ALIGN_CENTER, 0, 0);
 
  lv_label_set_text(label1, "hi");

  lv_label_set_text(label2, "morbin time");
  lv_obj_align(label2, LV_ALIGN_CENTER, 20, 0);


  display_blanking_off(display_dev);
  while (1) {
    //must call periodically to render changes
    lv_timer_handler();
    k_msleep(SLEEP_MS);

    if (BTN_check_clear_pressed(BTN2)){
      lv_label_set_text(label2, "morbin times!!!!");
      lv_obj_align(label2, LV_ALIGN_CENTER, -20, 0);
    }

    if (BTN_check_clear_pressed(BTN0)){
      lv_label_set_text(label2, "morbin time");
      lv_obj_align(label2, LV_ALIGN_CENTER, -20, 0);
    }
    
  }
  return 0;
}
