#include "adc.h"

void adc_init (){

}
uint8_t adc_read(uint8_t channel){
    return 0;
} //volatile
void pos_calibrate(){

}

pos_t pos_read(){
    return (pos_t){0,0};
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