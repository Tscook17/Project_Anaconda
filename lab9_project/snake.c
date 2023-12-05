#include "snake.h"
#include "display.h"
#include "snakeMap.h"
#include <stdio.h>

// state machine states for the snake
typedef enum { init_st, moving_st, go_right, go_left, dead_st } snake_st_t;
// this is a global var that determines what direction the snake should move
typedef enum { up, down, left, right } snake_direction;
// helper functions
static void move_snake(mapSpaceLocation_t move, bool grow);
static mapSpaceLocation_t set_snake_location(uint8_t rowLocation,
                                             uint8_t colLocation);
static void add_head_snake(mapSpaceLocation_t move);
static void remove_tail_snake();
static void snakesToSquares(mapSpaceLocation_t location, bool erase);
static uint8_t findOpening();

// static bool checkButton();//will be done later
// static void computeNextMove();
//  global variables
static snake_t snake;
static snake_st_t currentState;
static snakemap_t *currentMap;
static snake_direction snakeDirection;
static uint8_t speedControl;

static uint8_t moveTest;

// call before snake tick
void snake_init(snakemap_t *map) {
  snake.head = -1;
  snake.tail = -1;
  snake.snakeLength = 0;
  currentMap = map;
  snakeDirection = right;
  currentState = init_st;
}

// probably tick fast enough that you won't miss interrupts?
void snake_tick() {
  // printf("%d\n", currentState);
  //  action sm
  switch (currentState) {
  case init_st:
    snake_dead();
    for (uint8_t i = 0; i < MYCONFIG_STARTING_SNAKE_LENGTH; i++) {
      add_head_snake(set_snake_location(1, (i + 1)));
    }
    break;
  case moving_st:
    // here is where the movement happens
    // or rather, call move snake

    if (!(speedControl % MYCONFIG_SNAKE_SPEED)) { // determines snake speed
    move_snake(set_snake_location(1, moveTest), false);
    moveTest++;
    }
    speedControl++;
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

void snake_dead() {
  node_t temp;
  temp.empty = true;
  temp.next = -1;
  temp.previous = -1;

  for (uint16_t i = 0; i < MYCONFIG_SNAKE_MAX_LENGTH; i++) {
    snake.body[i] = temp;
  }
}

static void move_snake(mapSpaceLocation_t move, bool grow) {
  add_head_snake(move);
  if (!grow) {
    remove_tail_snake();
  }
}

static mapSpaceLocation_t set_snake_location(uint8_t rowLocation,
                                             uint8_t colLocation) {
  mapSpaceLocation_t newLocation;
  newLocation.row = rowLocation;
  newLocation.col = colLocation;
  return newLocation;
}

static void add_head_snake(mapSpaceLocation_t move) {
  node_t new_node;
  uint8_t index = findOpening();
  snake.body[index] = new_node;
  snake.body[index].tileLocation = move;
  snake.body[index].next = -1;
  snake.body[index].previous = snake.head;
  snake.body[snake.head].next = index;
  snake.head = index;
  // if snake empty update tail
  if (snake.snakeLength == 0) {
    snake.tail = snake.head;
  }
  snake.snakeLength++;
  snakesToSquares(move, MYCONFIG_DRAW);
}

static void remove_tail_snake() {
  mapSpaceLocation_t spot = snake.body[snake.tail].tileLocation;
  int8_t newTailIndex = snake.body[snake.tail].next;
  snake.body[snake.tail].empty = true;
  snake.tail = newTailIndex;
  //printf("tail:%d\n", newTailIndex);
  snake.body[newTailIndex].previous = -1;
  snake.snakeLength--;
  snakesToSquares(spot, MYCONFIG_ERASE);
}

static void snakesToSquares(mapSpaceLocation_t location, bool erase) {
  display_point_t snakeSquare = snakeMap_getLocationFromTile(location);
  display_fillRect(snakeSquare.x, snakeSquare.y, MYCONFIG_TILE_SIZE,
                   MYCONFIG_TILE_SIZE,
                   (erase ? MYCONFIG_BACKGROUND_COLOR : MYCONFIG_SNAKE_COLOR));
}

static uint8_t findOpening() {
  for (uint8_t i = 0; i < MYCONFIG_SNAKE_MAX_LENGTH; i++) {
    if (snake.body[i].empty) {
      return i;
    }
  }
}