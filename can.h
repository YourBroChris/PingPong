#include <avr/io.h>

void init_can();
void issue_instruction(uint8_t can_instruction, uint8_t addr, uint8_t mask, uint8_t txb_bits, uint8_t mask);
void read_instruction(uint8_t addr);
void write_instruction(uint8_t addr, uint8_t data);
void rts_instruction(uint8_t txb_bits);
void bitmodify_instruction(uint8_t addr, uint8_t data, uint8_t mask);
void readstatus_instruction();
void reset_instruction();


typedef struct {
    unsigned short id;
    unsigned char length;
    char data[8];
} can_message

typedef enum {
    RESET = 0b11000000,
    READ = 0b00000011,
    WRITE = 0b00000010,
    RTS = 0b10000000, // 10000xxx
    BITMODIFY = 0b00000101,
    READSTATUS = 0b10100000
} can_instruction