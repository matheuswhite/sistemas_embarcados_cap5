#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define LED_RED_PIN 13
#define LED_YELLOW_PIN 14
#define LED_GREEN_PIN 22

extern void turn_on(uint32_t pin);
extern void turn_off(uint32_t pin);
extern void error(void);
extern void start_timer(uint32_t secs);
extern void sleep_ms(uint32_t ms);
extern bool pin_state(uint32_t pin);
