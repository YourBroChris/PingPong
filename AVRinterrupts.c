#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "AVRinterrupts.h"

extern volatile int ReceiveFlag;     // <-- declare as extern
extern volatile int oledFlag;
#define constant oledTimerFreq 30; //30 Hz


void init_interrupts(void)
{
    // Disable global interrupts
    cli();


    //UART1 RX rising edge interrupt configuration
    //UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
    // UART1 RX interrupt enable
    UCSR1B |= (1 << RXCIE1);

    //Timer interrupt
    TCCR0  = (1 << WGM01);
    OCR0   = F_CPU/(1024*30) - 1;
    TIMSK |= (1 << OCIE0);
    TCCR0 |= (1 << CS02) | (1 << CS00);
 

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

ISR(TIMER0_COMP_vect){
    oledFlag = 1;
}