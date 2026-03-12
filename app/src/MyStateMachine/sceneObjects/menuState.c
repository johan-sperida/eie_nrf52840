#include <zephyr/smf.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/display.h>
#include <zephyr/device.h>


#include <lvgl.h>

#include "BTN.h"
#include "LED.h"
#include "lv_data_obj.h"
#include "scenes.h"
#include "MyStateMachine/my_state_machine.h"
#include "menuState.h"


void menu_enter(void* v_scene, void* o){
  scene* p_scene = v_scene;

  lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFF0000), 0);  // RED

  lv_obj_t* labelMenu = lv_label_create(p_scene->parent);
  lv_label_set_text(labelMenu, "Connecting");
  lv_obj_align(labelMenu, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_name(labelMenu, "labelMenu");

  lv_screen_load_anim(p_scene->screen,LV_SCR_LOAD_ANIM_FADE_IN,1,1,true);
}

