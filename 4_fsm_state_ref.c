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

struct state_obj {
  void (*enter)(struct state_obj *);
  void (*exit)(struct state_obj *);
  enum state (*next_state)(struct state_obj *, enum event evt);
};

void red_enter(struct state_obj *obj) { turn_on(STATE_RED); }
void red_exit(struct state_obj *obj) { turn_off(STATE_RED); }
enum state red_next_state(struct state_obj *obj, enum event evt) {
  if (evt == EVENT_GO) {
    return STATE_GREEN;
  } else {
    return STATE_RED;
  }
}
static struct state_obj red_state = {
    .enter = red_enter,
    .exit = red_exit,
    .next_state = red_next_state,
};

void yellow_enter(struct state_obj *obj) {
  turn_on(STATE_YELLOW);
  start_timer(60000);
}
void yellow_exit(struct state_obj *obj) { turn_off(STATE_YELLOW); }
enum state yellow_next_state(struct state_obj *obj, enum event evt) {
  if (evt == EVENT_TIMEOUT) {
    return STATE_RED;
  } else {
    return STATE_YELLOW;
  }
}
static struct state_obj yellow_state = {
    .enter = yellow_enter,
    .exit = yellow_exit,
    .next_state = yellow_next_state,

};

void green_enter(struct state_obj *obj) { turn_on(STATE_GREEN); }
void green_exit(struct state_obj *obj) { turn_off(STATE_GREEN); }
enum state green_next_state(struct state_obj *obj, enum event evt) {
  if (evt == EVENT_STOP) {
    return STATE_YELLOW;
  } else {
    return STATE_GREEN;
  }
}
static struct state_obj green_state = {
    .enter = green_enter,
    .exit = green_exit,
    .next_state = green_next_state,
};

static enum state state;
static enum event evt;
static struct state_obj *state_list[] = {
    [STATE_RED] = &red_state,
    [STATE_YELLOW] = &yellow_state,
    [STATE_GREEN] = &green_state,
};

/*
 * Vantagens:
 * - O loop principal não é alterado na adição e remoção de estados.
 * - Cada estado fica isolado.
 * - Alterações ou adições só mexem na sua parte.
 *
 * Desvantagem:
 * - Mais código
 * - Mais dificil de seguir o fluxo principal
 * - Remoção de estados envolve mexer em pontos separados
 */
int main(int argc, char *argv[]) {
  struct state_obj *obj;
  struct state_obj *new_obj;
  enum state new_state;

  while (1) {
    obj = state_list[state];
    new_state = obj->next_state(obj, evt);
    if (new_state == state) {
      sleep_ms(200);
      continue;
    }

    new_obj = state_list[new_state];
    obj->exit(obj);
    new_obj->enter(obj);

    state = new_state;
    evt = NO_EVENT;
  }

  return 0;
}
