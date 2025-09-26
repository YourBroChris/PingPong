#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "adc.h"
#include <stdio.h>
#include <stdlib.h> 

#define t_CONV = (9 * 4 * 2) / (4915200/2)
uint16_t ext_adc_size = 0x0400;

void adc_init (){

}

void pos_calibrate(){

}

uint8_t adc_read(volatile uint8_t *ext_adc){
    printf("Reading ADC channel.\r\n");
    uint8_t data;
    data = ext_adc[0];
    return data;
} //volatile

pos_t convert_pos(uint8_t rawpos_x, uint8_t rawpos_y){
    uint8_t idle_x = 160, max_x = 247, min_x = 64;
    uint8_t idle_y = 158, max_y = 248, min_y = 64;

    if (rawpos_x < min_x) rawpos_x = min_x;
    if (rawpos_x > max_x) rawpos_x = max_x;
    if (rawpos_y < min_y) rawpos_y = min_y;
    if (rawpos_y > max_y) rawpos_y = max_y;

    pos_t result;
    if (rawpos_x >= idle_x) {
        result.x = (rawpos_x - idle_x) * 100 / (max_x - idle_x);
    } else {
        result.x = (rawpos_x - idle_x) * 100 / (idle_x - min_x);
    }

    if (rawpos_y >= idle_y) {
        result.y = (rawpos_y - idle_y) * 100 / (max_y - idle_y);
    } else {
        result.y = (rawpos_y - idle_y) * 100 / (idle_y - min_y);
    }


    return result;
}


pos_t pos_read(){
    pos_t joystick_pos;
    raw_pos_t rawjoystick_pos;
    volatile uint8_t *ext_adc = (uint8_t *) 0x1000;
    printf("Starting joystick position read.\r\n");
    // Activate write
    ext_adc[0] = 0x00;
    _delay_us(100);
    printf("Test\r\n");
    // Wait for ADC to finish reading analog signals
    //_delay_us(100);
    while(!(PIND & (1 << PD2))); // Wait while Busy
    rawjoystick_pos.y = adc_read(ext_adc);
    //_delay_us(100);
    rawjoystick_pos.x = adc_read(ext_adc);
    //_delay_us(100);
    adc_read(ext_adc);
    //_delay_us(100);
    adc_read(ext_adc);
    //printf("Joystick position before conversion, X:%d\t  Y:%d\r\n", rawjoystick_pos.x, rawjoystick_pos.y);
    joystick_pos = convert_pos(rawjoystick_pos.x, rawjoystick_pos.y);
    return joystick_pos;
}

void init_ADC_clk(){
    // Set pin as output
    DDRD |= (1 << PD5);  // OC1A pin
    
    // Set CTC mode, toggle OC1A on compare match
    TCCR1A = (1 << COM1A0);              // Toggle OC1A
    TCCR1B = (1 << WGM12) | (1 << CS10); // CTC mode, prescaler = 1
    
    // Set compare value for desired frequency
    // f_clk_out = f_CPU / (2 * (OCR1A + 1))
    OCR1A = 0; 
}

void ADC_test(void){
    printf("Starting ADC test...\r\n");
    volatile uint8_t *ext_adc = (uint8_t *) 0x1000;

    uint16_t seed = rand();

    srand(seed);
    for(uint16_t i = 0; i < ext_adc_size; i++){
        uint8_t some_value = rand();
        ext_adc[i] = some_value;
    }
    printf("ADC test completed\r\n\n");
}
