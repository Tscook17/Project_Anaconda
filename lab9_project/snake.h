#ifndef SNAKE_H
#define SNAKE_H

#include "myConfig.h"
#include "snakeMap.h"
#include "snake.h"

typedef struct node {
    mapSpaceLocation_t tileLocation;
    struct node * next;
    struct node * previous;
} node_t;


typedef struct {
    node_t *head;
    node_t *tail;
    uint16_t snakeLength;
} snake_t;

void snake_init(snakemap_t *currentMap);

void snake_tick();

void snake_dead();

#endif /* SNAKE_H */