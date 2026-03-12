
#include "transitionState.h"
#include "minigameState.h"
#include <stdio.h>

void transition_enter (void* v_scene, void* v_game_state_object){
    scene* p_scene = v_scene;
    game_state_t* game_state_object = v_game_state_object;

    lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFFFFFF), 0);  // WHITE

    lv_obj_t* labelLives = lv_label_create(p_scene->parent);
    char buf[64];
    sprintf(buf,"Level: %d \n\nLives: %d", game_state_object->levelCount, game_state_object->lives);
    lv_label_set_text(labelLives, buf);
    lv_obj_align(labelLives, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_name(labelLives, "labelMenu");

    lv_screen_load_anim(p_scene->screen,LV_SCR_LOAD_ANIM_FADE_IN,0,0,true);

    lv_load_changes(300);
}

int transition_run (void* v_scene, void* v_activeMinigame){
    scene* p_scene = v_scene;
    minigameStateScene* p_activeMinigame = v_activeMinigame;

    lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFFFFFF), 0);  // WHITE

    lv_obj_t* labelMenu = getChild(p_scene->parent, "labelMenu");
    char buf[64];
    sprintf(buf,"%s \n\n%s", p_activeMinigame->name, p_activeMinigame->controls);
    lv_label_set_text(labelMenu, buf);
    lv_obj_align(labelMenu, LV_ALIGN_CENTER, 0, 0);

    lv_load_changes(300);

    return 1;
}