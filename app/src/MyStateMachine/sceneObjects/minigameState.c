#include "minigameState.h"
#include "MyStateMachine/my_state_machine.h"
#include <stdio.h>
#include <zephyr/random/random.h>

#define COUNT_TIME_MULT 1
#define SPAM_TIME_MULT 1.5

static void progress_bar_init(lv_obj_t *parent) {
  lv_obj_t *progress_bar = lv_obj_create(parent);
  lv_obj_set_style_bg_color(progress_bar, lv_color_hex(0x00FF00), LV_PART_MAIN);
  lv_obj_set_size(progress_bar, LV_PCT(100), LV_PCT(1));
  lv_obj_align(progress_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_name(progress_bar, "progress_bar");
}

static void progress_bar_update(lv_obj_t *parent, int height) {

  lv_obj_set_size(getChild(parent, "progress_bar"), LV_PCT(100),
                  LV_PCT(height));
}

static void timer_init(lv_obj_t *parent, float game_time_mult) {
  lv_obj_t *timer = lv_label_create(parent);
  char buf[64];
  sprintf(buf, "%d", (int)(5000 * time_multiplier * game_time_mult));
  lv_label_set_text(timer, buf);
  lv_obj_set_name(timer, "timer");
  lv_obj_align(timer, LV_ALIGN_TOP_RIGHT, 0, 0);
}

static int timer_run(lv_obj_t *parent, float game_time_mult, int start) {
  int timeDiff = k_uptime_get() - start;
  int totalTime = 5000 * time_multiplier * game_time_mult;
  char buf[64];
  sprintf(buf, "%d", (int)(totalTime - timeDiff));
  lv_label_set_text(getChild(parent, "timer"), buf);
  printk("%f", time_multiplier);
  return totalTime - timeDiff;
}

// recieve button outputs here
static int count_output = 0;

static void count_button_callback(lv_event_t *event) {
  lv_obj_t *data_obj = (lv_obj_t *)lv_event_get_user_data(event);
  int *val = (int *)lv_data_obj_get_data_ptr(data_obj);
  count_output = *val;
}

void count_enter(void *v_scene, void *o) {
  scene *p_scene = v_scene;
  lv_obj_t *parent = p_scene->parent;
  // list of values to have randomized order
  int vals[4] = {1, 2, 3, 4};

  // randomize the order
  // thanks google
  for (int i = 3; i > 0; i--) {
    int j = sys_rand32_get() % (i + 1);
    int tmp = vals[i];
    vals[i] = vals[j];
    vals[j] = tmp;
  }

  timer_init(parent, COUNT_TIME_MULT);
  progress_bar_init(parent);

  for (int i = 0; i < 4; i++) {
    lv_obj_t *ui_btn = lv_button_create(parent);
    // place the buttons in a 2x2 grid in the center of the screen
    // matching the orientations of the LEDs on the board
    lv_obj_align(ui_btn, LV_ALIGN_CENTER, 50 * (i % 2 ? 1 : -1),
                 50 * (i < 2 ? -1 : 1));
    lv_obj_t *button_label = lv_label_create(ui_btn);
    lv_obj_set_size(ui_btn, LV_PCT(30), LV_PCT(30));
    char label_text[4];
    snprintf(label_text, 4, "%d", vals[i]);
    lv_label_set_text(button_label, label_text);
    lv_obj_align(button_label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *data_obj =
        lv_data_obj_create_alloc_assign(ui_btn, &(vals[i]), sizeof(int));
    lv_obj_add_event_cb(ui_btn, count_button_callback, LV_EVENT_CLICKED,
                        data_obj);
  }

  // placing this here as there may be inputs before count_run starts
  count_output = 0;

  lv_screen_load_anim(p_scene->screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 1, 1, true);
}

int count_run(void *v_scene, void *o) {
  scene *p_scene = v_scene;
  lv_obj_t *parent = p_scene->parent;

  int i = 0;

  // timer
  int start = k_uptime_get();

  while (true) {
    progress_bar_update(parent, (int)100 / 4 * i);
    lv_load_changes(1);

    if (count_output == i + 1) {
      printk("\ncurrent Index %d\n\ncurrent count_output %d\n", i,
             count_output);
      i += 1;

      // if you mess up the order
    } else if (count_output != i) {
      i = 0;
      count_output = 0;
    }

    // win condition (iterate through all the buttons in the right order)
    if (i > 3)
      return 0;

    // Lose if you take too long!
    // Update the timer
    if (timer_run(p_scene->parent, COUNT_TIME_MULT, start) <= 0)
      return 1;
  }
}

static int spam_output = 0;

// just increment the button with each button press
static void spam_button_callback(lv_event_t *event) { spam_output++; }

void spam_enter(void *v_scene, void *o) {
  scene *p_scene = v_scene;
  lv_obj_t *parent = p_scene->parent;

  progress_bar_init(parent);

  lv_obj_t *spam_btn = lv_button_create(parent);
  lv_obj_align(spam_btn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(spam_btn, LV_PCT(50), LV_PCT(50));

  lv_obj_t *button_label = lv_label_create(spam_btn);
  lv_label_set_text(button_label, "Spam Me");
  lv_obj_align(button_label, LV_ALIGN_CENTER, 0, 0);

  lv_obj_add_event_cb(spam_btn, spam_button_callback, LV_EVENT_CLICKED, NULL);

  timer_init(parent, SPAM_TIME_MULT);

  // here so it doesn't wipe inputs before spam_run
  spam_output = 0;

  lv_screen_load_anim(p_scene->screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 1, 1, true);
}

int spam_run(void *v_scene, void *o) {
  scene *p_scene = v_scene;
  lv_obj_t *parent = p_scene->parent;

  int start = k_uptime_get();
  timer_run(parent, COUNT_TIME_MULT, start);

  while (true) {
    progress_bar_update(parent, (int)100 / 11 * spam_output);
    lv_load_changes(1);
    // win if yo press the button a lot
    if (spam_output > 10) {
      return 0;
    }

    // lose if you take too long
    if (timer_run(parent, SPAM_TIME_MULT, start) <= 0) {
      return 1;
    }
  }
}

void align_enter(void *v_scene, void *o) {}

int align_run(void *v_scene, void *o) { return 1; }