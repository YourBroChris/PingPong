#include <avr/io.h>


void init_spi(void);
void slave_select(int slave);
void write_byte(char data, int slave);
char read_byte(int slave);
void write_spi(char* buffer, int slave, int bytes);
void read_spi(char* buffer, int slave, int bytes);
void command_data_set(int command);

