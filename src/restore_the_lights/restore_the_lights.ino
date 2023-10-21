/**
 * ESIOT a.y. 2022/2023
 * 
 * ASSIGNMENT 1 - CATCH THE LED PATTERN
 * 
 * Author: Enrico Marchionni
*/
#include "config.h"
#include "core.h"
#include "wait.h"

void setup() {
  setup_config();
}

void loop() {
  switch (game_state) {
    case GAME_INTRO:
      intro();
      break;
    case GAME_START:
      start_loop();
      break;
    case GAME_SLEEP:
      sleep();
      break;
    case GAME_MATCH_INIT:
      match_init();
      break;
    case GAME_MATCH_GENERATE_PATTERN:
      match_generate_pattern();
      break;
    case GAME_MATCH_LEDS_OFF:
      match_leds_off_loop();
      break;
    case GAME_MATCH_GUESS_PATTERN:
      match_guess_pattern_loop();
      break;
    case GAME_MATCH_COMPLETED:
      victory();
      break;
    case GAME_OVER:
      defeat();
      break;
    case GAME_WAIT:
      wait_loop();
      break;
  }
}
