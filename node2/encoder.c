#include "encoder.h"

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

uint32_t encoder_read(){
    return TC2->TC_CHANNEL[0].TC_CV;
}
