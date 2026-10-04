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

enum state {
  STATE_RED = 0,
  STATE_YELLOW,
  STATE_GREEN,
};

enum event {
  NO_EVENT = 0,
  EVENT_STOP,
  EVENT_TIMEOUT,
  EVENT_GO,
};

static enum state state;
static enum event evt;

/*
 * Vantagens:
 * - Implementação direta a partir do diagrama
 * Desvantagem:
 * - Em cada estado deve saber qual o estado anterior.
 * Se mudar um estado anterior, deve mudar manualmente
 * no estado atual.
 */
int main(int argc, char *argv[]) {
  while (1) {
    if (evt == NO_EVENT) {
      sleep_ms(200);
      continue;
    }

    switch (state) {
    case STATE_RED:
      if (evt == EVENT_GO) { /* Verifica os eventos válidos para esse estado. */
        turn_off(LED_RED_PIN);  /* Trata o evento */
        turn_on(LED_GREEN_PIN); /* Prepara o novo estado */
        state = STATE_GREEN;    /* Troca para o novo estado */
      }
      break;
    case STATE_YELLOW:
      if (evt == EVENT_TIMEOUT) {
        turn_off(LED_YELLOW_PIN);
        turn_on(LED_RED_PIN);
        state = STATE_RED;
      }
      break;
    case STATE_GREEN:
      if (evt == EVENT_STOP) {
        turn_off(LED_GREEN_PIN);
        turn_on(LED_YELLOW_PIN);
        start_timer(60000);
        state = STATE_YELLOW;
      }
      break;
    default:
      error();
      break;
    }

    evt = NO_EVENT;
  }

  return 0;
}
