#include "usart.h"
#include <avr/io.h>

int main()
{
    init_usart(MYUBBR);
    DDRB = (1 << DDB4);
    while(1)
    {
        transmit_usart('a');
        /*unsigned char received = receive_usart();
        if(received == 'a') {
            PORTB = (1 << PB4);
        }*/
    }

}
