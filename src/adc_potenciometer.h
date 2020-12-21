#ifndef ADC_POTENCIOMETER_H_
#define ADC_POTENCIOMETER_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial_printf.h"

void init_adc(void);
unsigned int read_ADC(uint8_t channel);

#endif