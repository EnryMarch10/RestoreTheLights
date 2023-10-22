#include <Arduino.h>
#include <assert.h>
#include <EnableInterrupt.h>
#include "config.h"

#define LED_PIN_1 8
#define LED_PIN_2 9
#define LED_PIN_3 10
#define LED_PIN_4 11

#define BTN_PIN_1 4
#define BTN_PIN_2 5
#define BTN_PIN_3 6
#define BTN_PIN_4 7

#define BAUD_RATE 115200

extern void btn1_handler(void);
extern void btn2_handler(void);
extern void btn3_handler(void);
extern void btn4_handler(void);

static const void (*button_handlers[NPINS])(void) = {btn1_handler, btn2_handler, btn3_handler, btn4_handler};
const byte leds_pins[NPINS] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4};
const byte buttons_pins[NPINS] = {BTN_PIN_1, BTN_PIN_2, BTN_PIN_3, BTN_PIN_4};

static bool enabled[NPINS] = {false, false, false, false};

void enable_interrupt(byte i) {
  assert(i >= 0 && i < NPINS);
  noInterrupts();
  if (!enabled[i]) {
    enableInterrupt(buttons_pins[i], button_handlers[i], RISING);
    enabled[i] = true;
  }
  interrupts();
}

void disable_interrupt(byte i) {
  assert(i >= 0 && i < NPINS);
  noInterrupts();
  if (enabled[i]) {
    disableInterrupt(buttons_pins[i]);
    enabled[i] = false;
  }
  interrupts();
}

void enable_all_interrupts(void) {
  for (byte i = 0; i < NPINS; i++) {
    enable_interrupt(i);
  }
}

void disable_all_interrupts(void) {
  for (byte i = 0; i < NPINS; i++) {
    disable_interrupt(i);
  }
}

static void _init_led(byte pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void setup_config(void) {
  Serial.begin(BAUD_RATE);
  for (byte i = 0; i < NPINS; i++) {
    _init_led(leds_pins[i]);
    // pinMode(buttons_pins[i], INPUT);
    // enable_interrupt(i);
  }
  _init_led(LED_PIN_CONTROL);
}