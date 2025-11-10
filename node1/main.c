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

volatile uint8_t ReceiveFlag = 0; 
volatile uint8_t oledFlag    = 0;
volatile uint8_t canFlag     = 0;

pos_t joystick_pos, slider_pos;
position currentPosition = PLAY;

can_message msg_out = {
    .id = 54,
    .length = 1,
    .data = {0b00000100}
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
    printf("Initializing\r\n");
    init_xmem();
    init_ADC_clk();    
    init_spi();
    oled_init();
    init_interrupts();
    init_can();

    _delay_ms(4000); // 4 second delay to allow for flashing after reset in case something blocks flashing later

    frame_clear();
    oled_clear();
    while(1){
        if(oledFlag == 1){
            updateOLED(FULL);
            oledFlag = 0;
        }

        if (ReceiveFlag){
            //char received_char = receive_usart(received_char);
            //printf("Received USART: %d\n", received_char);
            ReceiveFlag = 0;
        }
        printf("canFlag: %d\r\n", canFlag);
        if (canFlag){
            if (receive_can_any(&msg_in)){
            printf("CAN Message, ID:%u Length:%u Data0:%u\r\n", msg_in.id, msg_in.length, msg_in.data[0]);
            }
            // if (read_instruction(MCP_CANINTF) & ((MCP_RX0IF) | (MCP_RX1IF))) {
            //     receive_can_any(&msg_in);
            //     printf("CAN Message, ID:%d Length:%d Data0:%d\r\n", msg_in.id, msg_in.length, msg_in.data[0]);
            //     }
            transmit_can(&msg_out, 0);
            canFlag = 0;
        }
        //printf("We got past the can loop\r\n");
        
        slave_select(IO);
        oled_command(0x04);
        _delay_us(40);
        char right = read_byte();
        char left = read_byte();
        char nav = read_byte();
        slave_select(NONE);
        //printf("Right button: %3d, Left button: %3d, Nav button: %3d\r\n", right, left, nav);

        pos_read(&slider_pos, &joystick_pos);
        menu(&joystick_pos, &slider_pos, &currentPosition);
        
        // if (read_instruction(MCP_CANINTF & MCP_RX0IF)){
        //      receive_can(&msg_in);
        //     printf("CAN Message, ID: %d Length: %d Data: %d\r\n", msg_in.id, msg_in.length, msg_in.data);
        // }
        //transmit_can(&msg_out, 0);
        //printf("Joystick position:  X:%3d\t  Y:%3d   Slider position:   X:%3d\t  Y:%3d\r\n", joystick_pos.x, joystick_pos.y, slider_pos.x, slider_pos.y);
        //_delay_us(50000);
        //printf("------------------\r\nJoystick position:\r\nX:%3d\tY:%3d\r\nSlider position:\r\nX:%3d\tY:%3d\r\n------------------\r\n\r\n", joystick_pos.x, joystick_pos.y, slider_pos.x, slider_pos.y);
    }   
    return 0;
}

