#include "can.h"
#include <avr/io.h>
#include "spi.h"


void can_init(){
    DDRB |= (1 << PB4);
    slave_select(NONE);
}

void read_instruction(uint8_t addr){
    slave_select(CAN);
    SPDR = (uint8_t)0b00000011; // READ instruction = 00000011
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    SPDR = addr;
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    slave_select(NONE);
}

void write_instruction(uint8_t addr, uint8_t data){
    slave_select(CAN);
    SPDR = (uint8_t)0b00000010; // READ instruction = 00000011
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    SPDR = addr;
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    // slave_select(NONE); Ikke dra opp CS før man er ferdig med å skrive, kan fortsette å skrive
}

