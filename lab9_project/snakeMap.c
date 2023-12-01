#include "snakeMap.h"


// clears map to all empty
void snakemap_clear(snakemap_t *currentMap) {
  for (uint16_t i = 0; i < MYCONFIG_TILE_HEIGHT; i++) {
    for (uint16_t j = 0; j < MYCONFIG_TILE_WIDTH; i++) {
      currentMap->snakeMap[i][j] = MAPSPACE_EMPTY;
    }
  }
  currentMap->haveApple = false;
  currentMap->haveObstacle = false;
}

// converts snake row and column to display pixels
display_point_t snakeMap_getLocationFromTile(mapSpaceLocation_t tile) {
  display_point_t location;
  uint8_t tx = tile.col;
  uint8_t ty = tile.row;

  location.x = tx * MYCONFIG_TILE_SIZE + MYCONFIG_LEFT_BOARDER;
  location.x = ty * MYCONFIG_TILE_SIZE + MYCONFIG_UPPER_BOARDER;

  return location;
}