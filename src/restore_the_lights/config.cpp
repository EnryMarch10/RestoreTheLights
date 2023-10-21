#include <Arduino.h>
#include <EnableInterrupt.h>
#include "config.h"
#include "input.h"
#include "console.h"

#define LED_PIN_1 8
#define LED_PIN_2 9
#define LED_PIN_3 10
#define LED_PIN_4 11

#define BTN_PIN_1 4
#define BTN_PIN_2 5
#define BTN_PIN_3 6
#define BTN_PIN_4 7

#define BAUD_RATE 115200

GameState game_state = GAME_INTRO;

extern void btn1_handler(void);
extern void btn2_handler(void);
extern void btn3_handler(void);
extern void btn4_handler(void);
extern void wakeup_handler(void);
extern void clear_pattern_handlers_state(void);

static void _init_led(const byte pin);
static void _enable_interrupt(const byte i, const void (*handler)(void));
static void _disable_interrupt(const byte i);
static void _enable_interrupt(const byte i);
static void _enable_all_interrupts(const void (*handler)(void));
static void _disable_all_interrupts(void);

static const byte leds_pins[NPINS] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4};
static const byte buttons_pins[NPINS] = {BTN_PIN_1, BTN_PIN_2, BTN_PIN_3, BTN_PIN_4};

static const void (*buttons_handlers[NPINS])(void) = {btn1_handler, btn2_handler, btn3_handler, btn4_handler};
static bool inpterrupt_enabled[NPINS] = {false, false, false, false};

void setup_config(void) {
  Serial.begin(BAUD_RATE);
  for (byte i = 0; i < NPINS; i++) {
    _init_led(leds_pins[i]);
    pinMode(buttons_pins[i], INPUT);
  }
  _init_led(LED_PIN_CONTROL);
}

void turn_leds_off(void) {
  for (byte i = 0; i < NPINS; i++) {
    digitalWrite(leds_pins[i], LOW);
  }
  console_log(F("Leds turned off"));
}

void turn_leds_on(void) {
  for (byte i = 0; i < NPINS; i++) {
    digitalWrite(leds_pins[i], HIGH);
  }
  console_log(F("Leds turned on"));
}

void turn_led_on(byte i) {
  digitalWrite(leds_pins[i], HIGH);
}

void turn_led_off(byte i) {
  digitalWrite(leds_pins[i], LOW);
}

bool is_button_pressed(byte i) {
  return digitalRead(buttons_pins[i]) == HIGH;
}

void enable_all_wakeup_interrupts(void) {
  _enable_all_interrupts(&wakeup_handler);
}

void disable_all_wakeup_interrupts(void) {
  _disable_all_interrupts();
}

void enable_all_pattern_interrupts(void) {
  for (byte i = 0; i < NPINS; i++) {
    _enable_interrupt(i);
  }
}

void disable_all_pattern_interrupts(void) {
  _disable_all_interrupts();
  clear_pattern_handlers_state();
}

/* UTILITY functions */

static void _init_led(const byte pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

static void _enable_interrupt(const byte i, const void (*handler)(void)) {
  if (!inpterrupt_enabled[i]) {
    enableInterrupt(buttons_pins[i], handler, RISING);
    inpterrupt_enabled[i] = true;
  }
}

static void _disable_interrupt(const byte i) {
  if (inpterrupt_enabled[i]) {
    disableInterrupt(buttons_pins[i]);
    inpterrupt_enabled[i] = false;
  }
}

static void _enable_interrupt(const byte i) {
  _enable_interrupt(i, buttons_handlers[i]);
}

static void _enable_all_interrupts(const void (*handler)(void)) {
  for (byte i = 0; i < NPINS; i++) {
    _enable_interrupt(i, handler);
  }
}

static void _disable_all_interrupts(void) {
  for (byte i = 0; i < NPINS; i++) {
    _disable_interrupt(i);
  }
}
