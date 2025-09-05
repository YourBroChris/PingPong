#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<unistd.h>


#define FREQ 4915200  


int main(){
	//timer_init();
	while(1){
		PORTA = (1 << PA5);
		DDRA = (1 << DDA5);
		sleep(3);
		PORTA = (0 << PA5);
		}
	}


