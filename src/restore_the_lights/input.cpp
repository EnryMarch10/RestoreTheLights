#include <Arduino.h>
#include "console.h"
#include "config.h"

#define BOUNCING_TIME 200

#define ANALOG_VALUES 1024
#define RESULT(value) ((value) / divisor + 1)

extern void pattern_match(void);
extern void pattern_unmatch(void);

// function that determines level passed/game over
extern volatile void (*match_result)(void);

static int _compute_divisor(double value);

// Pattern match utilities
extern volatile byte pattern[NPINS];
static unsigned long last_button_pressed_time[NPINS] = {0};
static byte index = 0;

// Useful to read from analog potentiometer
static const int analog_range = (ANALOG_VALUES / (DIFFICULTY_LEVELS * 2));
static const int divisor = _compute_divisor((double) ANALOG_VALUES / DIFFICULTY_LEVELS);

static unsigned long last_analog_read_time = 0;
static int last_value = -analog_range;
static int last_result = -1;

byte read_difficulty_level(void) {
  const unsigned long current_time = millis();
  const int value = analogRead(POT_PIN);
  if (current_time - last_analog_read_time > BOUNCING_TIME
      && (value <= last_value - analog_range || value >= last_value + analog_range)) {
    const byte result = RESULT(value);
    if (last_result != result) {
      last_value = value;
      last_result = result;
      return result;
    }
    last_analog_read_time = millis();
  }
  return last_result = RESULT(last_value);
}

static void _button_handler(const byte i) {
  const unsigned long current_time = millis();
  if (current_time - last_button_pressed_time[i] > BOUNCING_TIME) {
    last_button_pressed_time[i] = current_time;
    if (i != pattern[NPINS - index - 1]) {
      match_result = &pattern_unmatch;
      disable_all_pattern_interrupts();
      // console_log(String("Interrupt ") + i + " detected DEFEAT");
      return;
    } else if (index == NPINS - 1) {
      led_turn_on(i);
      match_result = &pattern_match;
      disable_all_pattern_interrupts();
      // console_log(String("Interrupt ") + i + " detected VICTORY");
      return;
    }
    led_turn_on(i);
    index++;
  }
}

void btn1_handler(void) {
  _button_handler(PIN_1);
}

void btn2_handler(void) {
  _button_handler(PIN_2);
}

void btn3_handler(void) {
  _button_handler(PIN_3);
}

void btn4_handler(void) {
  _button_handler(PIN_4);
}

void wakeup_handler(void) {
  // console_log(F("Wakeup interrupt called successfully"));
  disable_all_wakeup_interrupts();
}

void clear_pattern_handlers_state(void) {
  index = 0;
}

static int _compute_divisor(double value) {
  return (value - (int) value) == 0 ? value : value + 1;
}