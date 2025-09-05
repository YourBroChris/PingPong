#include "usart.h"

void init_usart(unsigned int ubbr)
{
    // Set baud rate
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char)ubrr;

    // Enable receiver and transmitter
    UCSRB = (1 << RXEN) | (1 << TXEN);

    // Set frame format: 8data, 2stopbit
    UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
}

void transmit_usart(unsigned char data)
{
    // Wait for empty transmit buffer
    while (!(UCSRA & (1 << UDRE)));

    // Put data into buffer
    UDR = data;
}

unsigned char receive_usart()
{
    // Wait for data to be received
    while (!(UCSRA & (1 << RXC)));

    // Return data receiver from buffer
    return UDR;
}