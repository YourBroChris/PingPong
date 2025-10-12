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

void oled_init(void);
void oled_command(uint8_t command);
void oled_data(uint8_t data);
void oled_line(uint8_t line);
void oled_column(uint8_t column);
int oled_printf(char *str, uint8_t column, uint8_t line, FontType font);
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
    write_byte(command, OLED);
    return;
}


void oled_data(uint8_t data){
    command_data_set(DATA);
    write_byte(data, OLED);
    return;
}

void io_command(uint8_t command){
    command_data_set(COMMAND);
    write_byte(command, IO);
    return;
}


void io_data(uint8_t data){
    command_data_set(DATA);
    write_byte(data, IO);
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

int oled_printf(char *str, uint8_t column, uint8_t line, FontType font){
    // Get font information
    const FontDescriptor *font_p = get_font(font);
    const uint8_t *font_data = (const uint8_t *)font_p->data;
    uint8_t width = font_p -> width;
    slave_select(OLED);
    oled_command(0x20); // Set Memory Addressing Mode
    oled_command(0x00); // 0b = Horizontal Addressing Mode, 1b = Vertical Addressing Mode
    oled_line(line);
    oled_column(column);
    //printf("%p\r\n", &font5['A' - 32][0]);
    //printf("%p\r\n", &font_data[( 'A' - 32 ) * width]);


    while ( *str) {
        char current_char = *str++;
        if (current_char < 32 || current_char > 126){
            current_char = '?';
            //return -1;
        }
        for (uint8_t i = 0; i < width; i++) {
            uint16_t index = (current_char - 32) * width + i;
            uint8_t column_byte = pgm_read_byte(&font_data[index]);
            oled_data(column_byte);
        }
        oled_data(0x00);
    }
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
