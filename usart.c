#include "usart.h"
#include <avr/io.h>

void init_usart(unsigned int ubrr)
{
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8data, 2stopbit
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}

void transmit_usart(unsigned char data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer
    UDR0 = data;
}

unsigned char receive_usart()
{
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));

    // Return data receiver from buffer
    return UDR0;
}