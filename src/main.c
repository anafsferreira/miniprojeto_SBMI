

// #include <avr/io.h>
// #include <util/delay.h>
// #include "timer_tools.h"
// #include "serial_printf.h"
// #include <avr/interrupt.h>

// volatile uint8_t T2;
// volatile uint8_t T1 = 0;

// volatile uint16_t servos[6] = {32000, 24000, 16500, 32000, 24000, 16500};
// volatile uint8_t num_servo_atual = 0;


// void init_tc1(void){
//     TCCR1B = 0; // stop tc1
//     TIFR1 |=(7 << TOV1); // clear pending intrpt
//     /* Set Fast PWM non-inverting mode */
// 	TCCR1A = (1 << WGM11) | (1 << COM1A1);
// 	TCCR1B |= (1 << WGM12) | (1 << WGM13) ;
//     //init counter
//     TCNT1 = 0;
//     ICR1 = 32000; // 2 ms

//     TIMSK1 |= (1 << OCIE0A);
//     // Prescaler 1
//     TCCR1B |= (1 << CS10);

// }



// ISR(TIMER1_COMPA_vect){
//     PORTB = (1 << (num_servo_atual)); // Desliga o servo atual e liga o proximo
//     OCR1A = servos[num_servo_atual]; // Time para o proximo
//     num_servo_atual++;
//     if(num_servo_atual == 6)
//         num_servo_atual = 0;
// }



// int main(void){
//     init_tc1();
//     sei();
//     printf_init();
//     DDRB |= 0xFF; // make all output
//     for(;;){
//         printf("%d\n", num_servo_atual);
//         //OCR1A = 4000;
//     }
// }
