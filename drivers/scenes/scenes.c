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
  

void sceneInit(scene* p_scene){
  if (!lv_obj_is_valid(p_scene->screen)){
    p_scene->screen = lv_obj_create(NULL);
    p_scene->parent = lv_obj_create(p_scene->screen);
    lv_obj_set_size(p_scene->parent, LV_PCT(100), LV_PCT(100));
    p_scene->enterFunc(p_scene, NULL);
  }
    
}

void reset(scene* p_scene){
  lv_obj_delete(p_scene->parent);
  p_scene->parent = lv_obj_create(p_scene->screen);
}

void sceneClose(scene* p_scene){
  lv_obj_delete(p_scene->screen);
}

lv_obj_t* getChild(lv_obj_t* parent, char* childName){
  return lv_obj_get_child_by_name(parent, childName);
  
} 
