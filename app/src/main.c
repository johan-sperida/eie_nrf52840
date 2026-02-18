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
#include "customDisplay.h"

#define SLEEP_MS 1

int main(void) {

  displayStruct display;

  if (0 > displayInit(&display)){
    return 0;
  }


  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  textCreate(&display,0);

  

  //content
  // lv_image_set_src(image, &IMG_4439);
  // lv_obj_align(image, LV_ALIGN_CENTER, 0, 0);

  while (1) {
    //must call periodically to render changes
    lv_timer_handler();
    k_msleep(SLEEP_MS);

    if (BTN_check_clear_pressed(BTN2)){
      textUpdate(&display,0, "Bananas");
    }

    if (BTN_check_clear_pressed(BTN0)){
      textUpdate(&display,0, "ORANGES");
    }
    
  }
  return 0;
}
