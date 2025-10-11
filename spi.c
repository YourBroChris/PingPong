#include "spi.h"
#include <avr/io.h>

void init_spi(void){
    // Set MOSI, SCK as Output
    DDRB = (1 << PB3) | (1 << PB5); // PB3 is MOSI, PB5 is SCK
    DDRD = (1 << PD0) | (1 << PD1) | (1 << PD2); // PD0 and PD1 as SS for slaves 2 and 3, PD2 for D/C selection
    // Enable SPI, Set as Master
    // Prescaler: Fosc/16
    // Limit SPI clock to meet timing constraints
    // 40 us minimum interval between command byte and first data byte
    // 2 ms minimum interval between two data bytes for read-commands
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    
}

void slave_select(int slave){
    //Set all slaves high for safety
    PORTB |= (1 << PB4);
    PORTD |= (1 << PD0);
    PORTD |= (1 << PD1);
    // Slave 1, set PB4 low (OLED)
    // Slave 2, set PD0 low (IO controller)
    // Slave 3, set PD1 low (CAN)
    switch (slave)
    {
    case 0:
        PORTB &= ~(1 << PB4);
        break;
    case 1:
        PORTD &= ~(1 << PD0);
        break;
    case 2:
        PORTD &= ~(1 << PD1);
        break;
    case 3: // No slave selected
        break;
    } 

}

void write_byte(char data, int slave){
    slave_select(slave); // Select the slave
    SPDR = data;
    // Wait for transmission complete 
    while(!(SPSR & (1<<SPIF)));
    slave_select(3); // Deselect all slaves
}

char read_byte(int slave){
    slave_select(slave); // Select the slave
    // Waiting to receive data
    while(!(SPSR & (1<<SPIF)));
    char data = SPDR;
    slave_select(3); // Deselect all slaves
    return data;
}

void write_spi(char* buffer, int slave, int bytes){
    for (int i = 0; i < bytes; i++) {
        write_byte(buffer[i], slave);
    }
}


void read_spi(char* buffer, int slave, int bytes){
    for (int i = 0; i < bytes; i++) {
        buffer[i] = read_byte(slave);
    }
}

