#include <Arduino.h>
#include "console.h"
#include "config.h"

#define BOUNCING_TIME 200

#define ANALOG_VALUES 1024
#define RESULT(value) ((value) / divisor + 1)

static int _compute_divisor(double value);

static const unsigned analog_range = (ANALOG_VALUES / (DIFFICULTY_LEVELS * 2));
static const unsigned divisor = _compute_divisor((double) ANALOG_VALUES / DIFFICULTY_LEVELS);

// extern volatile void (*match_result)(void);

volatile byte pattern[NPINS] = {PIN_1, PIN_2, PIN_3, PIN_4};
static byte index = 0;
static unsigned long last_button_pressed_time[NPINS] = {0};
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
  // const unsigned long current_time = millis();
  // if (current_time - last_button_pressed_time[i] > BOUNCING_TIME) {
  //   last_button_pressed_time[i] = current_time;
  //   if (i != pattern[NPINS - index - 1]) {
  //     match_result = &defeat;
  //     disable_all_pattern_interrupts();
  //     console_log(String("Interrupt ") + i + " detected DEFEAT");
  //     return;
  //   } else if (index == NPINS - 1) {
  //     match_result = &victory;
  //     disable_all_pattern_interrupts();
  //     console_log(String("Interrupt ") + i + " detected VICTORY");
  //     return;
  //   }
  //   digitalWrite(leds_pins[i], HIGH);
  //   index++;
  // }
  // console_log(String("Button") + (i + 1) + String(" pressed"));
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
  console_log(F("Wakeup interrupt called successfully"));
  disable_all_wakeup_interrupts();
}

void clear_pattern_handlers_state(void) {
  index = 0;
}

static int _compute_divisor(double value) {
  return value - (int) value == 0 ? value : value + 1;
}