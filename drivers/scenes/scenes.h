#include <inttypes.h>

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/smf.h>
#include <zephyr/sys/printk.h>

#include <lvgl.h>

#include "BTN.h"
#include "LED.h"
#include "lv_data_obj.h"

#pragma once

typedef struct {
  // parent is created on top of the screen as it is a little easier to
  // manipulate
  lv_obj_t *parent;
  lv_obj_t *screen;

  // must be specific to each scene
  void (*enterFunc)(void *v_scene, void *obj); // creates objects
  void (*exitFunc)(void *v_scene, void *obj);  // deletes screen (if necessary)
  int (*runFunc)(void *v_scene, void *obj);    // does logic
} scene;

// loads lv changes
void lv_load_changes(int loops);

// only initializes the scene object
void scene_Init(scene *p_scene);

// runs scene_Init, and loads the scene
void scene_Load(scene *scene, bool delete_prev_screen);

void sceneClose(scene *scene);

void reset(scene *scene);

lv_obj_t *getChild(lv_obj_t *parent, char *childName);
