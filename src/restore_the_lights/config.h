#ifndef ___CONFIG___
#define ___CONFIG___

// #ifndef ___DEBUG___
// #define ___DEBUG___
// #endif

#define LED_PIN_CONTROL 13

#define POT_PIN A0
#define FLOATING_PIN A5

// These constraints refers to both buttons and leds pins
#define PIN_1 0
#define PIN_2 1
#define PIN_3 2
#define PIN_4 3

#define NPINS 4

#define DIFFICULTY_LEVELS 4

typedef enum State {
  GAME_INTRO,
  GAME_START,
  GAME_SLEEP,
  GAME_MATCH_INIT,
  GAME_MATCH_GENERATE_PATTERN,
  GAME_MATCH_LEDS_OFF,
  GAME_MATCH_GUESS_PATTERN,
  GAME_MATCH_COMPLETED,
  GAME_OVER,
  GAME_WAIT
} GameState;

extern GameState game_state;

void setup_config(void);

void leds_turn_on(void);
void leds_turn_off(void);
void led_turn_on(byte i);
void led_turn_off(byte i);

bool is_button_pressed(byte i);

void enable_all_wakeup_interrupts(void);
void disable_all_wakeup_interrupts(void);
void enable_all_pattern_interrupts(void);
void disable_all_pattern_interrupts(void);

#endif
