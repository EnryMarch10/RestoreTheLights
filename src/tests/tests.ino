#include "config.h"

#define RESULT(value) ((value) / 256.0 + 1)

// unsigned long time;
static byte difficulty = 1;

static const int offset = 300;
static int last_value = -offset;
static int last_result = -1;

byte read_difficulty_level(void) {
  const int value = analogRead(POT_PIN);
  if (value <= last_value - offset || value >= last_value + offset) {
    const byte result = RESULT(value);
    if (last_result != result) {
      last_value = value;
      last_result = result;
      return result;
    }
  }
  return last_result = RESULT(last_value);
}

void setup() {
  setup_config();
  difficulty = read_difficulty_level();
  Serial.println(String("DIFFICULTY = ") + difficulty);
}

void loop() {
  const byte curr_diff = read_difficulty_level();
  if (curr_diff != difficulty) {
    difficulty = curr_diff;
    Serial.println(String("DIFFICULTY = ") + difficulty);
  }
  // static unsigned long start_time = millis();
  // time = millis();
  // if (time - start_time > 10000) {
  //   Serial.println(String("Reenabling interrupts after: ") + (time - start_time) / 1000.0 + " s");
  //   enable_all_interrupts();
  //   start_time = time;
  // }
  // put your main code here, to run repeatedly:
}
