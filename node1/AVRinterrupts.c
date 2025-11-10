#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "AVRinterrupts.h"

extern volatile uint8_t ReceiveFlag;     // <-- declare as extern
extern volatile uint8_t oledFlag;
extern volatile uint8_t canFlag;
int oledTimerFreq = 30; //30 Hz
int canTimerFreq = 1; //30 Hz

void init_interrupts(void)
{
    // Disable global interrupts
    cli();


    //UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
    // UART1 RX interrupt enable
    UCSR1B |= (1 << RXCIE1);

    // Timer interrupts
    // OLED TIMER
    TCCR0  = (1 << WGM01);
    OCR0   = F_CPU/(1024 * oledTimerFreq) - 1;
    TIMSK |= (1 << OCIE0);
    TCCR0 |= (1 << CS02) | (1 << CS00);
    // CAN TIMER
    TCCR2  = 0;
    TCNT2  = 0;
    OCR2   = (uint8_t)((F_CPU / 1024) / (uint32_t)canTimerFreq - 1); // 159
    TIFR  |= (1 << OCF2);
    TIMSK |= (1 << OCIE2);
    TCCR2  = (1 << WGM21) | (1 << CS22) | (1 << CS21) | (1 << CS20); // CTC, /1024


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

ISR(TIMER2_COMP_vect){
    canFlag = 1;
}