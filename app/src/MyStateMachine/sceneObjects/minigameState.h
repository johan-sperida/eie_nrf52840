
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
#include "MyStateMachine/my_state_machine.h"

typedef struct {
    scene* minigame;
    char name[16];
    char controls[16];

} minigameStateScene;


void count_enter(void* v_scene, void* o);

int count_run(void* v_scene, void* o);

void align_enter(void* v_scene, void* o);

int align_run(void* v_scene, void* o);