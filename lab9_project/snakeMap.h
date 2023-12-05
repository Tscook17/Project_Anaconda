#ifndef SNAKEMAP_H
#define SNAKEMAP_H

#include "myConfig.h"
#include "display.h"
#include <stdbool.h>
#include <stdint.h>

// type for whats on that board spot
typedef enum {
  MAPSPACE_EMPTY,
  MAPSPACE_OBSTACLE,
  MAPSPACE_APPLE,
  MAPSPACE_CONTAINS_SNAKE
} mapSpace_t;

// keeps track of tile location
typedef struct {
  uint8_t row;
  uint8_t col;
} mapSpaceLocation_t;

// passable board object
typedef struct {
  mapSpace_t snakeMap[MYCONFIG_TILE_HEIGHT][MYCONFIG_TILE_WIDTH];

  bool haveApple;
  bool haveObstacle;
} snakemap_t;

void snakemap_clear(snakemap_t *currentMap);
display_point_t snakeMap_getLocationFromTile(mapSpaceLocation_t tile);

mapSpaceLocation_t snakeMap_getTileFromLocation(display_point_t spot);

#endif /* SNAKEMAP_H */