#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/display.h>
#include <zephyr/device.h>
#include <zephyr/smf.h>

#include <lvgl.h>

#include "BTN.h"
#include "LED.h"
#include "lv_data_obj.h"

typedef void (*transitionFunc)(void* obj);


typedef struct{
    lv_obj_t* parent;

    //must be specific to each scene
    transitionFunc enterFunc;
    transitionFunc exitFunc;
    enum smf_state_result (*runFunc)(void* obj);
} scene;

void sceneInit(scene* scene, lv_obj_t* screen);

lv_obj_t* getChild(scene* parentScene, char* childName);