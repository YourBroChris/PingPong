#include <avr/io.h>
#include "mcp2515.h"

typedef struct {
    unsigned short id;
    unsigned char length;
    char data[8];
} can_message;

uint8_t init_can();
uint8_t read_instruction(uint8_t addr);
void write_instruction(uint8_t addr, uint8_t data);
void rts_instruction(uint8_t txb_bits);
void bitmodify_instruction(uint8_t addr, uint8_t data, uint8_t mask);
void readstatus_instruction();
void reset_instruction();
void select_mode(uint8_t configuration_mode);
void transmit_can(can_message* msg, uint8_t buffer_index);
