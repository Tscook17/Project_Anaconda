#include <display.h>

#include "gameControl.h"
#include "myConfig.h"
#include "snakeMap.h"
#include "snake.h"

// helper function declarations
static void setMap();
static void startScreen(bool erase);

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
static uint32_t startDelayCnt;
static uint32_t startDelayTicks;
static snakemap_t currentMap;
static snakemap_t *mapPtr;

void gameControl_init() {
  currentState = init_st;
  mapPtr = &currentMap;
  startDelayTicks =
      (int)(MYCONFIG_STARTSCREEN_DELAY / MYCONFIG_GAME_TIMER_PERIOD);
}

void gameControl_tick() {
  // action sm
  switch (currentState) {
  case init_st:
    display_fillScreen(MYCONFIG_BACKGROUND_COLOR);
    startDelayCnt = 0;
    break;
  case title_st:
    break;
  case startScreen_st:
    startDelayCnt++;
    break;
  case playing_st:
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
    currentState = startScreen_st;
    startScreen(false);
    break;
  case title_st:
    break;
  case startScreen_st:
    if (startDelayCnt >= startDelayTicks) {
      startScreen(true);
      setMap();
      currentState = playing_st;
    } else {
      currentState = startScreen_st;
    }
    break;
  case playing_st:
    break;
  case paused_st:
    break;
  case endGame_st:
    break;
  default:
    break;
  }
}

static void setMap() {
  // set borders
  display_drawFastVLine(MYCONFIG_LEFT_BOARDER, MYCONFIG_UPPER_BOARDER,
                        (MYCONFIG_LOWER_BOARDER - MYCONFIG_UPPER_BOARDER),
                        MYCONFIG_BOARDER_COLOR);
  display_drawFastVLine(MYCONFIG_RIGHT_BOARDER, MYCONFIG_UPPER_BOARDER,
                        (MYCONFIG_LOWER_BOARDER - MYCONFIG_UPPER_BOARDER),
                        MYCONFIG_BOARDER_COLOR);
  display_drawFastHLine(MYCONFIG_LEFT_BOARDER, MYCONFIG_UPPER_BOARDER,
                        (MYCONFIG_RIGHT_BOARDER - MYCONFIG_LEFT_BOARDER),
                        MYCONFIG_BOARDER_COLOR);
  display_drawFastHLine(MYCONFIG_LEFT_BOARDER, MYCONFIG_LOWER_BOARDER,
                        (MYCONFIG_RIGHT_BOARDER - MYCONFIG_LEFT_BOARDER),
                        MYCONFIG_BOARDER_COLOR);
}

static void startScreen(bool erase) {
  display_setTextColor(erase ? MYCONFIG_BACKGROUND_COLOR : MYCONFIG_TEXT_COLOR);
  display_setTextSize(MYCONFIG_STARTSCREEN_TEXT_SIZE);
  display_setCursor(0, MYCONFIG_TEXT_YLOCATION);
  display_println("Press button3 and button0\n"
                  "  to turn left and right,\n"
                  " press button2/button1 to\n"
                  "  reset/pause, eat apples\n"
                  " avoid obstacles, survive\n");
}