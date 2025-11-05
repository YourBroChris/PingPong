#include <stdio.h>
#include <stdarg.h>
#include "sam.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    PMC->PMC_PCER0 |= (1 << ID_PIOB);

    // Disable PWM control on PB13 (in case it was enabled)
    // Make sure PB13 is under PIO control, not peripheral
    PIOB->PIO_PER = (1 << 13);    // Enable PIO control on PB13
    PIOB->PIO_OER = (1 << 13);    // Configure PB13 as output

    // Set PB13 HIGH
    

    
    //Uncomment after including uart above
    //uart_init(/*cpufreq*/, /*baud*/);
    //printf("Hello World\n\r");

    while (1)
    {

        PIOB->PIO_SODR = (1 << 13);   // Set output data register (drive pin high)
    }
    
}