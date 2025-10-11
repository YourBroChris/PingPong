#include <avr/io.h>

typedef enum {
    CAN = 0,
    IO = 1,
    OLED = 2,
    NONE = 3
} slave_t;


void init_spi(void);
void slave_select(slave_t slave);
void write_byte(char data, slave_t slave);
char read_byte(slave_t slave);
void write_spi(char* buffer, slave_t slave, int bytes);
void read_spi(char* buffer, slave_t slave, int bytes);
void command_data_set(int command);

