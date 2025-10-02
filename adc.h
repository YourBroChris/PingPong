#include <stdint.h>

struct raw_pos_t{
    uint8_t x;
    uint8_t y;
};
typedef struct raw_pos_t raw_pos_t;

struct pos_t{
    int x;
    int y;
};
typedef struct pos_t pos_t;

void adc_init (void);
uint8_t adc_read(); //volatile
void pos_calibrate();
void pos_read(pos_t *slider_pos, pos_t *joystick_pos);
void init_ADC_clk(void);
void ADC_test(void);
pos_t convert_pos(uint8_t rawpos_x, uint8_t rawpos_y);
