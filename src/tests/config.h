#ifndef ___CONFIG___
#define ___CONFIG___

#define LED_PIN_CONTROL 13

#define PIN_1 0
#define PIN_2 1
#define PIN_3 2
#define PIN_4 3

#define POT_PIN A0

#define NPINS 4

void setup_config(void);

void enable_all_interrupts(void);
void disable_all_interrupts(void);

#endif
