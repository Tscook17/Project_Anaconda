#ifndef SNAKEMAP_H
#define SNAKEMAP_H

#include "myConfig.h"
#include <stdbool.h>
#include <stdint.h>

// type for whats on that board spot
typedef enum {
  MAPSPACE_EMPTY,
  MAPSPACE_OBSTACLE,
  MAPSPACE_APPLE,
  MAPSPACE_CONTAINS_SNAKE
} mapSpact_t;

typedef struct {
  int8_t row;
  int8_t col;
} mapSpaceLocation_t;

// passable board object
typedef struct {
  mapSpact_t snakeMap[MYCONFIG_TILE_HEIGHT][MYCONFIG_TILE_WIDTH];

  bool haveApple;
  bool haveObstacle;
} snakemap_t;

// clears map to all empty
void snakemap_clear(snakemap_t *currentMap) {
  for (int16_t i = 0; i < MYCONFIG_TILE_HEIGHT; i++) {
    for (int16_t j = 0; j < MYCONFIG_TILE_WIDTH; i++) {
      currentMap->snakeMap[i][j] = MAPSPACE_EMPTY;
    }
  }
  currentMap->haveApple = false;
  currentMap->haveObstacle = false;
}

#endif /* SNAKEMAP_H */