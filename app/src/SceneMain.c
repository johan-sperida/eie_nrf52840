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
#include "MyStateMachine/my_state_machine.h"

#define SLEEP_MS 1

static const struct device *display_dev;


int main(void) {
  display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
  

  if (!device_is_ready(display_dev)) {
      return -1;
  }

  display_blanking_off(display_dev);  

  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  
  
  state_machine_init();

  char c = 0;
  int ret;
  while (1) {
    //must call periodically to render changes
    ret = state_machine_run();
    k_msleep(SLEEP_MS);
    // scene1.runFunc(&scene1, &c);
    // scene2.runFunc(&scene2, &c);

 

  }
  return 0;
}
