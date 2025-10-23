#pragma once
#include <stdint.h>
#include <stdlib.h> //for rand()
#include <avr/io.h>
#include "usart.h"
#define CONFIG1_BASE_ADDRESS 0x1000 // ADC
#define CONFIG2_BASE_ADDRESS 0x1400 // SRAM
void init_xmem(void);
void write_xmem(uint8_t data, uint16_t addr);
uint8_t read_xmem(uint16_t addr);
void SRAM_test(void);