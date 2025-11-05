#include "can.h"
#include "spi.h"


void init_can(){
    init_spi();
    reset_instruction();
    select_mode(MCP_CONFIG);
    uint8_t mode = read_instruction(MCP_CANSTAT) & 0xE0;
    
    while(mode != MODE_CONFIG){
        mode = read_instruction(MCP_CANSTAT) & 0xE0;
    }
    
	write_instruction(MCP_CNF1, 0x03); //00000011 SJW1, SJW0, BRP5, BRP4, BRP3, BRP2, BRP1, BRP0
	write_instruction(MCP_CNF2, 0xB1); //10110001 BTLMODE, SAM, PHSEG12, PHSEG11, PHSEG10, PRSEG2, PRSEG1, PRSEG0
	write_instruction(MCP_CNF3, 0x05); //00000101 SOF, WAKFIL, ___, ___, ___, PHSEG22, PHSEG21, PHSEG20

    // 	write_instruction(MCP_CNF1, 0b11001000); //SJW1, SJW0, BRP5, BRP4, BRP3, BRP2, BRP1, BRP0
	// write_instruction(MCP_CNF2, 0b11100100); //BTLMODE, SAM, PHSEG12, PHSEG11, PHSEG10, PRSEG2, PRSEG1, PRSEG0
	// write_instruction(MCP_CNF3, 0b00000100); //SOF, WAKFIL, ___, ___, ___, PHSEG22, PHSEG21, PHSEG20

    select_mode(MCP_NORMAL);
    uint8_t mode2 = read_instruction(MCP_CANSTAT) & 0xE0;
    while(mode2 != MODE_NORMAL){
        mode2 = read_instruction(MCP_CANSTAT) & 0xE0;
    }
    //return mode;
    //while((read_instruction(MCP_CANCTRL) << 5) != MODE_LOOPBACK);
}

            // uint32_t phase2:4;  // Phase 2 segment
            // uint32_t propag:4;  // Propagation time segment
            // uint32_t phase1:4;  // Phase 1 segment
            // uint32_t sjw:3;     // Synchronization jump width
            // uint32_t brp:8;     // Baud rate prescaler
            // uint32_t smp:8;     // Sampling mode


uint8_t read_instruction(uint8_t addr){
    uint8_t result;
    slave_select(CAN);
    write_byte(MCP_READ);      
    write_byte(addr);         
    result = read_byte();       
    slave_select(NONE);
    return result;
}


void write_instruction(uint8_t addr, uint8_t data){
    slave_select(CAN);
    write_byte(MCP_WRITE);
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
    write_byte(MCP_BITMOD);
    write_byte(addr);
    write_byte(mask);
    write_byte(data);
    slave_select(NONE);
}

void readstatus_instruction(){
    slave_select(CAN);
    write_byte(MCP_READ_STATUS);
    slave_select(NONE); // Kan holde CS nede lenger dersom man vil fortsette å lese status
}

void reset_instruction(){
    slave_select(CAN);
    write_byte(MCP_RESET);
    slave_select(NONE);
}

void select_mode(uint8_t configuration_mode){
    configuration_mode = configuration_mode << 5;
    bitmodify_instruction(MCP_CANCTRL, configuration_mode, 0b11100000);
}

void transmit_can(can_message* msg, uint8_t buffer_index) {
    uint8_t base_addr;

    // Select the correct transmit buffer base address
    switch (buffer_index) {
        case 0: base_addr = MCP_TXB0SIDH; break;
        case 1: base_addr = MCP_TXB1SIDH; break;
        case 2: base_addr = MCP_TXB2SIDH; break;
        default: return; // Invalid buffer index
    }

    // Load 11-bit standard ID into SIDH and SIDL
    write_instruction(base_addr, (msg->id >> 3) & 0xFF);         // SIDH
    write_instruction(base_addr + 1, (msg->id << 5) & 0xE0);     // SIDL

    // Clear extended ID registers
    write_instruction(base_addr + 2, 0x00);                      // EID8
    write_instruction(base_addr + 3, 0x00);                      // EID0

    // Set DLC (Data Length Code)
    write_instruction(base_addr + 4, msg->length & 0x0F);        // DLC

    // Load data bytes
    for (uint8_t i = 0; i < msg->length; i++) {
        write_instruction(base_addr + 5 + i, msg->data[i]);
    }

    // Request to send using the appropriate buffer
    rts_instruction(1 << buffer_index); // 0x01 for TXB0, 0x02 for TXB1, 0x04 for TXB2
}

void receive_can(can_message* msg){
    // Read ID from RXB0SIDH and RXB0SIDL
    uint8_t sid_high = read_instruction(MCP_RXB0SIDH);
    uint8_t sid_low  = read_instruction(MCP_RXB0SIDL);

    // Reconstruct 11-bit standard ID
    msg->id = (sid_high << 3) | (sid_low >> 5);

    // Read DLC (data length code)
    msg->length = read_instruction(MCP_RXB0DLC) & 0x0F;

    // Read data bytes
    for (uint8_t i = 0; i < msg->length; i++) {
        msg->data[i] = read_instruction(MCP_RXB0D0 + i);
    }

    // Optionally clear RXB0 interrupt flag
    bitmodify_instruction(MCP_CANINTF, 0x00, MCP_RX0IF);
}