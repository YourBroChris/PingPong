#ifndef ENCODER_H
#define ENCODER_H

#include "sam/sam3x/include/sam.h"
#include <stdio.h>
#include <stdint.h>

struct enc_boundaries
{
    int left_boundary;
    int right_boundary;
};


void encoder_init();
int encoder_read();
struct enc_boundaries calibrating_encoder();

#endif