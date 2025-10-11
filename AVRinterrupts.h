#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

void init_interrupts(void);
ISR(USART1_RXC_vect);
