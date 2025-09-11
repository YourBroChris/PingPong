#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"



void init_interrupts(void)
{
    // Disable global interrupts
    cli();


    //UART1 RX rising edge interrupt configuration
    //UCSR1B |= (1 << RXEN1);
    //UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
    // UART1 RX interrupt enable
    UCSR1B |= (1 << RXCIE1);

    // Enable global interrupts  
    sei();
    
}

ISR(USART1_RX_vect)
{
    // Handle UART1 RX interrupt
    ReceiveFlag = 1;
    //printf("Interrupt received\r\n");
    // Process the received character
}