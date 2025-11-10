#include "pwm.h"
#include "sam.h"
#include <stdio.h>

#define WP_SW_MASK (PWM_WPSR_WPSWS0 | PWM_WPSR_WPSWS1 | PWM_WPSR_WPSWS2 | \
                    PWM_WPSR_WPSWS3 | PWM_WPSR_WPSWS4 | PWM_WPSR_WPSWS5)
#define WP_HW_MASK (PWM_WPSR_WPHWS0 | PWM_WPSR_WPHWS1 | PWM_WPSR_WPHWS2 | \
                    PWM_WPSR_WPHWS3 | PWM_WPSR_WPHWS4 | PWM_WPSR_WPHWS5)


void disable_write_protection(prt_cmd cmd){
        PWM->PWM_WPCR = (
                        PWM_WPCR_WPRG0 | 
                        PWM_WPCR_WPRG1 |
                        PWM_WPCR_WPRG2 |
                        PWM_WPCR_WPRG3 |
                        PWM_WPCR_WPRG4 |
                        PWM_WPCR_WPRG5 |
                        PWM_WPCR_WPKEY(0x50574D)|
                        PWM_WPCR_WPCMD(cmd) // Select enable or disable
                    ); 
}

void init_pwm(){
    disable_write_protection(DISABLE);
    if ( (PWM->PWM_WPSR & WP_SW_MASK) != 0 || (PWM->PWM_WPSR & WP_HW_MASK) != 0 ) {
        printf("PWM WP not disabled correctly\r\n");
    }
    // Enable peripheral clocks
    PMC->PMC_PCERn |= (1 << PIDn);

    // Disable PIO
    PIOC->PIO_PDR |= PIO_PDR_Pn;

    // AB Peripheral select
    PIOC->PIO_ABSR |= PIO_ABSR_Pn;
    PIOC->PIO_MDDR = PIO_MDDR_Pn;

    //This register can only be written if the bits WPSWS0 and WPHWS0 are cleared in “PWM Write Protect Status Register” on page 1039.
    PWM->PWM_CLK / PWM->REG_PWM_CLK = ; //: PWM Clock register. Sets a prescaler, not required
    //This register can only be written if the bits WPSWS2 and WPHWS2 are cleared in “PWM Write Protect Status Register” on page 1039.
    PWM->PWM_CH_NUM[1].PWM_CMR  = ; //: PWM Channel Mode Register
    PWM->PWM_CH_NUM[1].PWM_CPRD = ; //: PWM Channel Period Registe
    PWM->PWM_CH_NUM[1].PWM_CDTY = ; //: PWM Channel Duty Cycle Register
    PWM->PWM_ENA = 1; //: PsWM Enable Register, channel ID


    disable_write_protection(ENABLE);
    if ( (PWM->PWM_WPSR & WP_SW_MASK) != WP_SW_MASK || (PWM->PWM_WPSR & WP_HW_MASK) != WP_HW_MASK )  {
        printf("PWM WP not enabled correctly\r\n");
    }
}