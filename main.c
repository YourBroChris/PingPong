#include "usart.h"
#include <avr/io.h>

int main()
{
    init_usart(MYUBBR);

    while(1)
    {
        transmit_usart('a');
    }

}
