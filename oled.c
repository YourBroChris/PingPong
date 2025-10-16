#include "oled.h"
#include "spi.h"
#include "fonts.h"
#include "usart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

//#define EXT_RAM 0x1400
#define OLED_WIDTH   128
#define OLED_HEIGHT  64
#define OLED_PAGES   8
#define FRAME_SIZE      (OLED_WIDTH*OLED_PAGES) // 1024 bytes

volatile uint8_t * const EXT_RAM = (volatile uint8_t *)0x1400;


void oled_init(void);
void oled_command(uint8_t command);
void oled_data(uint8_t data);
void oled_line(uint8_t line);
void oled_column(uint8_t column);
int  oled_printf(char *str, uint8_t column, uint8_t line, FontType font);
void funny_graphics(void);
void command_data_set(int mode);

const FontDescriptor fonts[] = {
    { font4, 4, 6 },
    { font5, 5, 7 },
    { font8, 8, 8 }
};

const FontDescriptor* get_font(FontType font) {
    return &fonts[font];
}


void oled_init(void)
{
    slave_select(OLED);
    // Initialization sequence
    oled_command(0xA1); // Segment remap
    oled_command(0xC8); // Scan direction
    oled_command(0xAF); // Display on
    slave_select(NONE);

}

void oled_command(uint8_t command){
    command_data_set(COMMAND);
    write_byte(command);
    return;
}


void oled_data(uint8_t data){
    command_data_set(DATA);
    write_byte(data);
    return;
}

void io_command(uint8_t command){
    command_data_set(COMMAND);
    write_byte(command);
    return;
}


void io_data(uint8_t data){
    command_data_set(DATA);
    write_byte(data);
    return;
}

void oled_clear(){
    slave_select(OLED);
    oled_command(0x20); // Set Memory Addressing Mode
    oled_command(0x00); // 0b = Horizontal Addressing Mode, 1b = Vertical Addressing Mode
    oled_command(0x22); // Set Page Start Address
    oled_command(0x00);
    oled_command(0x07);
    for(int i = 0; i < (8*128); ++i){
        oled_data(0x00);
    }
    slave_select(NONE);
}

void oled_line(uint8_t line){
    oled_command(0x22);
    oled_command(line);
    oled_command(0x07);
}

void oled_column(uint8_t column){
    oled_command(0x21);
    oled_command(column);
    oled_command(0x7F);
}

void frame_clear(void) {
    for (uint16_t i = 0; i < FRAME_SIZE; ++i) EXT_RAM[i] = 0x00;
}

uint16_t get_frame_index(uint8_t column, uint8_t row) {
    // page: 0..7, col: 0..127
    return (uint16_t)row * OLED_WIDTH + column;
}


int oled_printf(char *str, uint8_t column, uint8_t row, FontType font){
    // Get font information
    const FontDescriptor *font_p = get_font(font);
    const uint8_t *font_data = (const uint8_t *)font_p->data;
    uint8_t width = font_p -> width;

    if (row >= OLED_PAGES || column >= OLED_WIDTH) return -1;
    

    while ( *str) {
        char current_char = *str++;
        if (current_char < 32 || current_char > 126){
            current_char = '?';
            //return -1;
        }

        if ((uint16_t)column + width > OLED_WIDTH) break;

        for (uint8_t i = 0; i < width; i++) {
            uint16_t index = (current_char - 32) * width + i;
            uint8_t column_byte = pgm_read_byte(&font_data[index]);
            EXT_RAM[get_frame_index(column++, row)] = column_byte;
        }

        if (column < OLED_WIDTH) {
            EXT_RAM[get_frame_index(column++, row)] = 0x00;
        }
    }
    return 0;
}

int oled_write(char data, uint8_t column, uint8_t line, FontType font){
    // Get font information
    slave_select(OLED);
    oled_command(0x20); // Set Memory Addressing Mode
    oled_command(0x00); // 0b = Horizontal Addressing Mode, 1b = Vertical Addressing Mode
    oled_line(line);
    oled_column(column);
    oled_data(data);
    slave_select(NONE);
    return 0;
}




void draw_box(int x, int y, int length, int height){

}

void funny_graphics(){


}

void command_data_set(int mode_t) {
    // Set DC pin (PORTD5) to mode
    // 0 data mode, 1 command mode
    if (mode_t == 0) {
        PORTD &= ~(1 << PD2); // Set low for command mode
    } else {
        PORTD |= (1 << PD2); // Set high for data mode
    }
}
