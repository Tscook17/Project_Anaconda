#include "snake.h"
#include "snakeMap.h"

typedef enum { init_st, start_st, moving, dead_st } snake_st_t;

// helper functions
static void move_snake(mapSpaceLocation_t move);

// global variables
static snake_t snake;
static snake_st_t currentState;
static snakemap_t *currentMap;

void snake_init(snakemap_t *map) { currentMap = map; }

void snake_tick() {

  // action sm
  switch (currentState) {
  case init_st:
    break;
  case start_st:
    break;
  case moving:
    break;
  case dead_st:
    break;
  default:
    break;
  }

  // transition sm
  switch (currentState) {
  case init_st:
    break;
  case start_st:
    break;
  case moving:
    break;
  case dead_st:
    break;
  default:
    break;
  }
}

void snake_dead() {}

static void move_snake(mapSpaceLocation_t move) {
  for (int8_t i = snake.snakeLength; i > snake.snakeLength; i--) {
    snake.body[i + 1] = snake.body[i];
  }
  *snake.head = move;
}