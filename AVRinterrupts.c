#include <avr/io.h>
#include <avr/interrupt.h>

void init_interrupts(void)
{
    // Enable global interrupts
    SREG |= (1 << SREG_I);
    
}
