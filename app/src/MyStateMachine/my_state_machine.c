/**
 * @file my_state_machine.c
 * 
 */

#include <zephyr/smf.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/display.h>
#include <zephyr/device.h>
#include <zephyr/random/random.h>


#include <lvgl.h>

#include "BTN.h"
#include "LED.h"
#include "lv_data_obj.h"
#include "scenes.h"
#include "my_state_machine.h"

#include "sceneObjects/transitionScreens.h"
#include "sceneObjects/menuState.h"
#include "sceneObjects/minigameState.h"
#include "sceneObjects/transitionState.h"

#define SLEEP_MS 1
#define TIME_CONST 1
#define MINIGAMES_AMOUNT 2

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

static scene transitionStateScene = {
    .enterFunc = transition_enter,
    .runFunc =transition_run
};

//minigames ---------------------------------
static scene countMinigameScene = {
    .enterFunc = &count_enter,
    .runFunc = &count_run
};

static minigameStateScene countMinigame = {
    .minigame = &countMinigameScene,
    .name = "Count!",
    .controls = "[Use Screen]"
};

static scene alignMinigameScene = {
    .enterFunc = &align_enter,
    .runFunc = &align_run
};

static minigameStateScene alignMinigame = {
    .minigame = &alignMinigameScene,
    .name = "Align!",
    .controls = "[Use Sensor]"
};

//The pool from which to pick a minigame
static minigameStateScene* minigamePool[MINIGAMES_AMOUNT] = {
    &countMinigame, 
    &alignMinigame
};
//---------------------------------

static scene menuStateScene = {
    .enterFunc = &menu_enter,
    .runFunc = &menu_run
};

static scene redTransition = {
    .enterFunc = &red_Transition_Screen_enter,
};

static scene greenTransition = {
    .enterFunc = &green_Transition_Screen_enter,
};


void lv_load_changes(int loops) {
    for (int i = 0; i < loops; i++){
        lv_timer_handler();
        k_msleep(1);
    }
}



/*STATE FUNCS -------------------------------------------------------------------*/

                                                        // Menu
static void menu_state_entry(void* o) {
    printk("enter menu <-------------------\n");
    
    
}

//only runs once the board is connected via BLE
static enum smf_state_result menu_state_run(void* o) {
    printk("running menu <------------------\n");
    //gotta be here otherwise it would run too upon startup
    scene_Load(&greenTransition, false);
    
    menuStateScene.runFunc(&menuStateScene, NULL);

    smf_set_state(SMF_CTX(&game_state_object), &game_states[TRANSITION_STATE]);
    return SMF_EVENT_HANDLED;
}


static void menu_state_exit(void* o) {
    printk("exit menu <------------------\n");
}

                                                        //Transition
static void transition_state_entry(void* o) {
    printk("enter transition <------------------\n");
    scene_Init(&transitionStateScene);
    transitionStateScene.enterFunc(&transitionStateScene, &game_state_object);
    k_msleep(1000*SLEEP_MS*TIME_CONST);
}

static enum smf_state_result transition_state_run(void* o) {
    printk("running transition <------------------\n");

    //picking the right minigame
    int minigame_Index = (sys_rand32_get() % (MINIGAMES_AMOUNT));
    minigameStateScene* p_activeMinigame =  minigamePool[minigame_Index];

    transitionStateScene.runFunc(&transitionStateScene, p_activeMinigame);
    return SMF_EVENT_HANDLED;
}

static void transition_state_exit(void* o) {
    printk("exit transition <------------------\n");
}

                                                        //Game
static void minigame_state_entry(void* o) {
    printk("enter minigame <------------------\n");
}

static enum smf_state_result minigame_state_run(void* o) {
    printk("running minigame <------------------\n");
 
    return SMF_EVENT_HANDLED;
}

static void minigame_state_exit(void* o) {
    printk("exit minigame <------------------\n");
}

                                                        //Loss
static void loss_state_entry(void* o) {
    printk("enter loss <------------------\n");
}

static enum smf_state_result loss_state_run(void* o) {
    printk("running loss <------------------\n");
    return SMF_EVENT_HANDLED;
}

static void loss_state_exit(void* o) {
    printk("exit loss <------------------\n");
}

//runs on intialization
void state_machine_init(){
    game_state_object.levelCount = 0;
    game_state_object.lives = 4;

    printk("init state machine <-------------------\n");
    scene_Load(&menuStateScene, true);

    //set some initial state with the state object
    smf_set_initial(SMF_CTX(&game_state_object), &game_states[MENU_STATE]);
}

int state_machine_run(){
    //poll the state from the object
    //updates based on states
    lv_timer_handler();
    return smf_run_state(SMF_CTX(&game_state_object));
}


