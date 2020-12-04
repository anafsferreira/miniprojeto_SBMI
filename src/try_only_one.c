
// #define F_CPU 16000000
// #include <avr/io.h>
// #include <util/delay.h>
// #include "timer_tools.h"
// #include "serial_printf.h"
// #include <avr/interrupt.h>

// // segundo a datasheet roda no máximo 120
// // 4100 -2000 - > roda 90 graus / 45
// // 4100 - 1000 - > roda 90 + 45 // descontrola
// // 4100 - 1500 -> 90 + 15 

// #define MAX_SERVO 4000 // 2 ms
// #define MIN_SERVO 2000 // 1 ms
// #define MAX_POT 1023
// #define MIN_POT 0 

// void init_tc1(void){
//     TCCR1B = 0; // stop tc1
//     TIFR1 |=(7 << TOV1); // clear pending intrpt
//     /* Set Fast PWM non-inverting mode */
// 	TCCR1A = (1 << WGM11) | (1 << COM1A1);
// 	TCCR1B |= (1 << WGM12) | (1 << WGM13) ;
//     //init counter
//     TCNT1 = 0;
//     ICR1 = 40000; // 20 ms

//     TIMSK1 |= (1 << OCIE0A);
//     // Prescaler 8
//     TCCR1B |= (1 << CS11);

// }

// void init_adc(void){
//     ADMUX |= (1 << REFS0); // definir Vref
//     ADCSRA |= (7 << ADPS0); // prescaler 128
//     ADCSRA |= (1 << ADEN); // ativar adc
//     DIDR0 |= (1 << PC0); // desativar o buffer digital

// }

// unsigned int read_ADC(uint8_t channel){
//     /* Seleciona canal e referência */
//     ADMUX = (ADMUX & 0xF0)  | (channel & 0x0F);

//     ADCSRA |= (1 << ADSC); // iniciar a conversao em modo manual

//     while(ADCSRA & ( 1 << ADSC)); // esperar pelo fim da conversão

//     return ADC;
// }



// int main(void){
//     uint16_t adc_value  = 0;
//     uint16_t m = 0;
//     uint16_t b = MIN_SERVO;
//     m = (MAX_SERVO - MIN_SERVO)/MAX_POT;
//     DDRB |= (1 << PB1); // pin 9 arduino - servo
    

//     init_tc1();
//     init_adc();
//     printf_init();
 
//     while (1)
//     {
//         adc_value = read_ADC('0'); // ler no PC0 que corresponde ao A0
//         printf("%d\n", adc_value);
//         OCR1A = m * adc_value + b; // 180 - um bocado por tentativa erro
//         _delay_ms(20);

        
//     }
    
// }
