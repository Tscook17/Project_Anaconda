#ifndef MY_CONFIG_H
#define MY_CONFIG_H

// general specs
#define MYCONFIG_GAME_TIMER_PERIOD 45.0E-3

// title screen
#define MYCONFIG_TITLE_TEXT_SIZE 4
#define MYCONFIG_SUBTITLE_TEXT_SIZE 2
#define MYCONFIG_TITLE_TEXT_YLOCATION 80
#define MYCONFIG_SUBTITLE_TEXT_YLOCATION 40
#define MYCONFIG_TITLE_TEXT_XLOCATION 0

// start screen
#define MYCONFIG_STARTSCREEN_DELAY 10
#define MYCONFIG_STARTSCREEN_TEXT_SIZE 2
#define MYCONFIG_STARTSCREEN_TEXT_YLOCATION 80
#define MYCONFIG_STARTSCREEN_TEXT_XLOCATION 0

// map specs
#define MYCONFIG_UPPER_BOARDER 30
#define MYCONFIG_LEFT_BOARDER 10
#define MYCONFIG_RIGHT_BOARDER 310
#define MYCONFIG_LOWER_BOARDER 230
#define MYCONFIG_BOARDER_COLOR DISPLAY_GREEN
#define MYCONFIG_BACKGROUND_COLOR DISPLAY_BLACK
#define MYCONFIG_TILE_SIZE 5
#define MYCONFIG_TILE_WIDTH                                                    \
  (int)((MYCONFIG_RIGHT_BOARDER - MYCONFIG_LEFT_BOARDER) / MYCONFIG_TILE_SIZE)
#define MYCONFIG_TILE_HEIGHT                                                   \
  (int)((MYCONFIG_LOWER_BOARDER - MYCONFIG_UPPER_BOARDER) / MYCONFIG_TILE_SIZE)

// score board
#define MYCONFIG_TEXT_COLOR DISPLAY_WHITE
#define MYCONFIG_SCORE_TEXT_SIZE 1

// snake
#define MYCONFIG_SNAKE_MAX_LENGTH (MYCONFIG_TILE_HEIGHT * MYCONFIG_TILE_WIDTH)
#define MYCONFIG_STARTING_SNAKE_LENGTH 5
#define MYCONFIG_SNAKE_HEAD_START_LOCATION_ROW // add value here
// apples

// blockades

#endif /* CONFIG_H */