#include "lossState.h"
#include "MyStateMachine/my_state_machine.h"
#include "scenes.h"

void loss_enter(void *v_scene, void *v_game_state_object) {
  scene *p_scene = v_scene;
  game_state_t *game_state_object = v_game_state_object;

  lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFF0000), 0); // RED

  lv_obj_t *labelLoss = lv_label_create(p_scene->parent);
  char label_text[64];
  snprintf(label_text, 64,
           "You Lose!\n\nPress the restart button. \n\nLevel Reached: %d",
           (int)game_state_object->levelCount);
  lv_label_set_text(labelLoss, label_text);
  lv_obj_align(labelLoss, LV_ALIGN_CENTER, 0, 0);

  lv_screen_load_anim(p_scene->screen, LV_SCR_LOAD_ANIM_FADE_IN, 0, 0, true);
}