#include "solenoid.h"

void init_solenoid(){
    PMC->PMC_PCER0 = (1 << ID_PIOB); // Enable the clock for PIOB (Port B)

    // Step 2: Configure PB13 as an output
    PIOB->PIO_PER = PIO_PB25;        // Enable control of PB25 (set PIO Enable Register)
    PIOB->PIO_OER = PIO_PB25;        // Set PB25 as output (set Output Enable Register)
}


void activate_solenoid(uint8_t flag){
   if (!flag){
        //Set PB25 high
        PIOB->PIO_SODR = PIO_PB25;      // Set PB25 high (Set Output Data Register)
   }
    else{
        //Set PB25 low
        PIOB->PIO_CODR = PIO_PB25;      // Set PB25 low (Clear Output Data Register)
    }
};