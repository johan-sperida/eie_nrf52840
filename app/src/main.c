/*
 * main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

int main(void) {

  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  int result = 0b0;
  

  while(1) {
    if (BTN_check_clear_pressed(BTN0)) {
      result = (result << 1) | 0;
    }
    if (BTN_check_clear_pressed(BTN1)) {
      result = (result << 1) | 1;
    }
    printk("%d.\n", result);

    k_msleep(SLEEP_MS * 100);

    
  }
	return 0;
}
