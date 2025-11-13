#include "sam/sam3x/include/sam.h"
#include <stdio.h>
#include <stdint.h>

#define motor_MIN 0
#define motor_MAX 30


void motordriver_init();
void motorchange(uint8_t rawjoystickpos);
void goleft();