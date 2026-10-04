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

/* Aqui é tratado a transição dos estados. */
static enum state next_state(enum state state) {
  switch (state) {
  case STATE_RED:
    return STATE_GREEN;
  case STATE_YELLOW:
    return STATE_RED;
  case STATE_GREEN:
    return STATE_YELLOW;
  }
}

/*
 * Vantagens:
 * - Uma função para mudar os estados (Totalmente isolada)
 * - Focada na troca (em alguns casos a lógica fica mais simples)
 * - 2 switches: 1 para evento e 1 transição
 *
 * Desvantagem:
 * - Focada na troca (em alguns casos a lógica fica mais complicada)
 * - Mais código
 */
int main(int argc, char *argv[]) {
  while (1) {
    /* Aqui é tratado a atuação do estado. */
    switch (evt) {
    case NO_EVENT:
      sleep_ms(200);
      continue;
    case EVENT_STOP:
      if (state == STATE_GREEN) {
        turn_off(LED_GREEN_PIN);
      }
      break;
    case EVENT_TIMEOUT:
      if (state == STATE_YELLOW) {
        turn_off(LED_YELLOW_PIN);
      }
      break;
    case EVENT_GO:
      if (state == STATE_RED) {
        turn_off(LED_RED_PIN);
      }
      break;
    default:
      error();
      break;
    }

    evt = NO_EVENT;
    state = next_state(state);

    /* Aqui é tratado a atuação do estado. */
    switch (state) {
    case STATE_RED:
      if (!pin_state(LED_RED_PIN)) {
        turn_on(LED_RED_PIN);
      }
      break;
    case STATE_YELLOW:
      if (!pin_state(LED_YELLOW_PIN)) {
        turn_on(LED_YELLOW_PIN);
      }

      start_timer(60000);
      break;
    case STATE_GREEN:
      if (!pin_state(LED_GREEN_PIN)) {
        turn_on(LED_GREEN_PIN);
      }
      break;
    default:
      error();
      break;
    }
  }

  return 0;
}
