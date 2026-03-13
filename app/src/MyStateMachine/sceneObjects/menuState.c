#include <zephyr/sys/printk.h>
#include "lv_data_obj.h"
#include "scenes.h"
#include "MyStateMachine/my_state_machine.h"
#include "menuState.h"

#define SLEEP_MS 1

void menu_enter(void* v_scene, void* o){
  scene* p_scene = v_scene;

  lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFF0000), 0);  // RED

  lv_obj_t* labelMenu = lv_label_create(p_scene->parent);
  lv_label_set_text(labelMenu, "Connecting...");
  lv_obj_align(labelMenu, LV_ALIGN_CENTER, 0, 0);

  lv_screen_load_anim(p_scene->screen,LV_SCR_LOAD_ANIM_FADE_IN,0,0,true);
}

//for the UI button
void menu_button_callback(lv_event_t *event) {
  lv_obj_t* data_obj = (lv_obj_t *)lv_event_get_user_data(event);
  int* pressed = (int *)lv_data_obj_get_data_ptr(data_obj);
  *pressed = 1;
}

int menu_run(void* v_scene, void* o){
  scene* p_scene = v_scene;

  lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFFFFFF), 0);  // RED

  lv_obj_t *menu_btn = lv_button_create(p_scene->parent);
  // place the buttons in a 2x2 grid in the center of the screen
  // matching the orientations of the LEDs on the board
  lv_obj_align(menu_btn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_t *button_label = lv_label_create(menu_btn);
  lv_label_set_text(button_label, "Start game");
  lv_obj_align(button_label, LV_ALIGN_CENTER, 0, 0);  

  int initial_state = 0;
  lv_obj_t *data_obj = lv_data_obj_create_alloc_assign(menu_btn, &initial_state, sizeof(initial_state));
  lv_obj_add_event_cb(menu_btn, menu_button_callback, LV_EVENT_CLICKED, data_obj);

  int *button_pressed = (int *)lv_data_obj_get_data_ptr(data_obj);

  lv_screen_load_anim(p_scene->screen,LV_SCR_LOAD_ANIM_MOVE_TOP,1,1,true);

  printk("button awaiting %d <----------", *button_pressed);
  while (!(*button_pressed)) {
      lv_load_changes(1);
  }

  printk("button pressed %d <----------", *button_pressed);
  return 1;
}


