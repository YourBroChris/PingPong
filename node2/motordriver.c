#include "motordriver.h"

void motordriver_init(){
 // Enable peripheral clocks
    PMC->PMC_PCER0 |= (1 << ID_PIOB);               // Enable PIOB
    PMC->PMC_PCER0 |= (1 << ID_PIOC);               // Enable PIOB
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));         // Enable PWM


    // Disable PIO
    PIOB->PIO_PDR |= PIO_PB12;
    PIOC->PIO_PER |= PIO_PC23;
    PIOC->PIO_PUDR |= PIO_PC23;

    PIOC->PIO_OER |= PIO_PC23; // Set as output

    PIOC->PIO_CODR = PIO_PC23; // Set LOW initially

    // AB Peripheral select
    PIOB->PIO_ABSR |= PIO_PB12;  // Select peripheral B
    PIOB->PIO_PUDR |= PIO_PB12; // Disable pull-up

    //This register can only be written if the bits WPSWS0 and WPHWS0 are cleared in “PWM Write Protect Status Register” on page 1039.
    PWM->PWM_CLK = PWM_CLK_PREA(3) | PWM_CLK_DIVA(8); //: PWM Clock register. Sets a prescaler, not required
    //This register can only be written if the bits WPSWS2 and WPHWS2 are cleared in “PWM Write Protect Status Register” on page 1039.
    PWM->PWM_CH_NUM[0].PWM_CMR  = PWM_CMR_CPOL | PWM_CMR_CPRE_CLKA; //: PWM Channel Mode Register
    PWM->PWM_CH_NUM[0].PWM_CPRD = 2620; //: PWM Channel Period Register, 20ms
    PWM->PWM_CH_NUM[0].PWM_CDTY = motor_MIN; //: PWM Channel Duty Cycle Register, 2ms

    PWM->PWM_ENA |= PWM_ENA_CHID0; //: PsWM Enable Register, channel ID, try (1 << 1) if it doesnt work
}

void goright(){
    PIOC->PIO_SODR = PIO_PC23; // RIGHT
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = 10;
}

void motorchange(uint8_t rawjoystickpos){
    static uint8_t prevJoyStickPos = 160;
    const uint8_t deadband = 5; // Change in joystick needed to update the servo
    
    if (rawjoystickpos < 160){
        printf("Setting direction to LEFT\r\n");
        PIOC->PIO_CODR = PIO_PC23; // LEFT
    }
    else{
        printf("Setting direction to RIGHT\r\n");
        PIOC->PIO_SODR = PIO_PC23; // RIGHT
    }

    int pulse_width;
    if(abs((int)rawjoystickpos - (int)prevJoyStickPos) > deadband) {

        pulse_width = motor_MIN + ((int)(rawjoystickpos) * (motor_MAX - motor_MIN)) / 255;
        if((pulse_width > motor_MIN) && (pulse_width < motor_MAX)){
            printf("Setting motor pulse width to: %lu\r\n", pulse_width);
            PWM->PWM_CH_NUM[0].PWM_CDTYUPD = pulse_width;
            prevJoyStickPos = rawjoystickpos;
        }
        else{
            printf("Pulse width out of range: %lu\r\n", pulse_width);
        }
    }   
}