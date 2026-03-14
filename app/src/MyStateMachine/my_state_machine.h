/**
 * @file  my_state_machine.h
 * */
#include "scenes.h"
#include <lvgl.h>

#ifndef MY_STATE_MACHINE_H
#define MY_STATE_MACHINE_H

//"typedef" makes it so that you don't have to write "sturct" next to the name
//each time when intiializing
typedef struct {
  // context variable for machines state
  struct smf_ctx ctx;

  uint8_t levelCount;
  uint8_t lives;
  scene *activeScene;

} game_state_t;

void state_machine_init();

int state_machine_run();

// runs lv_timer_handler a specified number of times
void lv_load_changes(int loops);

// used to change the speed of the game
extern float time_multiplier;

#endif // MY STATE MACHINE H
