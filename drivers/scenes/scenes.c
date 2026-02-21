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
  

void sceneInit(scene* scene, lv_obj_t * screen){
    scene->parent = lv_obj_create(screen);
    lv_obj_set_size(scene->parent, LV_PCT(100), LV_PCT(100));
}

lv_obj_t* getChild(scene* parentScene, char* childName){
  return lv_obj_get_child_by_name(parentScene->parent, childName);
  
} 
