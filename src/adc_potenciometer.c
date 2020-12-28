#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial_printf.h"
#include "adc_potenciometer.h"

void init_adc(void)
{
    ADMUX |= (1 << REFS0);  // definir Vref
    ADCSRA |= (7 << ADPS0); // prescaler 128
    ADCSRA |= (1 << ADEN);  // ativar adc
    DIDR0 |= 0xFF;          // desativar o buffer digital
}

unsigned int read_ADC(uint8_t channel)
{
    /* Seleciona canal e referência */
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    ADCSRA |= (1 << ADSC); // iniciar a conversao em modo manual

    while (ADCSRA & (1 << ADSC)); // esperar pelo fim da conversão

    return ADC;
}