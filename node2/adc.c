#include "adc.h"

void adc_init(){
    //PMC->PMC_PCER0 |= (1 << ID_PIOA);   
    PMC->PMC_PCER1 |= (1 << ID_ADC - 32);

    // Disable PIO
    PIOA->PIO_PDR = PIO_PA16;

    // Set as input (disable as ouput)
    PIOA->PIO_ODR = PIO_PA16;

    ADC->ADC_MR |= ADC_MR_FREERUN_ON; // Set mode, no prescaler

    ADC->ADC_CHER |= ADC_CHER_CH7; // Select channel 7
    ADC->ADC_CR = ADC_CR_START; // Free running mode on, so dont need to trigger start conversion everytime

}

uint16_t adc_read(){
    while (!(ADC->ADC_ISR & ADC_ISR_EOC7)); // Wait until conversion is complete
    return ADC->ADC_CDR[7];
}