#include <avr/io.h>
#include <stdio.h>

#define FOSC 4915200 // Clockspeed
#define BAUD 9600 // Baudrate
#define MYUBBR ((FOSC / (16UL * BAUD)) - 1)

void init_usart(unsigned int ubrr);
int transmit_usart(char data, FILE *file);
int receive_usart(FILE *file);
