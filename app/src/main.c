/*
 * main.c
 */

/* IMPORTS
 * --------------------------------------------------------------------------------------
 */

#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"
#include "MyStateMachine/my_state_machine.h"
#include "ble_manager.h"

/* MACROS
 * ---------------------------------------------------------------------------------------
 */

#define SLEEP_MS 1

/* VARIABLES
 * -----------------------------------------------------------------------------------
 */

static const struct device *display_dev;

/* MAIN
 * -----------------------------------------------------------------------------------------
 */

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

  int err = ble_init();
  if (err) {
    return 0;
  }

  while (1) {
    state_machine_init();

    // Load the first screen
    lv_timer_handler();

    // commenting this out until functional BLE game is in place

    // ble_start_scanning();

    // while(!ble_is_connected()){
    //   printk("Searching\n");
    //   k_msleep(SLEEP_MS * 5);
    // }

    int ret;
    while (1) { // ble_is_connected()
      // must call periodically to render changes
      ret = state_machine_run();
      k_msleep(SLEEP_MS);
    }

    // To disconnect: bt_conn_disconnect(conn,
    // BT_HCI_ERR_REMOTE_USER_TERM_CONN);
  }
  return 0;
}