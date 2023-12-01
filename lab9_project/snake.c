#include "snake.h"
#include "snakeMap.h"
#include "display.h"
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
      //move_snake(set_snake_location(snake.head->row - 1, snake.head->col));
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

void snake_dead() {
  for(uint16_t i = 0; i < snake.snakeLength; i++){
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
  snake.snakeLength++;
  
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
  // erase spot TODO
}