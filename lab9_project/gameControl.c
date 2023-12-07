#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "buttons.h"
#include "display.h"
#include "gameControl.h"
#include "myConfig.h"
#include "snake.h"
#include "snakeMap.h"
#include "switches.h"
#include "touchscreen.h"

#define undraw true
#define draw false

#define TOP_SIDE (MYCONFIG_UPPER_BOARDER - MYCONFIG_BOARDER_OFFSET)
#define BOT_SIDE (MYCONFIG_LOWER_BOARDER + MYCONFIG_BOARDER_OFFSET)
#define LEFT_SIDE (MYCONFIG_LEFT_BOARDER - MYCONFIG_BOARDER_OFFSET)
#define RIGHT_SIDE (MYCONFIG_RIGHT_BOARDER + MYCONFIG_BOARDER_OFFSET)

// helper function declarations
static void dropApple();
static void dropObstacle();
static void drawSquare(mapSpaceLocation_t location, int16_t display_color);
static void setMap();
static void startScreen(bool erase);
static void titleScreen(bool erase);
static void drawPaused(bool erase);
static void drawScore();
static void drawAttempts();
static void endGameScreen();

// game control sm state type
typedef enum {
  init_st,        // start up state
  title_st,       // show title
  startScreen_st, // basic instructions
  playing_st,     // actively playing game
  paused_st,      // pause menu
  endGame_st      // end game sequence
} gameControl_st_t;

// global variables
static gameControl_st_t currentState;
static gameControl_st_t previousState;
static uint32_t startDelayCnt;
static uint32_t startDelayTicks;
static snakemap_t currentMap;
static snakemap_t *mapPtr;
static int8_t score;
static int8_t numAttempts;
static uint8_t speedControl;
static button_indicator buttonInput = none;
static button_indicator *buttonPtr;

// call before running tick or to reset
void gameControl_init() {
  currentState = init_st;
  mapPtr = &currentMap;
  buttonPtr = &buttonInput;
  startDelayTicks =
      (int)(MYCONFIG_STARTSCREEN_DELAY / MYCONFIG_GAME_TIMER_PERIOD);
  srand(numAttempts);
}

// ticks game control sm
void gameControl_tick() {
  // action sm
  switch (currentState) {
  case init_st:
    display_fillScreen(MYCONFIG_BACKGROUND_COLOR);
    titleScreen(draw);
    snakemap_clear(mapPtr);
    snake_init(mapPtr, buttonPtr);
    break;
  case title_st:
    startDelayCnt = 0;
    break;
  case startScreen_st:
    startDelayCnt++;
    break;
  case playing_st:
    gameControl_checkButton();
    if (!(speedControl % MYCONFIG_SNAKE_SPEED)) { // determines snake speed
      snake_tick();
    }
    speedControl++;
    dropApple();
    dropObstacle();
    drawScore();
    break;
  case paused_st:
    break;
  case endGame_st:
    break;
  default:
    break;
  }

  // transition sm
  switch (currentState) {
  case init_st:
    currentState = title_st;
    break;
  case title_st:
    if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
      touchscreen_ack_touch();
      titleScreen(undraw);
      startScreen(draw);
      currentState = startScreen_st;
    } else {
      currentState = title_st;
    }
    break;
  case startScreen_st:
    if (startDelayCnt >= startDelayTicks) {
      startScreen(undraw);
      setMap();
      currentState = playing_st;
    } else {
      currentState = startScreen_st;
    }
    break;
  case playing_st:
    if (switches_read() & SWITCHES_SW0_MASK) { // switch
      currentState = paused_st;
      drawPaused(draw);
    } else if (0) { // deaded
      currentState = endGame_st;
      endGameScreen();
    } else {
      currentState = playing_st;
    }
    break;
  case paused_st:
    if (~switches_read() & SWITCHES_SW0_MASK) { // unswitched
      currentState = playing_st;
      drawPaused(undraw);
    } else if (buttons_read() & BUTTONS_BTN2_MASK) { // reset button pressed
      currentState = init_st;
      numAttempts++;
    } else {
      currentState = paused_st;
    }
    break;
  case endGame_st:
    if (buttons_read() & BUTTONS_BTN2_MASK) { // reset button pressed
      currentState = init_st;
    } else {
      currentState = endGame_st;
    }
    break;
  default:
    break;
  }
}

// drops apple on map
static void dropApple() {
  // if no apple present, drop apple
  if (!mapPtr->haveApple) {
    int8_t apple_x = (rand() % MYCONFIG_TILE_WIDTH);
    int8_t apple_y = (rand() % MYCONFIG_TILE_HEIGHT);
    mapPtr->snakeMap[apple_x][apple_y] = MAPSPACE_APPLE;
    mapPtr->haveApple = true;
    drawSquare(set_snake_location(apple_x, apple_y), MYCONFIG_APPLE_COLOR);
  }
}

// drops obstacles on map
static void dropObstacle() {}

// draw/erase square such color at specified location
static void drawSquare(mapSpaceLocation_t location, int16_t display_color) {
  display_point_t square = snakeMap_getLocationFromTile(location);
  display_fillRect(square.x, square.y, MYCONFIG_TILE_SIZE, MYCONFIG_TILE_SIZE,
                   display_color);
}

// draw map
static void setMap() {
  // set borders
  display_drawFastVLine(LEFT_SIDE, TOP_SIDE, (BOT_SIDE - TOP_SIDE),
                        MYCONFIG_BOARDER_COLOR);
  display_drawFastVLine(RIGHT_SIDE, TOP_SIDE, (BOT_SIDE - TOP_SIDE),
                        MYCONFIG_BOARDER_COLOR);
  display_drawFastHLine(LEFT_SIDE, TOP_SIDE, (RIGHT_SIDE - LEFT_SIDE),
                        MYCONFIG_BOARDER_COLOR);
  display_drawFastHLine(LEFT_SIDE, BOT_SIDE, (RIGHT_SIDE - LEFT_SIDE),
                        MYCONFIG_BOARDER_COLOR);
  drawAttempts();
}

// check to see if button has been pressed
void gameControl_checkButton() {
  if (buttons_read() == BUTTONS_BTN3_MASK) {
    buttonInput = left_button;
  } else if (buttons_read() == BUTTONS_BTN0_MASK) {
    buttonInput = right_button;
  }
}

// draw/erase instructions in start screen
static void startScreen(bool erase) {
  display_setTextColor(erase ? MYCONFIG_BACKGROUND_COLOR : MYCONFIG_TEXT_COLOR);
  display_setTextSize(MYCONFIG_STARTSCREEN_TEXT_SIZE);
  display_setCursor(0, MYCONFIG_STARTSCREEN_TEXT_YLOCATION);
  display_println("Press button 3 & 0 to turn\n"
                  "   flip switch0 to pause\n"
                  "\n"
                  " eat apples, avoid blocks,\n"
                  "\n"
                  "         SURVIVE!\n");
}

// draw/erase title screen
static void titleScreen(bool erase) {
  // title text
  display_setTextColor(erase ? MYCONFIG_BACKGROUND_COLOR
                             : MYCONFIG_TITLE_TEXT_COLOR);
  display_setTextSize(MYCONFIG_TITLE_TEXT_SIZE);
  display_setCursor(0, MYCONFIG_TITLE_TEXT_YLOCATION);
  display_println("   ANACONDA\n");

  // sub title text
  display_setTextColor(erase ? MYCONFIG_BACKGROUND_COLOR : MYCONFIG_TEXT_COLOR);
  display_setTextSize(MYCONFIG_SUBTITLE_TEXT_SIZE);
  display_setCursor(MYCONFIG_TITLE_TEXT_XLOCATION,
                    MYCONFIG_SUBTITLE_TEXT_YLOCATION);
  display_println("Tobias Cook/James Subieta\n");
  display_setCursor(MYCONFIG_TITLE_TEXT_XLOCATION,
                    MYCONFIG_SUBTITLE_TEXT_YLOCATION + 40);
  display_println("Touch the Screen to Start\n");
}

// draw/erase paused tag
static void drawPaused(bool erase) {
  display_setTextColor(erase ? MYCONFIG_BACKGROUND_COLOR : MYCONFIG_TEXT_COLOR);
  display_setTextSize(MYCONFIG_SCORE_TEXT_SIZE);
  display_setCursor(MYCONFIG_PAUSE_XLOCATION, MYCONFIG_PAUSE_YLOCATION);
  display_println("PAUSED: restart game by pressing button2");
}

// draw/update score
static void drawScore() {
  // print score
  display_setTextSize(MYCONFIG_SCORE_TEXT_SIZE);
  display_setCursor(MYCONFIG_SCORE_XLOCATION, MYCONFIG_SCORE_YLOCATION);
  display_setTextColor(MYCONFIG_TEXT_COLOR);
  display_println("Score: \n");

  // erases old number
  if (score > 0) {
    display_setTextColor(MYCONFIG_BACKGROUND_COLOR);
    display_setCursor(MYCONFIG_NUM_SCORE_XLOCATION,
                      MYCONFIG_NUM_SCORE_YLOCATION);
    display_printDecimalInt(score - 1);
  }

  display_setCursor(MYCONFIG_NUM_SCORE_XLOCATION, MYCONFIG_NUM_SCORE_YLOCATION);
  display_setTextColor(MYCONFIG_TEXT_COLOR);
  display_printDecimalInt(score);
}

// draw attempts
static void drawAttempts() {
  // print score
  display_setTextSize(MYCONFIG_SCORE_TEXT_SIZE);
  display_setCursor(MYCONFIG_ATTEMPTS_XLOCATION, MYCONFIG_ATTEMPTS_YLOCATION);
  display_setTextColor(MYCONFIG_TEXT_COLOR);
  display_println("Attempts: \n");

  display_setCursor(MYCONFIG_NUM_ATTEMPTS_XLOCATION,
                    MYCONFIG_NUM_ATTEMPTS_YLOCATION);
  display_setTextColor(MYCONFIG_TEXT_COLOR);
  display_printDecimalInt(numAttempts);
}

// draw endgame tag
static void endGameScreen() {
  display_setTextColor(MYCONFIG_ENDGAME_TEXT_COLOR);
  display_setTextSize(MYCONFIG_ENDGAME_TEXT_SIZE);
  display_setCursor(MYCONFIG_ENDGAME_XLOCATION, MYCONFIG_ENDGAME_YLOCATION);
  display_println("GAME OVER\n");

  display_setTextSize(MYCONFIG_SUBENDGAME_TEXT_SIZE);
  display_setCursor(MYCONFIG_SUBENDGAME_XLOCATION,
                    MYCONFIG_SUBENDGAME_YLOCATION);
  display_println("Press button2 to restart\n");
}
