#ifndef SNAKE_H
#define SNAKE_H

#include "myConfig.h"
#include "snakeMap.h"
#include "snake.h"

typedef struct node {
    mapSpaceLocation_t tileLocation;
    int8_t next;
    int8_t previous;
    bool empty;
} node_t;


typedef struct {
    uint8_t head;
    uint8_t tail;
    uint16_t snakeLength;
    node_t body[MYCONFIG_SNAKE_MAX_LENGTH];
} snake_t;

void snake_init(snakemap_t *currentMap);

void snake_tick();

void snake_dead();

#endif /* SNAKE_H */