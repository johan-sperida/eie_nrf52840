#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/display.h>
#include <zephyr/device.h>


#include <lvgl.h>

#include "BTN.h"
#include "LED.h"
#include "lv_data_obj.h"

struct displayStruct {
    const struct device *display_dev;
    lv_obj_t *screen;
    lv_obj_t* objArray[5];
};

typedef struct displayStruct displayStruct;


int displayInit(displayStruct* display);

void textCreate(displayStruct* display, int index);

void textUpdate(displayStruct* display, int index, const char* text);