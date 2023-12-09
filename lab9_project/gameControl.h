#ifndef GAMECONTROL_H
#define GAMECONTROL_H

// this is the button states it could possibly be
typedef enum { left_button_press, right_button_press, no_button_press } button_indicator_t;

// initialize game controller
void gameControl_init();

// tick game contoller sm
void gameControl_tick();

void gameControl_checkButton();
#endif /* GAMECONTROL_H */