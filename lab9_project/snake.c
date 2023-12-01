#include "snake.h"
#include "snakeMap.h"
#include <stdio.h>

// state machine states for the snake
typedef enum { init_st, moving_st, go_right, go_left, dead_st } snake_st_t;
// this is a global var that determines what direction the snake should move
typedef enum { up, down, left, right } snake_direction;
// helper functions
static void move_snake(mapSpaceLocation_t move);
static mapSpaceLocation_t set_snake_location(uint8_t rowLocation,
                                             uint8_t colLocation);

// global variables
static snake_t snake;
static snake_st_t currentState;
static snakemap_t *currentMap;
static snake_direction snakeDirection;
static mapSpaceLocation_t newLocation;

void snake_init(snakemap_t *map) { currentMap = map; }
// probably tick fast enough that you won't miss interrupts?
void snake_tick() {

  // action sm
  switch (currentState) {
  case init_st:
    // initialize snake body, snake direction,

    break;
  case moving_st:
    // here is where the movement happens
    // or rather, call move snake
    if (snakeDirection == up) {
      move_snake(set_snake_location(snake.head->row - 1, snake.head->col));
    } else
      break;
  case go_right:
    break;
  case go_left:
    break;
  case dead_st:
    break;
  default:
    printf("Error default state in action reached!");
    break;
  }

  // transition sm
  switch (currentState) {
  case init_st:
    // transition to moving
    currentState = moving_st;
    break;
  case moving_st:
    break;
  case go_right:
    break;
  case go_left:
    break;
  case dead_st:
    break;
  default:
    printf("Error default state in transition reached!");
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

static mapSpaceLocation_t set_snake_location(uint8_t rowLocation,
                                             uint8_t colLocation) {
  newLocation.row = rowLocation;
  newLocation.col = colLocation;
  return newLocation;
}