#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"
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
    /*
    SystemInit();
    uart_init(F_CPU, 9600);
    init_pwm();
    while(1){
        printf("CLK PRESCALER: %d \r\n", PWM->PWM_CLK);
    }
    */
    

    CanInit canTiming = {
        .phase2 = 5,
        .propag = 1,
        .phase1 = 6,
        .sjw    = 2,
        .brp    = 20,
        .smp    = 0
    };

    typedef struct CanMsg CanMsg;

    CanMsg msgRx;

    CanMsg msgTx = {
        .id = 10,
        .length = 1,
        .byte = {0x03}
    };

    SystemInit();
    uart_init(F_CPU, 9600);
    can_init(canTiming, 0); // 0 = no interrupt
    init_pwm();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    
    while (1)
    {
        can_tx(msgTx);
        if(can_rx(&msgRx)){
            change_pwm(msgRx.byte[1]);
            printf("CAN message: id=%d len=%d Joystick x: %d y: %d\r\n", msgRx.id, msgRx.length, msgRx.byte[0], msgRx.byte[1]);
        }
    }
    
}


void test_servo_header(){
    PMC->PMC_PCER0 = (1 << ID_PIOB); // Enable the clock for PIOB (Port B)

    // Step 2: Configure PB13 as an output
    PIOB->PIO_PER = PIO_PB13;        // Enable control of PB13 (set PIO Enable Register)
    PIOB->PIO_OER = PIO_PB13;        // Set PB13 as output (set Output Enable Register)
    //PIOB->PIO_PDR = PIO_PB13;        // Disable PIO (Parallel I/O) control for PB13

    // Step 3: Set PB13 high
    PIOB->PIO_SODR = PIO_PB13;      // Set PB13 high (Set Output Data Register)
    //PIOB->PIO_CODR = PIO_PB13; 
};