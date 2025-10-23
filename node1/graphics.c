#include "graphics.h"
#include "oled.h"
#include "adc.h"

#define EXT_RAM 0x1400 // External ram base address

initial = 0;

void menu(pos_t *joystick_pos, pos_t *slider_pos, position *currentPosition) {
    char *menuTitle = "MENU";
    char *playButton;
    char *exitButton;
    int exitPosition;
    int playPosition;
    //printf("Joystick position:  X:%3d\t  Y:%3d\r\n", joystick_pos->x, joystick_pos->y);
    
    switch (*currentPosition) {
        case PLAY:
            playButton = "-> PLAY";
            exitButton = "   EXIT";
            playPosition = (50-(6*3));
            exitPosition = 50;
            if (joystick_pos->y < -10) {
                *currentPosition = EXIT;
            }
            break;

        case EXIT:
            playButton = "   PLAY";
            exitButton = "-> EXIT";
            playPosition = 50;
            exitPosition = (50-(6*3));
            if (joystick_pos->y > 10) {
                *currentPosition = PLAY;
            }
            break;
    }
    //oled_clear();
    oled_printf(menuTitle, 45, 0, LARGE);
    oled_printf(playButton, (50-(6*3)), 2, NORMAL);
    oled_printf(exitButton, (50-(6*3)), 4, NORMAL);
}