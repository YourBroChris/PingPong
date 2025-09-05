#define FOSC 4915200 // Clockspeed
#define BAUD 9600 // Baudrate
#define MYUBBR FOSC/16/BAUD-1

void init_usart(unsigned int ubrr);
void transmit_usart(unsigned char data);
unsigned char receive_usart(void);
void init_interrupts(void);