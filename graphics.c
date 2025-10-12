#include "graphics.h"
#include "oled.h"

#define EXT_RAM 0x1400 // External ram base address



void menu(){
    char * menuTitle = "MENU";
    char * playButton = "PLAY";
    char * exitButton = "EXIT";

    oled_printf(menuTitle, 45, 0, LARGE);
    oled_printf(playButton, 50, 2, NORMAL);
    oled_printf(exitButton, 50, 4, NORMAL);
}