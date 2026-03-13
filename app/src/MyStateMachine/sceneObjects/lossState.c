#include "lossState.h"

void loss_enter(void* v_scene, void* o){
  scene* p_scene = v_scene;

  lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFF0000), 0);  // RED

  lv_obj_t* labelMenu = lv_label_create(p_scene->parent);
  lv_label_set_text(labelMenu, "You Lose\n\nPress the restart button");
  lv_obj_align(labelMenu, LV_ALIGN_CENTER, 0, 0);

  lv_screen_load_anim(p_scene->screen,LV_SCR_LOAD_ANIM_FADE_IN,0,0,true);
}