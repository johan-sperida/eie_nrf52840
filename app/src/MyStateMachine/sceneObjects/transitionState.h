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


void transition_enter (void* v_scene, void* v_game_state_object);

int transition_run (void* v_scene, void* v_activeMinigame);