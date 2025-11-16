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


int wait_until_stopped(void) {
    int last = encoder_read();
    int stable_count = 0;

    while (1) {
        delay_ms(5); // small delay
        int now = encoder_read();

        if (now == last) {
            stable_count++;
            if (stable_count > 10) {  
                // encoder unchanged for ~50 ms
                return now;
            }
        } else {
            stable_count = 0;
        }

        last = now;
    }
}

struct enc_boundaries calibrating_encoder(){
    struct enc_boundaries boundaries;
    // Move all the way to the right
    goright();
    boundaries.right_boundary = wait_until_stopped();
    printf("Right boundary: %d\r\n", boundaries.right_boundary);

    // Move all the way to the left
    goleft();
    boundaries.left_boundary = wait_until_stopped();
    printf("Left boundary: %d\r\n", boundaries.left_boundary);
    motorstop();
    return boundaries;
}

