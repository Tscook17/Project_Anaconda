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
//static bool checkButton();//will be done later
//static void computeNextMove();
// global variables
static snake_t snake;
static snake_st_t currentState;
static snakemap_t *currentMap;
static snake_direction snakeDirection;
static uint8_t moveTest;

//call before snake tick
void snake_init(snakemap_t *map) {
  snake.head = NULL;
  snake.tail = NULL;
  snake.snakeLength = 0;
  currentMap = map;
  snakeDirection = right;
  currentState = init_st;
  for (uint8_t i = 0; i < MYCONFIG_STARTING_SNAKE_LENGTH; i++) {
    add_head_snake(set_snake_location(1, (i + 1)));
  }
}

// probably tick fast enough that you won't miss interrupts?
void snake_tick() {

  // action sm
  switch (currentState) {
  case init_st:
    break;
  case moving_st:
    // here is where the movement happens
    // or rather, call move snake
    move_snake(set_snake_location(1,moveTest), false);
    moveTest++;
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
  for (uint16_t i = 0; i < snake.snakeLength; i++) {
    remove_tail_snake();
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
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->tileLocation = move;
  new_node->next = NULL;
  new_node->previous = snake.head;
  snake.head = new_node;
  // if snake empty update tail
  if (snake.snakeLength == 0) {
    snake.tail = snake.head;
  }
  snake.snakeLength++;
  snakesToSquares(move, MYCONFIG_DRAW);
}

static void remove_tail_snake() {
  /* if there is only one item in the list, remove it */
  if (snake.tail->next == NULL) {
    free(snake.tail);
    return;
  }

  /* now current points to the second to last item of the list, so let's remove
   * current->next */
  mapSpaceLocation_t spot = snake.tail->tileLocation;
  node_t *newSnakeTail = snake.tail->next;
  free(snake.tail);
  snake.tail = newSnakeTail;
  newSnakeTail->previous = NULL;
  snake.snakeLength--;
  snakesToSquares(spot, MYCONFIG_ERASE);
}

static void snakesToSquares(mapSpaceLocation_t location, bool erase) {
  display_point_t snakeSquare = snakeMap_getLocationFromTile(location);
  display_fillRect(snakeSquare.x, snakeSquare.y, MYCONFIG_TILE_SIZE,
                   MYCONFIG_TILE_SIZE,
                   (erase ? MYCONFIG_BACKGROUND_COLOR : MYCONFIG_SNAKE_COLOR));
}