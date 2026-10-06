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

  _EVENTS_SIZE,
};

struct state_obj {
  void (*enter)(struct state_obj *);
  void (*exit)(struct state_obj *);
};

void red_enter(struct state_obj *obj) { turn_on(LED_RED_PIN); }
void red_exit(struct state_obj *obj) { turn_off(LED_RED_PIN); }
static struct state_obj red_state = {
    .enter = red_enter,
    .exit = red_exit,
};

void yellow_enter(struct state_obj *obj) {
  turn_on(LED_YELLOW_PIN);
  start_timer(60);
}
void yellow_exit(struct state_obj *obj) { turn_off(LED_YELLOW_PIN); }
static struct state_obj yellow_state = {
    .enter = yellow_enter,
    .exit = yellow_exit,
};

void green_enter(struct state_obj *obj) { turn_on(LED_GREEN_PIN); }
void green_exit(struct state_obj *obj) { turn_off(LED_GREEN_PIN); }
static struct state_obj green_state = {
    .enter = green_enter,
    .exit = green_exit,
};

static enum state state = STATE_GREEN;
static enum event evt = NO_EVENT;
static struct state_obj *state_list[] = {
    [STATE_RED] = &red_state,
    [STATE_YELLOW] = &yellow_state,
    [STATE_GREEN] = &green_state,
};
static enum state next_state_table[][_EVENTS_SIZE] = {
    [STATE_RED] =
        {
            [NO_EVENT] = STATE_RED,
            [EVENT_GO] = STATE_GREEN,
            [EVENT_STOP] = STATE_RED,
            [EVENT_TIMEOUT] = STATE_RED,
        },
    [STATE_YELLOW] =
        {
            [NO_EVENT] = STATE_YELLOW,
            [EVENT_GO] = STATE_YELLOW,
            [EVENT_STOP] = STATE_YELLOW,
            [EVENT_TIMEOUT] = STATE_RED,
        },
    [STATE_GREEN] =
        {
            [NO_EVENT] = STATE_GREEN,
            [EVENT_GO] = STATE_GREEN,
            [EVENT_STOP] = STATE_YELLOW,
            [EVENT_TIMEOUT] = STATE_GREEN,
        },
};

int main(int argc, char *argv[]) {
  while (1) {
    struct state_obj *obj = state_list[state];
    enum state new_state = next_state_table[state][evt];
    if (new_state == state) {
      sleep_ms(200);
      continue;
    }

    struct state_obj *new_obj = state_list[new_state];

    obj->exit(obj);
    new_obj->enter(new_obj);

    state = new_state;
    evt = NO_EVENT;
  }

  return 0;
}

/*
 * Chamada 06/10/2026
 *
 * 1. Maria Antonia
 * 2. Matheus Melo
 * 3. Thiago Lobo Pereira Barros
 * 4. Kaio Vitor Nabuco
 * 5. Kauã Lessa Lima dos Santos
 * 6. Guilherme de Oliveira Costa
 * 7. Guilherme Oliveira Silva Gomes
 * 8. Jackson Bruno Lima Leão
 */
