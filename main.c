#include "usart.h"
#include <avr/io.h>

int main()
{
    init_usart(MYUBBR);
    return;
}

void test_transmit(){
    while(1){
        transmit_usart('a');
        transmit_usart('b');
        transmit_usart('c');
    }
    return:
}

void test_receive(){
        DDRB = (1 << DDB4);
        while(1){
        unsigned char received = receive_usart();
        if(received == 'a') {
            PORTB = (1 << PB4);
        }
    }
}
