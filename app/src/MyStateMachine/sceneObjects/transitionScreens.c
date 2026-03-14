#include "transitionScreens.h"
#include "scenes.h"
#include <stdbool.h>

void green_Transition_Screen_enter(void *v_scene, void *o) {
  scene *p_scene = v_scene;
  bool *p_delete_screen = o;

  lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0x00FF00),
                            LV_PART_MAIN);
  lv_screen_load_anim(p_scene->screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 1, 1,
                      *(bool *)p_delete_screen);
}

void red_Transition_Screen_enter(void *v_scene, void *o) {
  scene *p_scene = v_scene;

  lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFF0000),
                            LV_PART_MAIN);
  lv_screen_load_anim(p_scene->screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 1, 1,
                      true);
}