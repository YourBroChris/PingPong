#include <spi.h>
#include <avr/io.h>

void spi_init(void){
    // Set MOSI, SCK as Output
    DDRB = (1 << PB3) | (1 << PB5) | (1 << PB2); // PB2 is SS, PB3 is MOSI, PB5 is SCK
    // Enable SPI, Set as Master
    // Prescaler: Fosc/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void slave_select(int slave){
    //Set all slaves high for safety
    PORTE |= (1 << PE4);
    PORTD |= (1 << PD0);
    PORTD |= (1 << PD1);
    // Slave 1, set SS (PB4) low
    // Slave 2, set PD0 low
    // Slave 3, set PD1 low
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
    default:
        break;
    } 

}

void write_byte(char data, int slave){
    slave_select(slave);
    // Start transmission by writing data to SPI data register

    // Wait for transmission complete

    // Return received data from SPI data register
}

char read_byte(int slave){
    slave_select(slave);
    // Start transmission by writing dummy data to SPI data register

    // Wait for transmission complete

    // Return received data from SPI data register
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