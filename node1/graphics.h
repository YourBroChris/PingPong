#pragma once
#include "oled.h"
#include "adc.h"

typedef enum {
    PLAY = 0,
    EXIT = 1
} position;

typedef enum{
    MAINMENU = 0,
    NAME = 1,
    GAME = 2,
    SCORE = 3
} menu_state;

typedef struct {
    char name[4];   // 3 letters + null terminator
    uint16_t score; // seconds survived
} highscore_t;


void menu(pos_t * joystick_pos, pos_t * slider_pos, char button, uint8_t * goal, menu_state * state);