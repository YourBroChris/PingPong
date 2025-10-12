#pragma once
#include "oled.h"
#include "adc.h"

typedef enum {
    PLAY = 0,
    EXIT = 1
} position;

void menu(pos_t * joystick_pos, pos_t * slider_pos, position * currentPosition);