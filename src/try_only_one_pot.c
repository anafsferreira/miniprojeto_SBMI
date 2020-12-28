
// #define F_CPU 16000000
// #include <avr/io.h>
// #include <math.h>
// #include <util/delay.h>
// #include "timer_tools.h"
// #include "serial_printf.h"
// #include <avr/interrupt.h>
// #include "adc_potenciometer.h"
// // segundo a datasheet roda no máximo 120
// // 4000 -2000 - > roda 90 graus / 45
// // 4100 - 1500 -> 90 + 15 

// // fechar mão - roda sentido contrário aos ponteiros do relógio
// #define MAX_SERVO 1670 // mão fechada
// #define MIN_SERVO 4330 // mão aberta
// #define MAX_POT 1023 // mão aberta
// #define MIN_POT 0 // mão fechada 

// uint16_t servo_pos_ant = 1023;
// uint16_t adc_ant = 0;
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




// int main(void){
//     uint16_t adc_value  = 0, tmp = 0, tmpF = 0;
   
    
//     float m = 0, b = 0;
    
//      m = (float)(MAX_SERVO - MIN_SERVO) / (float)(MIN_POT - MAX_POT);
//       b = (float)(MAX_SERVO - m * MIN_POT);

//     DDRB |= (1 << PB1); // pin 9 arduino - servo

    

//     init_tc1();
//     init_adc();
//     printf_init();
 
//     while (1)
//     {
//         adc_value = read_ADC(0); // ler no PC0 que corresponde ao A0
//         _delay_ms(10);
//         printf("%d\t", adc_value);
//         if(adc_value>MAX_POT){
//             adc_value  = MAX_POT;
//         }
//         else if( adc_value < MIN_POT){
//             adc_value = MIN_POT;
//         }
//         else if (abs(adc_value-adc_ant)<5){
//             adc_value = adc_ant;
//         }
//         else{
//             adc_ant = adc_value;
//         }


//         printf("%d\n", adc_value);
        
//         tmp = m * adc_value + b;
//         // filtro
//         tmpF = 0.94 * tmpF + 0.06 * tmp;
        
//         OCR1A = tmpF;
//         // if(abs(servo_pos_ant - tmp)>5){
//         //    // 180 - um bocado por tentativa erro
//         //     servo_pos_ant = tmp;
//         // }

        


        
//     }
    
// }
