#include "usart.h"
#include <avr/io.h>
#include <stdio.h>
#include "AVRinterrupts.h"

volatile int ReceiveFlag = 0;        // <-- define here


int main()
{
    init_usart(MYUBBR);
    init_interrupts();
    /*
    while(1){
        test_transmit();
    }
    */
    //test_receive();
    while(1){
        if (ReceiveFlag)
        {
            //char received_char = receive_usart(received_char);
            printf("Received\n");
            ReceiveFlag = 0;
        }
        
        //printf("Test\n");
    }
    return 0;
}

/*S
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