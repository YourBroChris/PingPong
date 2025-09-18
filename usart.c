#include "usart.h"

void init_usart(unsigned int ubrr)
{
    // Set baud rate
    UBRR1H = (unsigned char)(ubrr>>8);
    UBRR1L = (unsigned char)ubrr;

    // Enable receiver and transmitter
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);

    // Set frame format: 2stopbit, 8data
    UCSR1C = (1 << URSEL1) | (1 << USBS1) | (3 << UCSZ10);

    fdevopen(transmit_usart, receive_usart);
}

int transmit_usart(char data)
{
    // Wait for empty transmit buffer
    while (!(UCSR1A & (1 << UDRE1)));

    // Put data into buffer
    UDR1 = data;
    return 0;
}

int receive_usart()
{
    // Wait for data to be received
    while (!(UCSR1A & (1 << RXC1)));

    // Return data receiver from buffer
    return UDR1;
}
