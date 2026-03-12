/**
 * @file my_state_machine.c
 * 
 */

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
#include "my_state_machine.h"

#include "sceneObjects/transitionScreens.h"
#include "sceneObjects/menuState.h"

//Forward declarations for scene functions
static void scene1_enter(void* v_scene, void* o);
static int scene1_run(void* v_scene, void* o);

//Forward declarations for state machine functions
static void menu_state_entry(void* o);
static enum smf_state_result menu_state_run(void* o);
static void menu_state_exit(void* o);
static void transition_state_entry(void* o);
static enum smf_state_result transition_state_run(void* o);
static void transition_state_exit(void* o);
static void minigame_state_entry(void* o);
static enum smf_state_result minigame_state_run(void* o);
static void minigame_state_exit(void* o);
static void loss_state_entry(void* o);
static enum smf_state_result loss_state_run(void* o);
static void loss_state_exit(void* o);

//Typedefs
enum game_states {
    MENU_STATE,
    TRANSITION_STATE,
    MINIGAME_STATE,
    LOSS_STATE
};

//"typedef" makes it so that you don't have to write "sturct" next to the name each time when intiializing
typedef struct {
    //context variable for machines state
    struct smf_ctx ctx;

    uint8_t levelCount;
    uint8_t lives;
    scene topScene;

} game_state_t;

// Local vars
// Define the state table using the enum values and naming conventions
static const struct smf_state game_states[] = {
    [MENU_STATE] = SMF_CREATE_STATE(menu_state_entry, menu_state_run, menu_state_exit, NULL, NULL),
    [TRANSITION_STATE] = SMF_CREATE_STATE(transition_state_entry, transition_state_run, transition_state_exit, NULL, NULL),
    [MINIGAME_STATE] = SMF_CREATE_STATE(minigame_state_entry, minigame_state_run, minigame_state_exit, NULL, NULL),
    [LOSS_STATE] = SMF_CREATE_STATE(loss_state_entry, loss_state_run, loss_state_exit, NULL, NULL),
};

//a struct to keep track of state?
static game_state_t game_state_object;


//Scenes -------------------------------------------------------------

static scene menuStateScene;

static scene transitionStateScene;

//The first minigame scene
static scene minigameStateScene;

//The first transition scene
static scene transitionScene;

static scene scene1 = {
    .enterFunc = &scene1_enter,
    .runFunc   = &scene1_run,
};

static scene menu = {
    .enterFunc = &menu_enter,
};

static scene redTransition = {
    .enterFunc = &redTransition_enter,
};

static scene greenTransition = {
    .enterFunc = &greenTransition_enter,
};


static void scene1_enter(void* v_scene, void* o){
    scene* p_scene = v_scene;

    lv_obj_set_style_bg_color(p_scene->parent, lv_color_hex(0xFF0000), 0); 

    lv_obj_t* rect1 = lv_obj_create(p_scene->parent);
    lv_obj_set_style_bg_color(rect1, lv_color_hex(0xFFFF00), 0); 
    lv_obj_set_name(rect1, "rect1");

    lv_screen_load_anim(p_scene->screen,LV_SCR_LOAD_ANIM_MOVE_LEFT,1,1,true);

}

static int scene1_run(void* v_scene, void* o) {
    scene* p_scene = v_scene;
    int* val = o;
    uint8_t value = (*((uint8_t*)val));
    
    lv_obj_set_size(getChild(p_scene->parent, "rect1"), LV_PCT(value), LV_PCT(value));
    return value;
}




/*STATE FUNCS -------------------------------------------------------------------*/

                                                        // Menu
static void menu_state_entry(void* o) {
    sceneInit(&menu);
}

//only runs once the board is connected via BLE
static enum smf_state_result menu_state_run(void* o) {
    smf_set_state(SMF_CTX(&game_state_object), &game_states[TRANSITION_STATE]);
  
    return SMF_EVENT_HANDLED;
}


//on successful connection
static void menu_state_exit(void* o) {
    sceneInit(&greenTransition);
}

                                                        //Transition
static void transition_state_entry(void* o) {
}

static enum smf_state_result transition_state_run(void* o) {
    return SMF_EVENT_HANDLED;
}

static void transition_state_exit(void* o) {
}

                                                        //Game
static void minigame_state_entry(void* o) {
    sceneInit(&scene1);
}

static enum smf_state_result minigame_state_run(void* o) {
    scene1.runFunc(&scene1,&game_state_object.levelCount);

    //change to other state
    if (BTN_check_clear_pressed(BTN1)){
        smf_set_state(SMF_CTX(&game_state_object), &game_states[MENU_STATE]);
    } else {
        BTN_check_clear_pressed_all();
    }
    return SMF_EVENT_HANDLED;
}

static void minigame_state_exit(void* o) {
}

                                                        //Loss
static void loss_state_entry(void* o) {
}

static enum smf_state_result loss_state_run(void* o) {
    return SMF_EVENT_HANDLED;
}

static void loss_state_exit(void* o) {
}

//runs on intialization
void state_machine_init(){
    game_state_object.levelCount = 0;
    game_state_object.lives = 4;

    //set some initial state with the state object
    smf_set_initial(SMF_CTX(&game_state_object), &game_states[MENU_STATE]);
}

int state_machine_run(){
    //poll the state from the object
    //updates based on states
    lv_timer_handler();
    game_state_object.levelCount++;

    if (game_state_object.levelCount >= 100) game_state_object.levelCount=0;


    return smf_run_state(SMF_CTX(&game_state_object));
}


