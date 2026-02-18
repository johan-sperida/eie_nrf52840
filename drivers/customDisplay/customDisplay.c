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




int displayInit(displayStruct* display){
    display->display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    display->screen = NULL;

    if (!device_is_ready(display->display_dev)) {
        return -1;
    }

    display->screen = lv_screen_active();

    if (display->screen == NULL) {
      return -1;
    }

    display_blanking_off(display->display_dev);  

    return 0;
}

void textCreate(displayStruct* display, int index){
    display->objArray[index] = lv_label_create(display->screen);
}

void textUpdate(displayStruct* display, int index, const char* text){
    lv_label_set_text(display->objArray[index], text);
}

