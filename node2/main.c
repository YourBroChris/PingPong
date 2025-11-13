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

void updateScore(uint16_t adc_value, uint8_t* in_goal, uint8_t* score);
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


float map_joystick_to_speed(uint8_t joystick)
{
    const int center = 128; // Joystick center i actually 160, it goes from 55 to 250 approximately, not sure if that affects it
    const float max_speed = 1000.0f; // encoder counts per second
    return (joystick - center) * (max_speed / 128.0f);
}


int main()
{
    /*
    SystemInit();
    uart_init(F_CPU, 9600);
    init_pwm();
    while(1){
        printf("CLK PRESCALER: %d \r\n", PWM->PWM_CLK);
    }
    */
    

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
    //init_solenoid();
    //goleft();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    struct enc_boundaries boundaries = calibrating_encoder();
    printf("Calibration done. Left: %d Right: %d\r\n", boundaries.left_boundary, boundaries.right_boundary);

    static int32_t last_encoder = 0;
    int32_t current_encoder = encoder_read();
    float measured_speed = (current_encoder - last_encoder) / pid.dt;
    last_encoder = current_encoder;

    int target_speed = 0;
    while (1)
    {
        can_tx(msgTx);
        
        
        if(can_rx(&msgRx)){
            printf("CAN message: id=%d len=%d Joystick x: %d y: %d, Button: %d\r\n", msgRx.id, msgRx.length, msgRx.byte[0], msgRx.byte[1], msgRx.byte[2]);
            motorchange(msgRx.byte[0]);
            target_speed = map_joystick_to_speed(msgRx.byte[0]);
            current_encoder = encoder_read();
            if((msgRx.byte[2] % 2 ) == 1){ // Checks if button is pressed
                activate_solenoid(0);
            }
            else{
                activate_solenoid(1);
            }
            
            printf("Encoder Value: %d\r\n", current_encoder);
            //change_pwm(msgRx.byte[1]);
            //uint16_t adc_value = adc_read();
            //printf("ADC Value: %d\r\n", adc_value);
            // updateScore(adc_value, &in_goal, &score);
        }

        // if (pid_flag)
        //     {
        //         last_encoder = current_encoder;
        //         current_encoder = encoder_read();
        //         measured_speed = (current_encoder - last_encoder) / pid.dt;
        //         float control_signal = pid_compute(&pid, (float)target_speed, measured_speed);
        //         // Use control_signal to adjust motor speed via PWM
        //         printf("Setpoint: %d Measured Speed: %.2f Control Signal: %.2f\r\n", target_speed, measured_speed, control_signal);
        //         pid_flag = 0;
        //     }
        
    }
    
}

// Ikke testet enda
void updateScore(uint16_t adc_value, uint8_t* in_goal, uint8_t* score){
    if (!*in_goal)
    {
        if (adc_value < 250){
            (*score)++;
            *in_goal = 1;
        }
        else{
            *in_goal = 0;
        }
    }
    else if (*in_goal){
        if (adc_value > 250){
            *in_goal = 0;
        }
        else{
            *in_goal = 1;
        }
    }
}