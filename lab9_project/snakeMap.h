#ifndef SNAKEMAP_H
#define SNAKEMAP_H

#include "display.h"
#include "myConfig.h"
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
  uint8_t col;
  uint8_t row;
} mapSpaceLocation_t;

// passable board object
typedef struct {
  mapSpace_t snakeMap[MYCONFIG_TILE_WIDTH][MYCONFIG_TILE_HEIGHT];

  bool haveApple;
  bool haveObstacle;
} snakemap_t;

void snakemap_clear(snakemap_t *currentMap);

display_point_t snakeMap_getLocationFromTile(mapSpaceLocation_t tile);

mapSpaceLocation_t snakeMap_getTileFromLocation(display_point_t spot);

mapSpaceLocation_t set_snake_location(uint8_t colLocation,
                                             uint8_t rowLocation);

#endif /* SNAKEMAP_H */