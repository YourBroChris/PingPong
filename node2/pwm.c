#include "pwm.h"
#include "sam/sam3x/include/sam.h"
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
    disable_write_protection(DIS);
    if ( (PWM->PWM_WPSR & WP_SW_MASK) != 0 || (PWM->PWM_WPSR & WP_HW_MASK) != 0 ) {
        printf("PWM WP not disabled correctly\r\n");
    }
    // Enable peripheral clocks
    //PMC->PMC_PCERn |= (1 << PIDn);
    PMC->PMC_PCER0 |= (1 << ID_PIOB);               // Enable PIOB
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));         // Enable PWM


    // Disable PIO
    PIOB->PIO_PDR |= PIO_PB13;

    // AB Peripheral select
    PIOB->PIO_ABSR |= PIO_PB13;  // Select peripheral B
    //PIOB->PIO_MDDR = PIO_PB13;  // Disable multi-drive
     PIOB->PIO_PUDR |= PIO_PB13; // Disable pull-up

    //This register can only be written if the bits WPSWS0 and WPHWS0 are cleared in “PWM Write Protect Status Register” on page 1039.
    PWM->PWM_CLK = PWM_CLK_PREA(3) | PWM_CLK_DIVA(8); //: PWM Clock register. Sets a prescaler, not required
    //This register can only be written if the bits WPSWS2 and WPHWS2 are cleared in “PWM Write Protect Status Register” on page 1039.
    PWM->PWM_CH_NUM[1].PWM_CMR  = PWM_CMR_CPOL | PWM_CMR_CPRE_CLKA; //: PWM Channel Mode Register
    PWM->PWM_CH_NUM[1].PWM_CPRD = 26250; //: PWM Channel Period Register, 20ms
    PWM->PWM_CH_NUM[1].PWM_CDTY = 1181; //: PWM Channel Duty Cycle Register, 2ms
    // (0.9ms-2.1ms) -> (1181-2756) PWM_CDTY values
    PWM->PWM_ENA = PWM_ENA_CHID1; //: PsWM Enable Register, channel ID, try (1 << 1) if it doesnt work


    disable_write_protection(ENABLE);
    if ( (PWM->PWM_WPSR & WP_SW_MASK) != WP_SW_MASK)  {
        printf("PWM WP not enabled correctly\r\n");
    }
}

int change_pwm(uint8_t joystickpos){
    // I assume you change PWD registers to change width
    return 1181 + ((uint32_t)joystickpos * (2756 - 1181)) / 255;
}