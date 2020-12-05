

// #include <avr/io.h>
// #include <util/delay.h>
// #include "timer_tools.h"
// #include "serial_printf.h"
// #include <avr/interrupt.h>

// // #define MAX_SERVO 4000 // 2 ms // mão fechada
// //#define MIN_SERVO 2000 // 1 ms // mão aberta
// #define MAX_POT 1023
// #define MIN_POT 0

// // 20 ms = 40000
// volatile uint16_t servos[7] = {4000, 2000, 4000, 2000, 4000, 2000, 22000 };
// volatile uint8_t num_servo_atual = 1;

// uint16_t MAX_SERVO[6] = {4000, 4000, 4000, 4000, 4000, 4000};
// uint16_t MIN_SERVO[6] = {2000, 2000, 2000, 2000, 2000, 2000};

// void init_tc1(void){
//     TCCR1B = 0; // stop tc1
//     TIFR1 |=(7 << TOV1); // clear pending intrpt
//     /* Set CTC */
// 	TCCR1B |= (1 << WGM12);
//     //init counter
//     TCNT1 = 0;
//     OCR1A = servos[0]; // 2 ms

//     TIMSK1 |= (1 << OCIE0A);
//     // Prescaler 8
//     TCCR1B |= (1 << CS11);

// }


// ISR(TIMER1_COMPA_vect){
//      // Desliga o servo atual e liga o proximo
//     if(num_servo_atual < 6){
//         PORTB = (1 << (num_servo_atual));
//         OCR1A = servos[num_servo_atual]; // Time para o proximo
//         num_servo_atual++;
//     }
    
//     else if(num_servo_atual == 6){
//         OCR1A = servos[num_servo_atual];
//         num_servo_atual = 0;
//         PORTB &= ~(0xFF);

//     }
        
// }


// void init_adc(void){
//     ADMUX |= (1 << REFS0); // definir Vref
//     ADCSRA |= (7 << ADPS0); // prescaler 128
//     ADCSRA |= (1 << ADEN); // ativar adc
//     DIDR0 |= 0xFF; // desativar o buffer digital

// }

// unsigned int read_ADC(uint8_t channel){
//     /* Seleciona canal e referência */
//     ADMUX = (ADMUX & 0xF0)  | (channel & 0x0F);

//     ADCSRA |= (1 << ADSC); // iniciar a conversao em modo manual

//     while(ADCSRA & ( 1 << ADSC)); // esperar pelo fim da conversão

//     return ADC;
// }

// // function that reads the value of each potenciometer that controls each servo
// void updatePositions(){
//     uint8_t sum = 0;
//     float m = 0;
//     uint16_t b = 0, adc_value = 0, servo_pos = 0;
//     for(uint8_t channel= 0; channel < 7; channel++){ // todas as posições do array servos[]
        
//         if(channel < 6){
            
//             b = MIN_SERVO[channel];
//             m = (float)(MAX_SERVO[channel] - MIN_SERVO[channel])/(float)MAX_POT;
//             adc_value = read_ADC(channel);
//             servo_pos =  m * adc_value + b;
//             sum = sum + servo_pos;
//             servos[channel] = servo_pos;
            
//         }
//         else{ // atualizar tempo restante
//             servos[channel] = 40000-sum;
//         }
//     }
// }





// int main(void){
//     DDRB |= 0xFF; // make all output
//     init_tc1();
//     PORTB = (1 << 0); // ativar o primeiro servo até ser lançada a primeira interrupção
//     sei();
//     init_adc(); // iniciar ADC
//     printf_init();
    
//     while(1){
        
//         updatePositions();

        
//     }
// }
