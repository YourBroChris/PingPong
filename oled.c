#include "oled.h"
#include "spi.h"
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

void oled_init(void)
{
    
    // Initialization sequence
    oled_command(0xA1); // Segment remap
    oled_command(0xC8); // Scan direction
    oled_command(0xAF); // Display on



    /*
    oled_command(0xAE); // Display off
    oled_command(0xA4); // Normal display mode
    oled_command(0xD5); // Set display clock divide ratio/oscillator frequency
    oled_command(0x80); // Suggested value 0x80
    oled_command(0xA8); // Set multiplex ratio
    oled_command(0x3F); // 1/64 duty
    oled_command(0xD3); // Set display offset
    oled_command(0x00); // No offset
    oled_command(0x40); // Set start line address to 0
    oled_command(0x8D); // Enable charge pump regulator
    oled_command(0x14);
    oled_command(0x20); // Set memory addressing mode
    oled_command(0x00); // Horizontal addressing mode
    oled_command(0xA1); // Set segment re-map to normal
    oled_command(0xC8); // Set COM output scan direction to remapped mode
    oled_command(0xDA); // Set COM pins hardware configuration
    oled_command(0x12);
    oled_command(0x81); // Set contrast control
    oled_command(0xCF);
    oled_command(0xD9); // Set pre-charge period
    oled_command(0xF1);
    oled_command(0xDB); // Set VCOMH deselect level
    oled_command(0x40);
    oled_command(0xA6); // Normal display (not inverted)
    oled_command(0xAF); // Display on
    */
}

void oled_command(uint8_t command){
    command_data_set(0);
    write_byte(command, 2);
    return;
}


void oled_data(uint8_t data){
    command_data_set(1);
    write_byte(data, 2);
    return;
}

void io_command(uint8_t command){
    command_data_set(0);
    write_byte(command, 1);
    return;
}


void io_data(uint8_t data){
    command_data_set(1);
    write_byte(data, 1);
    return;
}



void oled_line(int line){

}

void oled_column(int column){

}

void oled_printf(char *str){

}

void funny_graphics(){

}

void command_data_set(int mode) {
    // Set DC pin (PORTD5) to mode
    // 0 data mode, 1 command mode
    if (mode == 0) {
        PORTD &= ~(1 << PD2); // Set low for command mode
    } else {
        PORTD |= (1 << PD2); // Set high for data mode
    }
}
