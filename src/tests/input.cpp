#include <Arduino.h>
#include "config.h"

#define BOUNCING_TIME 200

byte pattern[NPINS] = {PIN_1, PIN_2, PIN_3, PIN_4};
static byte current[NPINS];
static unsigned long lastButtonPressedTime[NPINS] = {0};

static void _button_handler(byte i) {
  const unsigned long current_time = millis();
  if (current_time - lastButtonPressedTime[i] > BOUNCING_TIME) {
    lastButtonPressedTime[i] = current_time;
    Serial.println(String("Button") + (i + 1) + String(" pressed"));
  }
  disable_all_interrupts();
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