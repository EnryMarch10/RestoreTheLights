#include <Arduino.h>
#include <avr/sleep.h>
#include <assert.h>
#include "core.h"
#include "config.h"
#include "console.h"
#include "input.h"
#include "wait.h"

#define MAX_WAIT_TIME 10000
#define TOGGLE_TIME 300

#define T1 2000
#define MAX_T2 8000
#define MAX_T3 20000
#define MIN_T2 1000
#define MIN_T3 4000

#define RELOAD_TIME 10000

extern volatile byte pattern[NPINS];

volatile void (*match_result)(void) = NULL;

static unsigned int t2 = MAX_T2;
static unsigned int t3 = MAX_T3;

static unsigned long start_wait_time;
static unsigned long relative_wait_time;
static byte led_state = LOW;

static unsigned long start_match_time;
static byte score = 0;
static int f = 20;

static byte i_pattern;
static unsigned long start_pattern_time;
static unsigned long sleep_pattern_time;

static void _led_control_check_off(void);
static void _led_control_turn_off(void);
static void _led_control_turn_on(void);
static void _led_control_toggle(void);
static bool _update_difficulty_level(void);
static void _end(void);

void intro(void) {
  assert(led_state == LOW);
  Serial.println(F("Welcome to the Restore the Light Game. Press Key B1 to Start."));
  _led_control_turn_on();
  game_state = GAME_START;
  start_wait_time = millis();
  relative_wait_time = start_wait_time;
}

void start_loop(void) {
  const unsigned long current_loop_time = millis();
  if (current_loop_time - relative_wait_time >= TOGGLE_TIME) {
    _led_control_toggle();
    relative_wait_time = millis();
  }
  // Always works avoiding overflow thanks to unsigned int "wrap around" behavior
  if (current_loop_time - start_wait_time >= MAX_WAIT_TIME) {
    game_state = GAME_SLEEP;
  }
  // I chose to delay sleep status while player is selecting the difficulty level of the game so as to let the player choose with calm.
  if (_update_difficulty_level()) {
    start_wait_time = millis();
  }
  if (current_loop_time - start_wait_time >= 2 * TOGGLE_TIME && is_button_pressed(PIN_1)) {
    game_state = GAME_MATCH_INIT;
  }
}

void sleep(void) {
  _led_control_turn_off();
  console_log(F("Going to sleep..."));
  enable_all_wakeup_interrupts();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  console_log(F("Woke up from sleeping..."));
  game_state = GAME_INTRO;
}

void match_init(void) {
  _led_control_check_off();
  Serial.println(F("Go!"));
  leds_turn_on();
  console_log(String("T1 time ") + T1 + " will occur");
  delay(T1);
  game_state = GAME_MATCH_GENERATE_PATTERN;
}

void match_generate_pattern(void) {
  randomSeed(analogRead(FLOATING_PIN));
  for (byte i = 0; i < NPINS; i++) {
    const byte n = random(0, NPINS);
    if (i != n) {
      pattern[n] ^= pattern[i];
      pattern[i] ^= pattern[n];
      pattern[n] ^= pattern[i];
    }
  }
  game_state = GAME_MATCH_LEDS_OFF;
  sleep_pattern_time = t2 / NPINS;
  i_pattern = 0;
  start_pattern_time = millis();
}

void match_leds_off_loop(void) {
  const unsigned long current_time = millis();
  if (current_time - start_pattern_time > sleep_pattern_time) {
    led_turn_off(pattern[i_pattern]);
    i_pattern++;
    start_pattern_time = millis();
  }
  if (i_pattern == NPINS) {
    game_state = GAME_MATCH_GUESS_PATTERN;
    enable_all_pattern_interrupts();
    start_match_time = millis();
  }
}

void match_guess_pattern_loop(void) {
  const unsigned long current_time = millis();
  if (current_time - start_match_time > t3) {
    console_log(String("Max time ") + t3 / 1000.0 + " s expired");
    disable_all_pattern_interrupts();
    game_state = GAME_OVER;
  } else {
    noInterrupts();
    const void (*handler)(void) = match_result;
    interrupts();
    if (handler != NULL) {
      (*handler)();
    }
  }
}

void pattern_match(void) {
  game_state = GAME_MATCH_COMPLETED;
}

void pattern_unmatch(void) {
  game_state = GAME_OVER;
}

void victory(void) {
  t2 = max(MIN_T2, t2 - f);
  t3 = max(MIN_T3, t3 - f);
  score += 1;
  Serial.println(String("New point! Score: ") + score);
  _end();
  game_state = GAME_MATCH_GENERATE_PATTERN;
}

void defeat(void) {
  digitalWrite(LED_PIN_CONTROL, HIGH);
  delay(1000);
  digitalWrite(LED_PIN_CONTROL, LOW);
  Serial.println(String("Game Over. Final Score: ") + score);
  score = 0;
  t2 = MAX_T2;
  t3 = MAX_T3;
  _end();
  leds_turn_off();
  wait_restart(RELOAD_TIME, GAME_INTRO);
}

/* UTILITY */

static void _end(void) {
  // There is no race condition here becouse pattern interrupts are already disabled here
  match_result = NULL;
}

static bool _update_difficulty_level(void) {
  static byte prev_value = 1;
  byte value = read_difficulty_level();
  if (value != prev_value) {
    console_log(String("Difficulty updated to ") + value);
    f = (1 + value) * 100;
    console_log(String("Difficutly factor F = ") + f);
    prev_value = value;
    return true;
  }
  return false;
}

static void _led_control_check_off(void) {
  if (led_state == HIGH) {
    _led_control_turn_off();
  }
}

static void _led_control_turn_off(void) {
  digitalWrite(LED_PIN_CONTROL, LOW);
  led_state = LOW;
}

static void _led_control_turn_on(void) {
  digitalWrite(LED_PIN_CONTROL, HIGH);
  led_state = HIGH;
}

static void _led_control_toggle(void) {
  if (led_state == HIGH) {
    _led_control_turn_off();
  } else /* if (led_state == LOW) */ {
    _led_control_turn_on();
  }
}