/**
 * @file my_state_machine.c
 * 
 */

 #include <zephyr/smf.h>

 #include "BTN.h"
 #include "LED.h"
 #include "my_state_machine.h"

//function prototypes
static void led_on_state_entry(void* o);
static enum smf_state_result led_on_state_run(void* o);
static void led_off_state_entry(void* o);
static enum smf_state_result led_off_state_run(void* o);

//Typedefs
enum led_state_machine_states {
    LED_ON_STATE,
    LED_OFF_STATE
};

//"typedef" makes it so that you don't have to write "sturct" next to the name each time when intiializing
typedef struct {
    //context variable for machines state
    struct smf_ctx ctx;

    uint16_t count;
} led_state_object_t;

//Local vars
//seems to define what each state is
static const struct smf_state led_states[]= {
    [LED_ON_STATE] = SMF_CREATE_STATE(led_on_state_entry, led_on_state_run, NULL, NULL, NULL),
    [LED_OFF_STATE] = SMF_CREATE_STATE(led_off_state_entry, led_off_state_run, NULL, NULL, NULL)
};

//a struct to keep track of state?
static led_state_object_t led_state_object;

//runs on intialization
void state_machine_init(){
    led_state_object.count = 0;
    //set some initial state with the state object
    smf_set_initial(SMF_CTX(&led_state_object), &led_states[LED_ON_STATE]);
}


int state_machine_run(){
    //poll the state from the object
    //updates based on states
    return smf_run_state(SMF_CTX(&led_state_object));
}

static void led_on_state_entry(void* o) {
    LED_set(LED0, LED_ON);
}

static enum smf_state_result led_on_state_run(void* o) {
    //constantly updates the count val
    if (BTN_check_clear_pressed(BTN0)) {

        //switches to off state
        smf_set_state(SMF_CTX(&led_state_object), &led_states[LED_OFF_STATE]);
    } 
    return SMF_EVENT_HANDLED;
}

static void led_off_state_entry(void* o) {
    LED_set(LED0, LED_OFF);
}

static enum smf_state_result led_off_state_run(void* o) {
    if (BTN_check_clear_pressed(BTN1)) {

        smf_set_state(SMF_CTX(&led_state_object), &led_states[LED_ON_STATE]);
    }
    return SMF_EVENT_HANDLED;
}
