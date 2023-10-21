/**
 * ESIOT a.y. 2022/2023
 * 
 * ASSIGNMENT 1 - CATCH THE LED PATTERN
 * 
 * Author: Enrico Marchionni
*/
#include "config.h"
#include "core.h"

void setup() {
  setup_config();
}

void loop() {
  switch (game_state) {
    case GAME_INTRO:
      intro();
      break;
    case GAME_WAIT:
      wait_loop();
      break;
    case GAME_SLEEP:
      sleep();
      break;
    case GAME_MATCH_INIT:
      match_init();
      break;
    // case GAME_GENERATE_PATTERN:
      
    //   break;
    // case GAME_GUESS_PATTERN:
      
    //   break;
    // case DEFEAT:
      
    //   break;
    // case VICTORY:
    
    //   break;
  }
}
