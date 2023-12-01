#include <stdio.h>

#include "display.h"
#include "interrupts.h"
#include "buttons.h"
#include "intervalTimer.h"
#include "touchscreen.h"
#include "myConfig.h"
#include "gameControl.h"

// helper function declarations
void gameControl_isr();
static void init_hardware();

// infinite while loop
int main() {
  init_hardware();
  gameControl_init();
  while(1){
  }
    }

// tick game contoller when timer ends
void gameControl_isr() {
    intervalTimer_ackInterrupt(INTERVAL_TIMER_0);
    touchscreen_tick();
    gameControl_tick();
}

// initialize all hardware
static void init_hardware() {
  display_init();
  buttons_init();
  touchscreen_init(MYCONFIG_GAME_TIMER_PERIOD);

  // Initialize timer interrupts
  interrupts_init();
  interrupts_register(INTERVAL_TIMER_0_INTERRUPT_IRQ, gameControl_isr);
  interrupts_irq_enable(INTERVAL_TIMER_0_INTERRUPT_IRQ);

  intervalTimer_initCountDown(INTERVAL_TIMER_0, MYCONFIG_GAME_TIMER_PERIOD);
  intervalTimer_enableInterrupt(INTERVAL_TIMER_0);
  intervalTimer_start(INTERVAL_TIMER_0);
}