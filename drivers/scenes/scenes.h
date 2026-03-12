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

#pragma once

typedef struct{
    lv_obj_t* parent;
    lv_obj_t* screen;
    
    //must be specific to each scene
    void (*enterFunc)(void* v_scene,void* obj); //creates objects 
    void (*exitFunc)(void* v_scene,void* obj);   //deletes screen
    int (*runFunc)(void* v_scene, void* obj); //does logic
} scene;

void sceneInit(scene* scene, bool delete_prev_screen);

void sceneClose(scene* scene);

void reset(scene* scene);

lv_obj_t* getChild(lv_obj_t* parent, char* childName);

