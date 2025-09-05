#include "usart.h"

void init_usart(unsigned int ubbr)
{
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char)ubrr;

    UCSRB = (1 << RXEN) | (1 << TXEN);

    UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
}

void transmit_usart(unsigned char data)
{
    while (!(UCSRA & (1 << UDRE)));

    UDR = data;
}

unsigned char receive_usart()
{

    while (!(UCSRA & (1 << RXC)));

    return UDR;
}