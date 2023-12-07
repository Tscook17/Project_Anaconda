#ifndef GAMECONTROL_H
#define GAMECONTROL_H

// this is the button states it could possibly be
typedef enum { left_button, right_button, none } button_indicator;

// initialize game controller
void gameControl_init();

// tick game contoller sm
void gameControl_tick();

void gameControl_checkButton();
#endif /* GAMECONTROL_H */