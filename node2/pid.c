#include "pid.h"
#include "sam/sam3x/include/sam.h"

#define ENC_SAMPLE_FREQ 100
extern volatile uint8_t pid_flag;

void pid_init(struct PIDController* pid, float kp, float ki, float kd, float dt){
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->dt = dt;
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
}

float pid_compute(struct PIDController* pid, float setpoint, float measured){
    float error = setpoint - measured;
    pid->integral += error * pid->dt;
    float derivative = (error - pid->prev_error) / pid->dt;
    pid->prev_error = error;
    return (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
}

void TC0_Handler(void){
    // Clear the interrupt flag
    uint32_t status = TC0->TC_CHANNEL[0].TC_SR;
    pid_flag = 1;
}


void timer0_init(void){
    PMC->PMC_PCER0 |= (1 << ID_TC0); // Enable clock for TC0

    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | // MCK/128
                                TC_CMR_WAVE |                // Waveform mode
                                TC_CMR_WAVSEL_UP_RC;        // Count up to RC

    uint32_t rc = (84000000 / 128) / ENC_SAMPLE_FREQ; // For 1ms interrupt
    TC0->TC_CHANNEL[0].TC_RC = rc;
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; // Enable interrupt on RC compare
    NVIC_EnableIRQ(TC0_IRQn); // Enable TC0 interrupt in NVIC

    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // Start the timer
}