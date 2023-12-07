#ifndef SNAKE_H
#define SNAKE_H

#include "myConfig.h"
#include "snake.h"
#include "snakeMap.h"
#include "gameControl.h"

// snake body segment struct
typedef struct node {
  mapSpaceLocation_t tileLocation;
  int8_t next;
  int8_t previous;
  bool empty;
} node_t;

// snake struct
typedef struct {
  uint8_t head;
  uint8_t tail;
  uint16_t snakeLength;
  node_t body[MYCONFIG_SNAKE_MAX_LENGTH];
  bool isDead;
} snake_t;

// intializes/reinits the snake sm
void snake_init(snakemap_t *currentMap, button_indicator_t *button);

// ticks snake sm
void snake_tick();

#endif /* SNAKE_H */