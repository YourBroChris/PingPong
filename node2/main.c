#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
/*
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
        .brp = 3,       // Baud rate prescaler (BRP + 1 = 4 → 2 × (1/16MHz) × 4 = 500ns TQ)
        .sjw = 1,       // Synchronization Jump Width
        .propag = 3,    // Propagation segment
        .phase1 = 3,    // Phase segment 1
        .phase2 = 2,    // Phase segment 2
        .smp = 0        // Sampling mode (single sample)
    };

    typedef struct {
        uint8_t id;
        uint8_t length;
        uint32_t dword[2]; // 8 bytes max
    } CanMsg;

    CanMsg msg;

    
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
        if (can_rx(&msg)) {
            printf("CAN Msg ID: 0x%03X DLC: %d Data:", msg.id, msg.length);
            for (int i = 0; i < msg.length; i++) {
                uint8_t byte = (i < 4) ? (msg.dword[0] >> (i * 8)) & 0xFF
                                       : (msg.dword[1] >> ((i - 4) * 8)) & 0xFF;
                printf(" %02X", byte);
            }
            printf("\r\n");
        }
    }
    
}