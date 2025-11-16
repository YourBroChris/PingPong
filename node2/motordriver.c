#include "motordriver.h"
#include "pwm.h"
#include "encoder.h"
#include "pid.h"
#include "sam/sam3x/include/sam.h"
#include <math.h>

void motordriver_init(){
    disable_write_protection(DIS);
    if ( (PWM->PWM_WPSR & WP_SW_MASK) != 0 || (PWM->PWM_WPSR & WP_HW_MASK) != 0 ) {
        printf("PWM WP not disabled correctly\r\n");
    }
 // Enable peripheral clocks
    PMC->PMC_PCER0 |= (1 << ID_PIOB);               // Enable PIOB
    PMC->PMC_PCER0 |= (1 << ID_PIOC);               // Enable PIOB
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));       


    // Disable PIO
    PIOB->PIO_PDR |= PIO_PB12;
    PIOC->PIO_PER |= PIO_PC23;
    PIOC->PIO_PUDR |= PIO_PC23;

    PIOC->PIO_OER |= PIO_PC23; // Set as output

    PIOC->PIO_CODR = PIO_PC23; // Set LOW initially

    // AB Peripheral select
    PIOB->PIO_ABSR |= PIO_PB12;  // Select peripheral B
    PIOB->PIO_PUDR |= PIO_PB12; // Disable pull-up


    PWM->PWM_CLK = PWM_CLK_PREA(3) | PWM_CLK_DIVA(8); //: PWM Clock register. Sets a prescaler, not required

    PWM->PWM_CH_NUM[0].PWM_CMR  = PWM_CMR_CPOL | PWM_CMR_CPRE_CLKA; //: PWM Channel Mode Register
    PWM->PWM_CH_NUM[0].PWM_CPRD = 30; 
    PWM->PWM_CH_NUM[0].PWM_CDTY = 12; 

    PWM->PWM_ENA |= (1 << 0); //PWM_ENA_CHID0; //: PsWM Enable Register, channel ID, try (1 << 1) if it doesnt work
}

void goleft(){
    PIOC->PIO_SODR = PIO_PC23; // LEFT
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = 12;
}


void goright(){
    PIOC->PIO_CODR = PIO_PC23; // RIGHT
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = 12;
}

void motorstop(){
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = 0;
}

int joystickToMotorPosition(uint8_t x, struct enc_boundaries *boundaries){
    // Map joystick position to encoder boundaries
    int position = boundaries->left_boundary + (((int)x - 0) * (boundaries->right_boundary - boundaries->left_boundary)) / (255 - 0);
    // c + (x - a) * (d - c) / (b - a)
    // a = 0, b = 255
    return position;
}

void motorDriveVelocity(uint8_t rawjoystickpos){
    static uint8_t prevJoyStickPos = 160;
    const uint8_t deadband = 5; // Change in joystick needed to update the servo
    
    if (rawjoystickpos > 160){
        //printf("Setting direction to RIGHT\r\n");
        PIOC->PIO_CODR = PIO_PC23; // RIGHT
    }
    else{
        //printf("Setting direction to LEFT\r\n");
        PIOC->PIO_SODR = PIO_PC23; // LEFT
    }

    int pulse_width;
    if(abs((int)rawjoystickpos - (int)prevJoyStickPos) > deadband) {
        pulse_width = abs((int)rawjoystickpos - 160)/10;
        if((pulse_width <= 10)){
            //printf("Setting motor pulse width to: %lu\r\n", pulse_width);
            PWM->PWM_CH_NUM[0].PWM_CDTYUPD = pulse_width*1.7;
            prevJoyStickPos = rawjoystickpos;
        }
        else{
            //printf("Pulse width out of range: %lu\r\n", pulse_width);
        }
    }   
}


void motorDrivePosition(int targetPosition, int currentPosition, struct enc_boundaries* boundaries, struct PIDController* pid){
    int error = targetPosition - currentPosition;

    if (error == 0){
        PWM->PWM_CH_NUM[0].PWM_CDTYUPD = 0;
        return;
    }

    float pid_output = pid_compute(pid, (float)targetPosition, (float)currentPosition);

    if (pid_output > 0){
        PIOC->PIO_CODR = PIO_PC23; // RIGHT
    }
    else{
        PIOC->PIO_SODR = PIO_PC23; // LEFT
    }

    float duty = fabsf(pid_output);

    int maxDuty = PWM->PWM_CH_NUM[0].PWM_CPRD;
    if (duty > maxDuty * 0.30f) duty = maxDuty * 0.30f;  // example limit

    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = (int)duty;
}