#include "scenes.h"

typedef struct {
    scene* minigame;
    char name[16];
    char controls[16];

} minigameStateScene;


void count_enter(void* v_scene, void* o);

int count_run(void* v_scene, void* o);

void align_enter(void* v_scene, void* o);

int align_run(void* v_scene, void* o);