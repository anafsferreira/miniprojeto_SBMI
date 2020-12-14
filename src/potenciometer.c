#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial_printf.h"

void init_adc(void){
    ADMUX |= (1 << REFS0); // definir Vref
    ADCSRA |= (7 << ADPS0); // prescaler 128
    ADCSRA |= (1 << ADEN); // ativar adc
    DIDR0 |= (1 << PC0); // desativar o buffer digital

}

unsigned int read_ADC(uint8_t channel){
    /* Seleciona canal e referência */
    ADMUX = (ADMUX & 0xF0)  | (channel & 0x0F);

    ADCSRA |= (1 << ADSC); // iniciar a conversao em modo manual

    while(ADCSRA & ( 1 << ADSC)); // esperar pelo fim da conversão

    return ADC;
}

int main(void){
    DDRB |= (1 << PB5); // LED
    uint16_t adc_value;
    init_adc();
    printf_init();
    while (1)
    {
        adc_value = read_ADC('0');
        printf("%d\n", adc_value);
    }
    


}