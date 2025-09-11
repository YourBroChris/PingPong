#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "AVRinterrupts.h"

extern volatile int ReceiveFlag;     // <-- declare as extern


void init_interrupts(void)
{
    // Disable global interrupts
    cli();


    //UART1 RX rising edge interrupt configuration
    //UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
    // UART1 RX interrupt enable
    UCSR1B |= (1 << RXCIE1);

    // Enable global interrupts  
    sei();
    
}

ISR(USART1_RXC_vect)
{
    // Handle UART1 RX interrupt
    if (UDR1 == 'a')
    {
        ReceiveFlag = 1;
    }
    //printf("Interrupt received\r\n");
    // Process the received character
}