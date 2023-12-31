#include "snake.h"
#include "buttons.h"
#include "display.h"
#include "snakeMap.h"
#include <stdio.h>

// state machine states for the snake
typedef enum { init_st, moving_st, dead_st } snake_st_t;

// this is a global var that determines what direction the snake should move
typedef enum { up, down, left, right } snake_direction_t;

// helper functions
static void snake_clear();
static void move_snake(mapSpaceLocation_t move, bool grow);
static void add_head_snake(mapSpaceLocation_t move);
static void remove_tail_snake();
static void snakesToSquares(mapSpaceLocation_t location, bool erase);
static uint8_t findOpening();
static mapSpaceLocation_t computeNextMove();

//  global variables
static snake_t snake;
static snake_st_t currentState;
static snakemap_t *currentMap;
static button_indicator_t *buttonInput;
static snake_direction_t snakeDirection;
static mapSpaceLocation_t nextMove;

// call before snake tick
void snake_init(snakemap_t *map, button_indicator_t *button) {
  snake.head = -1;
  snake.tail = -1;
  snake.snakeLength = 0;
  snake.isDead = false;
  currentMap = map;
  buttonInput = button;
  snakeDirection = right;
  currentState = init_st;
}

// probably tick fast enough that you won't miss interrupts?
void snake_tick() {
  //  action sm
  switch (currentState) {
  case init_st:
    snake_clear();
    for (uint8_t i = 0; i < MYCONFIG_STARTING_SNAKE_LENGTH; i++) {
      add_head_snake(
          set_snake_location((MYCONFIG_SNAKE_HEAD_START_LOCATION_X + i),
                             MYCONFIG_SNAKE_HEAD_START_LOCATION_Y));
    }
    break;
  case moving_st:
    nextMove = computeNextMove();
    if ((nextMove.col > MYCONFIG_TILE_WIDTH || nextMove.col < 0) ||
        (nextMove.row > MYCONFIG_TILE_HEIGHT || nextMove.row < 0)) {
      snake.isDead = true;
      break;
    } else if (currentMap->snakeMap[nextMove.col][nextMove.row] ==
               MAPSPACE_CONTAINS_SNAKE) {
      snake.isDead = true;
      break;
    }

    if (currentMap->snakeMap[nextMove.col][nextMove.row] == MAPSPACE_APPLE) {
      move_snake(nextMove, true);
    } else {
      if (currentMap->snakeMap[nextMove.col][nextMove.row] ==
          MAPSPACE_OBSTACLE) {
        move_snake(nextMove, false);
        remove_tail_snake();
        (currentMap->numObstacle)--;
      } else {
        move_snake(nextMove, false);
      }
    }
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
    if (snake.isDead == true) {
      currentState = dead_st;
      currentMap->snakeDead = true;
    } else if (snake.snakeLength == 0) {
      currentState = dead_st;
      currentMap->snakeDead = true;
    }
    break;
  case dead_st:
    break;
  default:
    printf("Error default state in transition reached!");
    break;
  }
}

// clear snake body
static void snake_clear() {
  node_t temp;
  temp.empty = true;
  temp.next = -1;
  temp.previous = -1;
  temp.tileLocation.col = 0;
  temp.tileLocation.row = 0;

  for (uint16_t i = 0; i < MYCONFIG_SNAKE_MAX_LENGTH; i++) {
    snake.body[i] = temp;
  }
}

// move snake to passed move, don't cut tail if growing
static void move_snake(mapSpaceLocation_t move, bool grow) {
  add_head_snake(move);
  if (!grow) {
    remove_tail_snake();
  } else {
    currentMap->haveApple = false;
  }
}

// add a new head to snake
static void add_head_snake(mapSpaceLocation_t move) {
  // add new head node
  node_t new_node;
  uint8_t index = findOpening();
  snake.body[index] = new_node;
  snake.body[index].tileLocation = move;
  snake.body[index].next = -1;
  snake.body[index].previous = snake.head;
  snake.body[snake.head].next = index;
  snake.head = index;
  snake.snakeLength++;

  // if snake empty update tail
  if (snake.snakeLength == 0) {
    snake.tail = snake.head;
  }

  // update map
  currentMap->snakeMap[move.col][move.row] = MAPSPACE_CONTAINS_SNAKE;

  // draw new head square
  snakesToSquares(move, MYCONFIG_DRAW);
}

// remove tail from snake
static void remove_tail_snake() {
  // remove snake tail node
  mapSpaceLocation_t spot = snake.body[snake.tail].tileLocation;
  int8_t newTailIndex = snake.body[snake.tail].next;
  snake.body[snake.tail].empty = true;
  snake.tail = newTailIndex;
  snake.body[newTailIndex].previous = -1;
  snake.snakeLength--;

  // update map
  currentMap->snakeMap[spot.col][spot.row] = MAPSPACE_EMPTY;

  // erase tail square
  snakesToSquares(spot, MYCONFIG_ERASE);
}

// draw/erase snake segment
static void snakesToSquares(mapSpaceLocation_t location, bool erase) {
  display_point_t snakeSquare = snakeMap_getLocationFromTile(location);
  display_fillRect(snakeSquare.x, snakeSquare.y, MYCONFIG_TILE_SIZE,
                   MYCONFIG_TILE_SIZE,
                   (erase ? MYCONFIG_BACKGROUND_COLOR : MYCONFIG_SNAKE_COLOR));
  if (!erase) {
    display_drawRect(snakeSquare.x, snakeSquare.y, MYCONFIG_TILE_SIZE,
                     MYCONFIG_TILE_SIZE, MYCONFIG_SNAKE_BORDER_COLOR);
  }
}

// find opening in snake body array
static uint8_t findOpening() {
  for (uint8_t i = 0; i < MYCONFIG_SNAKE_MAX_LENGTH; i++) {
    if (snake.body[i].empty) {
      return i;
    }
  }
}

// based on button indicator and current direction determine the next direction
// to go.
static mapSpaceLocation_t computeNextMove() {
  button_indicator_t nextDirection = *buttonInput;
  *buttonInput = no_button_press;
  if (snakeDirection == up) {
    if (nextDirection == left_button_press) {
      snakeDirection = left;
      return set_snake_location(snake.body[snake.head].tileLocation.col - 1,
                                snake.body[snake.head].tileLocation.row);
    } else if (nextDirection == right_button_press) {
      snakeDirection = right;
      return set_snake_location(snake.body[snake.head].tileLocation.col + 1,
                                snake.body[snake.head].tileLocation.row);
    }
    // nextDirection is none
    else {
      return set_snake_location(snake.body[snake.head].tileLocation.col,
                                snake.body[snake.head].tileLocation.row - 1);
    }
  } else if (snakeDirection == down) {
    if (nextDirection == left_button_press) {
      snakeDirection = right;
      return set_snake_location(snake.body[snake.head].tileLocation.col + 1,
                                snake.body[snake.head].tileLocation.row);
    } else if (nextDirection == right_button_press) {
      snakeDirection = left;
      return set_snake_location(snake.body[snake.head].tileLocation.col - 1,
                                snake.body[snake.head].tileLocation.row);
    }
    // nextDirection is none
    else {
      return set_snake_location(snake.body[snake.head].tileLocation.col,
                                snake.body[snake.head].tileLocation.row + 1);
    }
  } else if (snakeDirection == left) {
    if (nextDirection == left_button_press) {
      snakeDirection = down;
      return set_snake_location(snake.body[snake.head].tileLocation.col,
                                snake.body[snake.head].tileLocation.row + 1);
    } else if (nextDirection == right_button_press) {
      snakeDirection = up;
      return set_snake_location(snake.body[snake.head].tileLocation.col,
                                snake.body[snake.head].tileLocation.row - 1);
    }
    // nextDirection is none
    else {
      return set_snake_location(snake.body[snake.head].tileLocation.col - 1,
                                snake.body[snake.head].tileLocation.row);
    }
  } else if (snakeDirection == right) {
    if (nextDirection == left_button_press) {
      snakeDirection = up;
      return set_snake_location(snake.body[snake.head].tileLocation.col,
                                snake.body[snake.head].tileLocation.row - 1);
    } else if (nextDirection == right_button_press) {
      snakeDirection = down;
      return set_snake_location(snake.body[snake.head].tileLocation.col,
                                snake.body[snake.head].tileLocation.row + 1);
    }
    // nextDirection is none
    else {
      return set_snake_location(snake.body[snake.head].tileLocation.col + 1,
                                snake.body[snake.head].tileLocation.row);
    }
  }
}