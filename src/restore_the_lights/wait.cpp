#include <Arduino.h>
#include "wait.h"

static unsigned long wait_time = 0;
static unsigned long start_time = 0;
static GameState state;

void wait_restart(const unsigned long time, const GameState new_state) {
    wait_time = time;
    state = new_state;
    game_state = GAME_WAIT;
    start_time = millis();
}

void wait_loop(void) {
    const unsigned long current_time = millis();
    if (current_time - start_time > wait_time) {
        game_state = state;
    }
}