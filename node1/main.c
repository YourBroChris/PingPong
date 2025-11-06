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

pos_t joystick_pos, slider_pos;
position currentPosition = PLAY;

can_message msg_out = {
    .id = 54,
    .length = 2,
    .data = {0b00111000}
};
// msg_out.id = 54;
// msg_out.length = 2;
// msg_out.data[0] = 0b00111000;

can_message msg_in;

void can_test(){
    init_can();
    while(1){
        //if(receive_can(&msg_out)){
            transmit_can(&msg_out, 0);
            _delay_ms(1000);   
        //}
        
        /*
        if (read_instruction(MCP_CANINTF) & MCP_RX0IF){
            receive_can(&msg_in);
        };
        
        printf("ID: %d \r\n", msg_in.id);
        printf("LENGTH: %d \r\n", msg_in.length);
        printf("DATA: %d \r\n", msg_in.data[0]);
        }
        */
    }
}

int main()
{
    init_usart(MYUBBR);
    init_xmem();
    init_ADC_clk();    
    init_spi();
    oled_init();
    init_interrupts();
    init_can();

    _delay_ms(4000); // 4 second delay to allow for flashing after reset in case something blocks flashing later

    printf("Initializing\r\n");
    frame_clear();
    oled_clear();
    //char * testString = "HELLO WORLD";
    //oled_printf(testString, 0, 1, NORMAL);
    //can_test();
    while(1){
        if(oledFlag == 1){
            updateOLED(FULL);
        }

        if (ReceiveFlag){
            //char received_char = receive_usart(received_char);
            //printf("Received\n");
            ReceiveFlag = 0;
        }
        
        slave_select(IO);
        oled_command(0x04);
        _delay_us(40);
        char right = read_byte();
        char left = read_byte();
        char nav = read_byte();
        //printf("Right button: %3d, Left button: %3d, Nav button: %3d\r\n", right, left, nav);

        pos_read(&slider_pos, &joystick_pos);
        menu(&joystick_pos, &slider_pos, &currentPosition);
        receive_can(&msg_in);
        printf("CAN Message, ID: %d Length: %d Data: %d\r\n", msg_in.id, msg_in.length, msg_in.data);
        transmit_can(&msg_out, 0);
        //printf("Joystick position:  X:%3d\t  Y:%3d   Slider position:   X:%3d\t  Y:%3d\r\n", joystick_pos.x, joystick_pos.y, slider_pos.x, slider_pos.y);
        //_delay_us(50000);
        //printf("------------------\r\nJoystick position:\r\nX:%3d\tY:%3d\r\nSlider position:\r\nX:%3d\tY:%3d\r\n------------------\r\n\r\n", joystick_pos.x, joystick_pos.y, slider_pos.x, slider_pos.y);
    }   
    return 0;
}

