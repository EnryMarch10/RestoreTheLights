#include <Arduino.h>
#include "console.h"
#include "config.h"

void console_log(const String& message) {
#ifdef ___DEBUG___
  Serial.println(message);
  Serial.flush();
#endif
}
