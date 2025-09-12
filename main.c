#include "usart.h"
#include <avr/io.h>
#include <stdio.h>
#include "AVRinterrupts.h"

volatile int ReceiveFlag = 0;        // <-- define here


int main()
{
    init_usart(MYUBBR);
    init_interrupts();
    /*
    while(1){
        test_transmit();
    }
    */
    //test_receive();
    
    while(1){
        //latch_test();
        SRAM_test();
        if (ReceiveFlag)
        {
            //char received_char = receive_usart(received_char);
            printf("Received\n");
            ReceiveFlag = 0;
        }
        
        //printf("Test\n");
    }
    return 0;
}

/*S
void test_transmit(){
        transmit_usart('a');
        transmit_usart('b');
        transmit_usart('c');
    return;
}

void test_receive(){
        while(1){
        unsigned char received = receive_usart();
        if(received == 'a') {
            transmit_usart('b');
        }
    }
    return;
}
*/

void latch_test(){
    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    const int ext_ram_size = 0x800; // Size of the SRAM in bytes
    printf("Latch test starting...\n");
    // Set latch enabled PORT E1
    PORTE |= (1 << PE1);
    // Enable PORTA and PORTC as output
    DDRA = 0xFF;
    // Write to external SRAM
    for (int i = 0; i < ext_ram_size; i++) {
        ext_ram[i] = i % 256;
    }
    
    
    //PORTA = 0b00000000;
    // Set latch disabled
    PORTE &= ~(1 << PE1);
}


#include <stdlib.h>
    void SRAM_test(void)
    {
		volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
        uint16_t ext_ram_size = 0x800;
        uint16_t write_errors = 0;
        uint16_t retrieval_errors = 0;
        printf("Starting SRAM test...\n");
        // rand() stores some internal state, so calling this function in a loop will
        // yield different seeds each time (unless srand() is called before this function)
        uint16_t seed = rand();
        // Write phase: Immediately check that the correct value was stored
        srand(seed);
        for (uint16_t i = 0; i < ext_ram_size; i++) {
            uint8_t some_value = rand();
            ext_ram[i] = some_value;
            uint8_t retreived_value = ext_ram[i];
            if (retreived_value != some_value) {
                printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
                write_errors++;
            }
        }
        // Retrieval phase: Check that no values were changed during or after the write phase
        srand(seed);
        // reset the PRNG to the state it had before the write phase
        for (uint16_t i = 0; i < ext_ram_size; i++) {
            uint8_t some_value = rand();
            uint8_t retreived_value = ext_ram[i];
            if (retreived_value != some_value) {
                printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
                retrieval_errors++;
            }
        }
        printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}