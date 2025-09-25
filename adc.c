#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "adc.h"


void adc_init (){

}

void pos_calibrate(){

}

uint8_t adc_read(uint8_t channel){
    uint8_t data = 0;
    // Activate read ADC signal PD3
    PORTD &= ~(1 << PD3);
    _delay_us(1);
    // Read on data bus port A
    data = PINA;
    PORTD |= (1 << PD3);
    return data;
} //volatile



pos_t pos_read(){
    pos_t joystick_pos;
    // Activate write
    PORTD &= ~(1 << PD5);
    _delay_us(1);
    PORTD |= (1 << dPD5);
    // Wait for ADC to finish reading analog signals
    while(!PIND2){}
    joystick_pos.x = adc_read();
    joystick_pos.y = adc_read();
    adc_read();
    adc_read();
    return joystick_pos;
}

void init_ADC_clk(){
    // Use Timer1 (16-bit) for flexibility
    // Output on OC1A (PD5) or OC1B (PD4) – recommended in course doc
    
    // Set pin as output
    DDRD |= (1 << PD4);  // OC1A pin
    
    // Set CTC mode, toggle OC1A on compare match
    TCCR1A = (1 << COM1B0);              // Toggle OC1A
    TCCR1B = (1 << WGM12) | (1 << CS10); // CTC mode, prescaler = 1
    
    // Set compare value for desired frequency
    // f_clk_out = f_CPU / (2 * (OCR1B + 1))
    OCR1B = 0; 
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