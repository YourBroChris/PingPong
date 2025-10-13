#include "spi.h"
#include <avr/io.h>


void init_spi(void){
    // Set MOSI, SCK as Output
    DDRB |= (1 << PB5) | (1 << PB7) | (1 << PB4);; // PB5 is MOSI, PB7 is SCK, PB4 is CAN slave
    DDRD |= (1 << PD0) | (1 << PD1) | (1 << PD2); // PD0 and PD1 as SS for slaves 2 and 3, PD2 for D/C selection
    slave_select(NONE);
    // Enable SPI, Set as Master
    // Prescaler: Fosc/16
    // Limit SPI clock to meet timing constraints
    // 40 us minimum interval between command byte and first data byte
    // 2 ms minimum interval between two data bytes for read-commands
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}



void slave_select(slave_t slave){
    //Set all slaves high for safety
    PORTB |= (1 << PB4);
    PORTD |= (1 << PD0);
    PORTD |= (1 << PD1);
    // Slave 1, set PB4 low (CAN)
    // Slave 2, set PD0 low (IO controller)
    // Slave 3, set PD1 low (OLED)
    switch (slave)
    {
    case CAN:
        PORTB &= ~(1 << PB4); // CAN
        break;
    case IO:
        PORTD &= ~(1 << PD0); 
        break;
    case OLED:
        PORTD &= ~(1 << PD1);
        break;
    case NONE: // No slave selected
        break;
    } 

}

void write_byte(char data, slave_t slave){
    //slave_select(slave); // Select the slave
    SPDR = data;
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    //slave_select(NONE); // Deselect all slaves
}

char read_byte(slave_t slave){
    //slave_select(slave); // Select the slave
    // Waiting to receive data
    while(!(SPSR & (1<<SPIF)));
    char data = SPDR;
    //slave_select(NONE); // Deselect all slaves
    return data;
}


void write_spi(char* buffer, slave_t slave, int bytes){
    for (int i = 0; i < bytes; i++) {
        write_byte(buffer[i], slave);
    }
}


void read_spi(char* buffer, slave_t slave, int bytes){
    for (int i = 0; i < bytes; i++) {
        buffer[i] = read_byte(slave);
    }
}



