#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "usart.h"
#include "AVRinterrupts.h"
#include "sram.h"
#include "adc.h"
#include "spi.h"
#include "oled.h"
#include "graphics.h"
#include "can.h"

#define F_CPU 4915200 // Clockspeed

volatile int ReceiveFlag = 0;        // <-- define here
volatile int oledFlag = 0;

int main()
{
    init_usart(MYUBBR);
    init_xmem();
    init_ADC_clk();
//  uint8_t currmode;
//  while(1){
//      currmode = init_can();
//      printf("Mode: %d \r\n", currmode);
// }

    _delay_ms(4000);
    
    init_spi();
    oled_init();
    init_interrupts();

    pos_t joystick_pos, slider_pos;
    position currentPosition = PLAY;

    printf("Initializing\r\n");
    frame_clear();
    oled_clear();
    //char * testString = "HELLO WORLD";
    //oled_printf(testString, 0, 1, NORMAL);
    while(1){
        if(oledFlag == 1){
            for (int i = 0; i < 8; i++){
                for (int j = 0; j < 128; j++){
                    uint8_t data_byte = EXT_RAM[(i*128) + j];
                    oled_write(data_byte, j, i, NORMAL);
                }
            }
            oledFlag == 0;
        }

        if (ReceiveFlag){
            //char received_char = receive_usart(received_char);
            //printf("Received\n");
            ReceiveFlag = 0;
        }
        
        slave_select(IO);
        io_command(0x04);
        _delay_us(40);
        char right = read_byte();
        char left = read_byte();
        char nav = read_byte();
        printf("Right button: %3d, Left button: %3d, Nav button: %3d\r\n", right, left, nav);

        pos_read(&slider_pos, &joystick_pos);
        menu(&joystick_pos, &slider_pos, &currentPosition);
        //printf("Joystick position:  X:%3d\t  Y:%3d   Slider position:   X:%3d\t  Y:%3d\r\n", joystick_pos.x, joystick_pos.y, slider_pos.x, slider_pos.y);
        //_delay_us(500000);
        //printf("------------------\r\nJoystick position:\r\nX:%3d\tY:%3d\r\nSlider position:\r\nX:%3d\tY:%3d\r\n------------------\r\n\r\n", joystick_pos.x, joystick_pos.y, slider_pos.x, slider_pos.y);
    }   
    return 0;
}

void latch_test(){
    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    const int ext_ram_size = 0x800; // Size of the SRAM in bytes
    printf("Latch test starting...\n");
    // Set latch enabled PORT E1
    PORTE |= (1 << PE1);
    // Enable PORTA and PORTC as output
    DDRA = 0xFF;
    // Write to external SRAM
    for (int i = 0; i < ext_ram_size; i++) {
        ext_ram[i] = i % 256;
    }
    
     
    //PORTA = 0b00000000;
    // Set latch disabled
    PORTE &= ~(1 << PE1);
}

/*
void test_transmit(){
        transmit_usart('a');
        transmit_usart('b');
        transmit_usart('c');
    return;
}

void test_receive(){
        while(1){
        unsigned char received = receive_usart();
        if(received == 'a') {
            transmit_usart('b');
        }
    }
    return;
}
*/