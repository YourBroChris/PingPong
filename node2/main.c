#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"
#include "adc.h"
#include "motordriver.h"
#include "encoder.h"
#include "solenoid.h"
#include "pid.h"

void updateScore(uint16_t adc_value, uint8_t* in_goal);
/*write_instruction
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

#define F_CPU 84000000

volatile uint8_t pid_flag = 0;
static uint8_t counter = 0;
static int target_position = 0;

int main()
{

    
    CanInit canTiming = {
        .phase2 = 5,
        .propag = 1,
        .phase1 = 6,
        .sjw    = 2,
        .brp    = 20,
        .smp    = 0
    };

    typedef struct CanMsg CanMsg;

    CanMsg msgRx;

    CanMsg msgTx = {
        .id = 10,
        .length = 1,
        .byte = {0x03}
    };

    struct PIDController pid;
    

    SystemInit();
    uart_init(F_CPU, 9600);
    can_init(canTiming, 0); // 0 = no interrupt
    init_pwm();
    adc_init();
    motordriver_init();
    encoder_init();
    init_solenoid();
    pid_init(&pid, 1.0f, 0.0f, 0.1f, 0.01f); // Kp, Ki, Kd, dt
    timer0_init();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    struct enc_boundaries boundaries = calibrating_encoder();
    printf("Calibration done. Left: %d Right: %d\r\n", boundaries.left_boundary, boundaries.right_boundary);

    int32_t current_encoder = encoder_read();
    uint8_t in_goal = 0;

    while (1)
    {

        
        
        
        if(can_rx(&msgRx)){
            //printf("CAN message: id=%d len=%d Joystick x: %d y: %d, Button: %d\r\n", msgRx.id, msgRx.length, msgRx.byte[0], msgRx.byte[1], msgRx.byte[2]);
            //motorDriveVelocity(msgRx.byte[0]);
            
            servochange(msgRx.byte[1]);
            
            if((msgRx.byte[2] % 2 ) == 1){ // Checks if button is pressed
                //printf("Solenoid activated\r\n");
                activate_solenoid();
            }
       
            //printf("Encoder Value: %d\r\n", current_encoder);

        }
        uint16_t adc_value = adc_read();
        updateScore(adc_value, &in_goal);
        
        msgTx.byte[0] = in_goal;

        if(in_goal == 1){
            printf("ADC Value: %d\r\n", adc_value);
            can_tx(msgTx);
        }
        if (pid_flag){
            target_position = joystickToMotorPosition(msgRx.byte[0], &boundaries);
            current_encoder = encoder_read();
            motorDrivePosition(target_position, current_encoder, &boundaries, &pid);    
            }
        
    }
    
}


void updateScore(uint16_t adc_value, uint8_t* in_goal)
{

    const uint8_t required_samples = 10;   // tune this
    const uint16_t threshold = 2000;

    if (adc_value < threshold) {
        if (counter < required_samples)
            counter++;

        if (counter >= required_samples)
            *in_goal = 1;
    }
    else {
        counter = 0;        // reset when signal goes back high
        *in_goal = 0;
    }
}
