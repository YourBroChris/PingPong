#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

#define F_CPU 4915200 
void init_interrupts(void);
ISR(USART1_RXC_vect);
