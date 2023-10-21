#include <Arduino.h>
#include <avr/sleep.h>
#include <assert.h>
#include "core.h"
#include "config.h"
#include "console.h"
#include "input.h"

#define MAX_WAIT_TIME 10000
#define TOGGLE_TIME 300

#define T1 500

static unsigned long start_wait_time;
static unsigned long relative_wait_time;
static byte led_state = LOW;

static byte score = 0;
static int f = 20;

static void _led_control_check_off(void);
static void _led_control_turn_off(void);
static void _led_control_turn_on(void);
static void _led_control_toggle(void);

bool update_difficulty_level(void) {
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

void intro(void) {
  assert(led_state == LOW);
  start_wait_time = millis();
  Serial.println(F("Welcome to the Restore the Light Game. Press Key B1 to Start."));
  _led_control_turn_on();
  game_state = GAME_WAIT;
  relative_wait_time = start_wait_time;
}

void wait_loop(void) {
  const unsigned long current_loop_time = millis();
  if (current_loop_time - relative_wait_time >= TOGGLE_TIME) {
    _led_control_toggle();
    relative_wait_time = millis();
  }
  // Always works avoiding overflow thanks to unsigned int "wrap around" behavior
  if (current_loop_time - start_wait_time >= MAX_WAIT_TIME) {
    game_state = GAME_SLEEP;
  }
  // TODO: verify if to let as now or change
  // I chose to delay sleep status while player is selecting the difficulty level of the game so as to let the player choose with calm.
  if (update_difficulty_level()) {
    start_wait_time = millis();
  }
  if (is_button_pressed(PIN_1)) {
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
  console_log(String("Score = ") + score);
  turn_leds_on();
  delay(T1);
  game_state = GAME_GENERATE_PATTERN;
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