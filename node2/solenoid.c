#include "solenoid.h"


void init_solenoid(){
    PMC->PMC_PCER0 = (1 << ID_PIOB); // Enable the clock for PIOB (Port B)

    // Step 2: Configure PB13 as an output
    PIOB->PIO_PER = PIO_PB25;        // Enable control of PB25 (set PIO Enable Register)
    PIOB->PIO_OER = PIO_PB25;        // Set PB25 as output (set Output Enable Register)
    PIOB->PIO_SODR = PIO_PB25;
}


void activate_solenoid(){
    PIOB->PIO_CODR = PIO_PB25;
    delay_ms(100);
    PIOB->PIO_SODR = PIO_PB25; 
};