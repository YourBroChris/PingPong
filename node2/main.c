#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"
#include "can.h"
/*write_instruction
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

#define F_CPU 84000000

int main()
{

    CanInit canTiming = {
        .phase2 = 5,
        .propag = 1,
        .phase1 = 6,
        .sjw    = 2,
        .brp    = 20,
        .smp    = 0
    };

typedef struct CanMsg CanMsg;

    CanMsg msg;

    CanMsg msgTx = {
        .id = 10,
        .length = 1,
        .byte = {0x03}
    };

    SystemInit();
    uart_init(F_CPU, 9600);
    can_init(canTiming, 0); // No interrupt
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    //uart_init(/*cpufreq*/, /*baud*/);
    //printf("Hello World\n\r");
    
    PMC->PMC_PCER0 = (1 << ID_PIOB); // Enable the clock for PIOB (Port B)

    // Step 2: Configure PB13 as an output
    PIOB->PIO_PER = PIO_PB13;        // Enable control of PB13 (set PIO Enable Register)
    PIOB->PIO_OER = PIO_PB13;        // Set PB13 as output (set Output Enable Register)
    //PIOB->PIO_PDR = PIO_PB13;        // Disable PIO (Parallel I/O) control for PB13

    // Step 3: Set PB13 high
    PIOB->PIO_SODR = PIO_PB13;      // Set PB13 high (Set Output Data Register)
    //PIOB->PIO_CODR = PIO_PB13; 
    while (1)
    {
        can_tx(msgTx);
        // if(can_rx(&msg)){
        //     printf("CAN message: id=%d len=%d data=%d\r\n", msg.id, msg.length, msg.byte[0]);
        // }
    }
}
