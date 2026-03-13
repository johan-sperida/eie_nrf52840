#include "minigameState.h"
#include "MyStateMachine/my_state_machine.h"
#include <zephyr/random/random.h>
#include <stdio.h>
typedef struct {
    int* p_output;
    int val;

} countButton;
//recieve button outputs here
static int output = 0;


void count_button_callback(lv_event_t *event) {
    lv_obj_t *data_obj = (lv_obj_t *)lv_event_get_user_data(event);
    countButton* button = (countButton *)lv_data_obj_get_data_ptr(data_obj);
    *(button->p_output) = button->val;
}

void count_enter(void* v_scene, void* o){
    scene* p_scene = v_scene;
    //list of values to have randomized order
    int vals[4] = {1, 2, 3, 4};

    //randomize the order
    //thanks google
    for (int i = 3; i > 0; i--) {
        int j = sys_rand32_get() % (i + 1);
        int tmp = vals[i];
        vals[i] = vals[j];
        vals[j] = tmp;
    }

    countButton data_struct = {
        .p_output = &output,
        .val = 0
    };

    lv_obj_t* timer = lv_label_create(p_scene->parent);
    char buf[64];
    sprintf(buf,"%d",(int) (5000 * time_const));
    lv_label_set_text(timer, buf);
    lv_obj_set_name(timer,"timer");
    lv_obj_align(timer, LV_ALIGN_TOP_RIGHT, 0, 0);

    for (int i = 0; i < 4; i++) {
        lv_obj_t *ui_btn = lv_button_create(p_scene->parent);
        // place the buttons in a 2x2 grid in the center of the screen
        // matching the orientations of the LEDs on the board
        lv_obj_align(ui_btn, LV_ALIGN_CENTER, 50 * (i % 2 ? 1 : -1), 50 * (i < 2 ? -1 : 1));
        lv_obj_t *button_label = lv_label_create(ui_btn);
        lv_obj_set_size(ui_btn, LV_PCT(30), LV_PCT(30));
        char label_text[10];
        snprintf(label_text, 10, "%d", vals[i]);
        lv_label_set_text(button_label, label_text);
        lv_obj_align(button_label, LV_ALIGN_CENTER, 0, 0);

        //assign new value to struct and pass it in
        data_struct.val = vals[i];

        lv_obj_t *data_obj = lv_data_obj_create_alloc_assign(ui_btn, &data_struct, sizeof(countButton));
        lv_obj_add_event_cb(ui_btn, count_button_callback, LV_EVENT_CLICKED, data_obj);
    }

    lv_screen_load_anim(p_scene->screen,LV_SCR_LOAD_ANIM_MOVE_LEFT,1,1,true);
}


int count_run(void* v_scene, void* o){
    scene* p_scene = v_scene;

    //so that the index starts at 0
    int i = 0;
    int timediff = 0;
    
    //timer
    int start = k_uptime_get();

    while(true){
        lv_load_changes(1);

        if(output == i + 1){
            printk("\ncurrent Index %d\n\ncurrent output %d\n", i, output);
            i += 1;
        
        //if you mess up the order
        } else if (output == i) {
            
        } else {
            i = 0;
            output = 0;
        }

        //win condition (iterate through all the buttons in the right order)
        if(i > 3)
            return 0;


        //Set timer
        
        timediff = k_uptime_get() - start;
        char buf[64];
        sprintf(buf,"%d", (int) (5000 * time_const - timediff));
        lv_label_set_text(getChild(p_scene->parent, "timer"), buf); 
        //Lose if you take too long!
        printk("%f", time_const);
        if( timediff > 5000 * time_const)
            return 1;
    }
    
}

void align_enter(void* v_scene, void* o){

}

int align_run(void* v_scene, void* o){
    return 1;
}