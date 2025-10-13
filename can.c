#include "can.h"
#include "spi.h"


void init_can(){
    init_spi();
    reset_instruction();
    select_mode(loopback);
    while((read_instruction(CANCTRL) << 5) != loopback);
    
}

void read_instruction(uint8_t addr){
    slave_select(CAN);
    write_byte(0b00000011);
    write_byte(addr);
    slave_select(NONE);
}

void write_instruction(uint8_t addr, uint8_t data){
    slave_select(CAN);
    write_byte(0b00000010);
    write_byte(addr);
    write_byte(data);
    slave_select(NONE);
}

void rts_instruction(uint8_t txb_bits){
    slave_select(CAN);
    uint8_t rts_instr = (0b10000000 | txb_bits);
    write_byte(rts_instr);
    slave_select(NONE);
}

void bitmodify_instruction(uint8_t addr, uint8_t data, uint8_t mask){
    slave_select(CAN);
    write_byte(0b00000101);
    write_byte(addr);
    write_byte(mask);
    write_byte(data);
    slave_select(NONE);
}

void readstatus_instruction(){
    slave_select(CAN);
    write_byte(0b10100000);
    slave_select(NONE); // Kan holde CS nede lenger dersom man vil fortsette å lese status
}

void reset_instruction(){
    slave_select(CAN);
    write_byte(0b11000000);
    slave_select(NONE);
}

void select_mode(uint8_t configuration_mode){
    configuration_mode = configuration_mode << 5;
    bitmodify_instruction(CANCTRL, configuration_mode, 0b11100000);
}

void transmit_can(can_message* msg, uint8_t buffer_index){
    write_byte()

    rts_instruction(buffer_index):
}
