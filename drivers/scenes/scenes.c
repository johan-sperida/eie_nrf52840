#include <inttypes.h>

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <lvgl.h>

#include "BTN.h"
#include "LED.h"
#include "lv_data_obj.h"
#include "scenes.h"

#define SLEEP_MS 1

void lv_load_changes(int loops) {
  for (int i = 0; i < loops; i++) {
    lv_timer_handler();
    k_msleep(1);
  }
}

void scene_Init(scene *p_scene) {
  if (!lv_obj_is_valid(p_scene->screen)) {
    p_scene->screen = lv_obj_create(NULL);
    p_scene->parent = lv_obj_create(p_scene->screen);
    lv_obj_set_size(p_scene->parent, LV_PCT(100), LV_PCT(100));
  } else {
    lv_obj_delete(p_scene->screen);
    scene_Init(p_scene);
  }
}

void scene_Load(scene *p_scene, bool delete_prev_screen) {
  scene_Init(p_scene);
  p_scene->enterFunc(p_scene, &delete_prev_screen);
  // have to run this to allow the screen transition to finish
  lv_load_changes(100);
}

void reset(scene *p_scene) {
  lv_obj_delete(p_scene->parent);
  p_scene->parent = lv_obj_create(p_scene->screen);
}

void sceneClose(scene *p_scene) { lv_obj_delete(p_scene->screen); }

lv_obj_t *getChild(lv_obj_t *parent, char *childName) {
  return lv_obj_get_child_by_name(parent, childName);
}
