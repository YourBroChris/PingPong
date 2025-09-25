#include <stdint.h>
#include <avr/io.h>

struct pos_t{
    uint8_t x;
    uint8_t y;
};
typedef struct pos_t pos_t;

void adc_init (void);
uint8_t adc_read(uint8_t channel); //volatile
void pos_calibrate();
pos_t pos_read(void);
void init_ADC_clk(void);


