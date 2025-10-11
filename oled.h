#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void oled_init(void);
void oled_command(uint8_t command);
void oled_data(uint8_t data);
void oled_line(int line);
void oled_column(int column);
void oled_printf(char *str);
void funny_graphics(void);
void command_data_set(int mode);