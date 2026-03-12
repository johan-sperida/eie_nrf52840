#include "transitionScreens.h"

void greenTransition_enter (void* v_scene, void* o){
    scene* p_scene = v_scene;

    lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0x00FF00), 0); 
    lv_screen_load_anim(p_scene->screen,LV_SCR_LOAD_ANIM_MOVE_TOP,1,1,true);

}

void redTransition_enter (void* v_scene, void* o){
    scene* p_scene = v_scene;

    lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFF0000), 0); 
    lv_screen_load_anim(p_scene->screen,LV_SCR_LOAD_ANIM_MOVE_BOTTOM,1,1,true);

}