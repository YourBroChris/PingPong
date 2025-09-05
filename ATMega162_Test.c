#include<avr/io.h>
//#include<util/delay.h>
#include<avr/interrupt.h>
#include<unistd.h>


#define FREQ 4915200  


int main(){
	//timer_init();
	while(1){
		PORTB = (1 << PB3);
		DDRB = (1 << DDB3);
		//sleep(3);
		//PORTA = (0 << PA5);
		/* Read port pins */
		//i = PINB;
		}
	}


