#include "snakeMap.h"

// clears map to all empty
void snakemap_clear(snakemap_t *currentMap) {
  for (uint16_t i = 0; i < MYCONFIG_TILE_WIDTH; i++) {
    for (uint16_t j = 0; j < MYCONFIG_TILE_HEIGHT; j++) {
      currentMap->snakeMap[i][j] = MAPSPACE_EMPTY;
    }
  }
  currentMap->haveApple = false;
  currentMap->haveObstacle = false;
  currentMap->snakeDead = false;
}

// converts snake row and column to display pixels
display_point_t snakeMap_getLocationFromTile(mapSpaceLocation_t tile) {
  display_point_t location;
  uint8_t tx = tile.col;
  uint8_t ty = tile.row;

  location.x = tx * MYCONFIG_TILE_SIZE + MYCONFIG_LEFT_BOARDER;
  location.y = ty * MYCONFIG_TILE_SIZE + MYCONFIG_UPPER_BOARDER;

  return location;
}

// converts display point to snake tile struct
mapSpaceLocation_t snakeMap_getTileFromLocation(display_point_t spot) {
  mapSpaceLocation_t location;
  uint8_t tx = spot.x;
  uint8_t ty = spot.y;

  location.col = (tx - MYCONFIG_LEFT_BOARDER) / MYCONFIG_TILE_SIZE;
  location.row = (ty - MYCONFIG_UPPER_BOARDER) / MYCONFIG_TILE_SIZE;
  return location;
}

// pass row and column, return tile location struct
mapSpaceLocation_t set_snake_location(uint8_t colLocation,
                                             uint8_t rowLocation) {
  mapSpaceLocation_t newLocation;
  newLocation.row = rowLocation;
  newLocation.col = colLocation;
  return newLocation;
}