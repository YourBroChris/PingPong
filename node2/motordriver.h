#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "encoder.h"
#include "pid.h"
#include "sam/sam3x/include/sam.h"
#include <stdio.h>
#include <stdint.h>

#define motor_MIN 0
#define motor_MAX 30


void motordriver_init();
int joystickToMotorPosition(uint8_t x, struct enc_boundaries *boundaries);
void motorDriveVelocity(uint8_t rawjoystickpos);
void motorDrivePosition(int targetPosition, int currentPosition, struct enc_boundaries* boundaries, struct PIDController* pid);
void goleft();
void goright();
void motorstop();

#endif