#include "pwm.h"
#include "sam.h"
#include <stdio.h>

void disable_write_protection(__uint8_t mode){
        PWM->PWM_WPCR = (
                        PWM_WPCR_WPRG0 | 
                        PWM_WPCR_WPRG1 |
                        PWM_WPCR_WPRG2 |
                        PWM_WPCR_WPRG3 |
                        PWM_WPCR_WPRG4 |
                        PWM_WPCR_WPRG5 |
                        PWM_WPCR_WPKEY(0x50574D)|
                        PWM_WPCR_WPCMD(mode) // Select enable or disable
                    ); 
}

void init_pwm(){

    // Enable peripheral clocks
    PMC->PMC_PCERn |= (1 << PIDn);

    // Disable PIO
    PIOC->PIO_PDR |= PIO_PDR_Pn;

    // AB Peripheral select
    PIOC->PIO_ABSR |= PIO_ABSR_Pn;

    //This register can only be written if the bits WPSWS0 and WPHWS0 are cleared in “PWM Write Protect Status Register” on page 1039.
    PWM->PWM_CLK / PWM->REG_PWM_CLK = ; //: PWM Clock register. Sets a prescaler, not required
    //This register can only be written if the bits WPSWS2 and WPHWS2 are cleared in “PWM Write Protect Status Register” on page 1039.
    PWM->PWM_CH_NUM[1].PWM_CMR / PWM->REG_PWM_CMRn = ; //: PWM Channel Mode Register
    PWM->PWM_CH_NUM[1].PWM_CPRD / PWM->REG_PWM_CPRDn = ; //: PWM Channel Period Registe
    PWM->PWM_CH_NUM[1].PWM_CDTY / PWM->REG_PWM_CDTYn = ; //: PWM Channel Duty Cycle Register
    PWM->PWM_ENA / PWM->PWM_ENA = 1; //: PsWM Enable Register, channel ID

}