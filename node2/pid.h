#ifndef PID_H
#define PID_H

struct PIDController
{
    float kp;
    float ki;
    float kd;
    float dt;
    float prev_error;
    float integral;
};

void timer0_init(void);
void pid_init(struct PIDController* pid, float kp, float ki, float kd, float dt);
float pid_compute(struct PIDController* pid, float setpoint, float measured);

#endif