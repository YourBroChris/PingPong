#include "can.h"
#include <avr/io.h>
#include "spi.h"


void can_init(){
    // Kanskje ha dette i spi init heller idk?
    DDRB |= (1 << PB4);
    slave_select(NONE);
}

void read_instruction(uint8_t addr){
    slave_select(CAN);
    SPDR = (uint8_t)0b00000011; // READ instruction
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    SPDR = addr;
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    slave_select(NONE);
}

void write_instruction(uint8_t addr, uint8_t data){
    slave_select(CAN);
    SPDR = (uint8_t)0b00000010; // WRITE instruction
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    SPDR = addr;
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    // slave_select(NONE); Kan fortsette å skrive på med mer data før man drar opp CS
}

void rts_instruction(uint8_t txb_bits){
    slave_select(CAN);
    SPDR = (0b10000000 | txb_bits);
    while(!(SPDR & (1<<SPIF)));
    slave_select(NONE);
}

void bitmodify_instruction(uint8_t addr, uint8_t data, uint8_t mask){
    slave_select(CAN);
    SPDR = (uint8_t)0b00000101; // BITMODIFY instruction
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    SPDR = addr;
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    SPDR = mask;
    while(!(SPSR & (1<<SPIF)));
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    slave_select(NONE);
}

void reset_instruction(){
    slave_select(CAN);
    SPDR = (uint8_t)0b11000000; // BITMODIFY instruction
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    slave_select(NONE);
}