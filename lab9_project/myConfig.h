#ifndef MY_CONFIG_H
#define MY_CONFIG_H

/*
Todo list:
- fix create snake bug
- right edge instant die/apple obstacle push
- fix obstacles
- add sounds maybe?
*/

// general specs
#define MYCONFIG_GAME_TIMER_PERIOD 45.0E-3
#define MYCONFIG_ERASE true
#define MYCONFIG_DRAW false

// title screen
#define MYCONFIG_TITLE_TEXT_SIZE 4
#define MYCONFIG_TITLE_TEXT_COLOR DISPLAY_GREEN
#define MYCONFIG_SUBTITLE_TEXT_SIZE 1
#define MYCONFIG_TITLE_TEXT_YLOCATION 80
#define MYCONFIG_SUBTITLE_TEXT_YLOCATION 120
#define MYCONFIG_TITLE_TEXT_XLOCATION 90

// start screen
#define MYCONFIG_STARTSCREEN_DELAY 4
#define MYCONFIG_STARTSCREEN_TEXT_SIZE 2
#define MYCONFIG_STARTSCREEN_TEXT_YLOCATION 60
#define MYCONFIG_STARTSCREEN_TEXT_XLOCATION 0

// map specs
#define MYCONFIG_UPPER_BOARDER 30
#define MYCONFIG_LEFT_BOARDER 10
#define MYCONFIG_RIGHT_BOARDER 310
#define MYCONFIG_LOWER_BOARDER 230
#define MYCONFIG_BOARDER_OFFSET 5
#define MYCONFIG_BOARDER_COLOR DISPLAY_GREEN
#define MYCONFIG_BACKGROUND_COLOR DISPLAY_BLACK
#define MYCONFIG_TILE_SIZE 10
#define MYCONFIG_TILE_WIDTH                                                    \
  (int)((MYCONFIG_RIGHT_BOARDER - MYCONFIG_LEFT_BOARDER) /                     \
            MYCONFIG_TILE_SIZE -                                               \
        1)
#define MYCONFIG_TILE_HEIGHT                                                   \
  (int)((MYCONFIG_LOWER_BOARDER - MYCONFIG_UPPER_BOARDER) /                    \
            MYCONFIG_TILE_SIZE -                                               \
        1)

// score board
#define MYCONFIG_TEXT_COLOR DISPLAY_WHITE
#define MYCONFIG_SCORE_TEXT_SIZE 1
#define MYCONFIG_SCORE_XLOCATION 15
#define MYCONFIG_SCORE_YLOCATION 5
#define MYCONFIG_ATTEMPTS_XLOCATION 140
#define MYCONFIG_ATTEMPTS_YLOCATION 5
#define MYCONFIG_NUM_SCORE_XLOCATION (MYCONFIG_SCORE_XLOCATION + 50)
#define MYCONFIG_NUM_SCORE_YLOCATION MYCONFIG_SCORE_YLOCATION
#define MYCONFIG_NUM_ATTEMPTS_XLOCATION (MYCONFIG_ATTEMPTS_XLOCATION + 65)
#define MYCONFIG_NUM_ATTEMPTS_YLOCATION MYCONFIG_ATTEMPTS_YLOCATION

// pause text
#define MYCONFIG_PAUSE_XLOCATION 15
#define MYCONFIG_PAUSE_YLOCATION 15

// end game text
#define MYCONFIG_ENDGAME_TEXT_COLOR DISPLAY_WHITE
#define MYCONFIG_ENDGAME_TEXT_SIZE 4
#define MYCONFIG_ENDGAME_XLOCATION 60
#define MYCONFIG_ENDGAME_YLOCATION 100
#define MYCONFIG_SUBENDGAME_TEXT_SIZE 1
#define MYCONFIG_SUBENDGAME_XLOCATION 90
#define MYCONFIG_SUBENDGAME_YLOCATION 140

// snake
#define MYCONFIG_SNAKE_MAX_LENGTH (MYCONFIG_TILE_HEIGHT * MYCONFIG_TILE_WIDTH)
#define MYCONFIG_STARTING_SNAKE_LENGTH 3
#define MYCONFIG_SNAKE_HEAD_START_LOCATION_X 1
#define MYCONFIG_SNAKE_HEAD_START_LOCATION_Y 1
#define MYCONFIG_SNAKE_COLOR DISPLAY_GREEN
#define MYCONFIG_SNAKE_BORDER_COLOR DISPLAY_DARK_GREEN

// snake speed
#define MYCONFIG_SNAKE_CHANGE_SPEED true
#define MYCONFIG_STARTING_SNAKE_SPEED EIGTH_SPEED

// apples
#define MYCONFIG_APPLE_COLOR DISPLAY_RED

// obstacle
#define MYCONFIG_OBSTACLE_ON true
#define MYCONFIG_OBSTACLE_NUM 3
#define MYCONFIG_OBSTACLE_COLOR DISPLAY_YELLOW
#define MYCONFIG_OBSTACLE_EDGE_COLOR DISPLAY_DARK_RED

// enum of different snake speed values
typedef enum {
  FULL_SPEED = 1,
  HALF_SPEED = 2,
  QUARTER_SPEED = 4,
  EIGTH_SPEED = 8,
  SIXTEENTH_SPEED = 16
} snake_speed_t;

#endif /* CONFIG_H */