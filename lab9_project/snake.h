#ifndef SNAKE_H
#define SNAKE_H

#include "myConfig.h"
#include "snakeMap.h"
#include "snake.h"

typedef struct {
    mapSpaceLocation_t body[MYCONFIG_SNAKE_MAX_LENGTH];
    mapSpaceLocation_t *head;
    mapSpaceLocation_t *tail;
    int16_t snakeLength;
} snake_t;

void snake_init(snakemap_t *currentMap);

void snake_tick();

void snake_dead();

#endif /* SNAKE_H */