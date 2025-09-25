#include "usart.h"
#include <avr/io.h>
#include <stdio.h>
#include "AVRinterrupts.h"
#include "sram.h"
#include "adc.h"

volatile int ReceiveFlag = 0;        // <-- define here


int main()
{
    init_usart(MYUBBR);
    init_xmem();
    init_ADC_clk();
    //init_interrupts();
    /*
    while(1){
        test_transmit();
    }
    */
    //test_receive();
    
    while(1){
        //ADC_test();
        //SRAM_test();
        /*
        if (ReceiveFlag)
        {
            //char received_char = receive_usart(received_char);
            printf("Received\n");
            ReceiveFlag = 0;
        }
        
        //printf("Test\n");
        */
    }
    return 0;
}




void ADC_test(void){
    
    volatile char *ext_adc = (char *) 0x1000;

    uint16_t ext_adc_size = 0x0400;
    printf("Starting ADC test...\r\n");

    uint16_t seed = rand();

    srand(seed);
    for(uint16_t i = 0; i < ext_adc_size; i++){
        uint8_t some_value = rand();
        ext_adc[i] = some_value;
    }
    printf("ADC test completed\r\n\n");
}

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

/*
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