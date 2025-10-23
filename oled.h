#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum{
    CLEAN = 0,
    FULL = 1,
    DIRTY = 2
} updateMode;

typedef enum{
    COMMAND = 0,
    DATA = 1
} mode_t;

typedef enum{
    SMALL = 0,
    NORMAL = 1,
    LARGE = 2
} FontType;

typedef struct {
    const void *data;  // pointer to font array in PROGMEM
    uint8_t width;
    uint8_t height;
} FontDescriptor;

extern volatile uint8_t * const EXT_RAM;
extern const FontDescriptor fonts[];
const FontDescriptor* get_font(FontType font);

void oled_init(void);
void oled_command(uint8_t command);
void oled_data(uint8_t data);
void io_command(uint8_t command);
void io_data(uint8_t data);
void oled_clear(void);
void oled_line(uint8_t line);
void oled_column(uint8_t column);
int oled_printf(char *str, uint8_t column, uint8_t line, FontType font);
void funny_graphics(void);
void command_data_set(int mode_t);
void frame_clear(void);
uint16_t get_frame_index(uint8_t column, uint8_t row);
int oled_write(char data, uint8_t column, uint8_t line, FontType font);