#include "usart.h"

int main()
{
    usart_init(MYUBBR);

    while(1)
    {
        usart_transmit('a');
    }

}