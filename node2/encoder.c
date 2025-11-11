#include "encoder.h"

encoder_init(){
    PMC->PMC_PCER1 |= (1 << (ID_TC6 - 32));
    PIOC->PIO_PDR |= PIO_PC25;
    PIOC->PIO_PDR |= PIO_PC26;
    PIOC->PIO_ABSR |= PIO_PC25;
    PIOC->PIO_ABSR |= PIO_PC26;

    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN;
}