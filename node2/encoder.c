#include "encoder.h"
#include "motordriver.h"

void delay_ms(uint32_t ms) {
    // crude delay loop based on CPU frequency
    for (uint32_t i = 0; i < (ms * (84000000 / 13000)); i++) {
        __asm("nop");
    }
}

void encoder_init(){
    PMC->PMC_PCER1 |= (1 << (ID_TC6 - 32));
    PIOC->PIO_PDR |= PIO_PC25;
    PIOC->PIO_PDR |= PIO_PC26;
    PIOC->PIO_ABSR |= PIO_PC25;
    PIOC->PIO_ABSR |= PIO_PC26;

    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN;

    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;

    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

int encoder_read(){
    return TC2->TC_CHANNEL[0].TC_CV;
}


struct enc_boundaries calibrating_encoder(){
    struct enc_boundaries boundaries;
    // Move all the way to the right
    goright();
    delay_ms(2000); // Wait for 2 seconds to ensure it reaches the boundary
    boundaries.right_boundary = encoder_read();
    printf("Right boundary: %d\r\n", boundaries.right_boundary);

    // Move all the way to the left
    goleft();
    delay_ms(2000); // Wait for 2 seconds to ensure it reaches the boundary
    boundaries.left_boundary = encoder_read();
    printf("Left boundary: %d\r\n", boundaries.left_boundary);
    return boundaries;
}

