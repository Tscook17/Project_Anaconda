#include "display.h"
#include <math.h>
#include <stdio.h>

#define WAITING_ST_MSG "in the waiting_t state\n"
#define ADC_SETTLING_ST_MSG "in the adc settling_t state\n"
#define PRESSED_ST_MSG "in the pressed_t state\n"
#define ADC_WAITTIME 0.05

// Status of the touchscreen
typedef enum {
  TOUCHSCREEN_IDLE,    // Touchscreen is idle (not pressed)
  TOUCHSCREEN_PRESSED, // Touchscreen is actively being pressed
  TOUCHSCREEN_RELEASED // Touchscreen has been released, but not acknowledged
} touchscreen_status_t;

// SM states
enum touchscreen_st_t {
  waiting_st,      // Waiting for input state
  adc_settling_st, // Waiting for ADC timer to settle state
  pressed_st       // Screen is pressed state
} currentState,
    previousState;

static touchscreen_status_t currentStatus;
static display_point_t touchLocation = {0, 0};
static int32_t adc_settle_ticks;
static int32_t adc_timer;
static bool pressed;

static void debugStatePrint();

// Initialize the touchscreen driver state machine, with a given tick period (in
// seconds).
void touchscreen_init(double period_seconds) {
  currentState = waiting_st;
  currentStatus = TOUCHSCREEN_IDLE;
  adc_timer = 0;
  pressed = false;
  adc_settle_ticks = ceil(ADC_WAITTIME / period_seconds);
}

// Tick the touchscreen driver state machine
void touchscreen_tick() {
  int8_t zvalue = 0;
  // Prints current state if changes
  //debugStatePrint();

  // State update SM
  switch (currentState) {
  case waiting_st:
    // change to adc_settling if touched
    if (display_isTouched()) {
      display_clearOldTouchData();
      currentState = adc_settling_st;
    } else {
      currentState = waiting_st;
    }
    break;
  case adc_settling_st:
    // change to waiting if not touched, if timer done and touched, pressed
    if (!display_isTouched()) {
      currentState = waiting_st;
    } else if (display_isTouched() && (adc_timer >= adc_settle_ticks)) {
      currentState = pressed_st;
      display_getTouchedPoint(&touchLocation.x, &touchLocation.y, &zvalue);
      currentStatus = TOUCHSCREEN_PRESSED;
    } else {
      currentState = adc_settling_st;
    }
    break;
  case pressed_st:
    // when no longer touched, back to waiting
    if (!display_isTouched()) {
      currentState = waiting_st;
      currentStatus = TOUCHSCREEN_RELEASED;
    } else {
      currentState = pressed_st;
    }
    break;
  default:
    break;
  }

  // State action SM
  switch (currentState) {
  case waiting_st:
    adc_timer = 0;
    break;
  case adc_settling_st:
    adc_timer++;
    break;
  case pressed_st:
    pressed = true;
    break;
  default:
    break;
  }
}

// Return the current status of the touchscreen
touchscreen_status_t touchscreen_get_status() { return currentStatus; }

// Acknowledge the touchscreen touch.  This function will only have effect when
// the touchscreen is in the TOUCHSCREEN_RELEASED status, and will cause it to
// switch to the TOUCHSCREEN_IDLE status.
void touchscreen_ack_touch() {
  if (currentStatus == TOUCHSCREEN_RELEASED) {
    currentStatus = TOUCHSCREEN_IDLE;
    pressed = false;
  }
}

// Get the (x,y) location of the last touchscreen touch
display_point_t touchscreen_get_location() { return touchLocation; }

// This is a debug state print routine. It will print the names of the states
// each time tick() is called. It only prints states if they are different than
// the previous state.
static void debugStatePrint() {
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state
  // name over and over.
  if (previousState != currentState || firstPass) {
    firstPass = false; // previousState will be defined, firstPass is false.
    previousState =
        currentState;       // keep track of the last state that you were in.
    switch (currentState) { // This prints messages based upon the state that
                            // you were in.
    case waiting_st:
      printf(WAITING_ST_MSG);
      break;
    case adc_settling_st:
      printf(ADC_SETTLING_ST_MSG);
      break;
    case pressed_st:
      printf(PRESSED_ST_MSG);
      break;
    }
  }
}
