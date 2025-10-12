#include "can.h"
#include "spi.h"


void init_can(){
    // Kanskje ha dette i spi init heller idk?
    DDRB |= (1 << PB4);
    slave_select(NONE);
}

void issue_instruction(uint8_t can_instruction, uint8_t addr, uint8_t data, uint8_t txb_bits, uint8_t mask){
    slave_select(CAN);
    SPDR = can_instruction | txb_bits;
    while(!(SPSR & (1<<SPIF)));

    switch(can_instruction){
        case READ:
            SPDR = addr;
            while(!(SPSR & (1<<SPIF)));
        
        case WRITE: 
            SPDR = addr;
            while(!(SPSR & (1<<SPIF)));
            SPDR = data;
            while(!(SPSR & (1<<SPIF)));

        case BITMODIFY:
            SPDR = addr;
            while(!(SPSR & (1<<SPIF)));
            SPDR = mask;
            while(!(SPSR & (1<<SPIF)));
            SPDR = data;
            while(!(SPSR & (1<<SPIF)));
    }
    slave_select(NONE);
}

/*

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
    SPDR = (0b10000000 | txb_bits); // RTS instruction
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

void readstatus_instruction(){
    slave_select(CAN);
    SPDR = (uint8_t)0b10100000; // READ STATUS instruction
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    slave_select(NONE); // Kan holde CS nede lenger dersom man vil fortsette å lese status
}

void reset_instruction(){
    slave_select(CAN);
    SPDR = (uint8_t)0b11000000; // RESET instruction
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    slave_select(NONE);
}

*/