// // o azul roda sempre mas a velocidades diferentes
// // o servo preto pequeno de manha funcionava agora deixou de funcionar

// #include <avr/io.h>
// #include <util/delay.h>
// #include "timer_tools.h"
// #include "serial_printf.h"
// #include <avr/interrupt.h>


// int main(void){

//     DDRB = 0xFF;
//     // Fast PWM
//     TCCR1A = (1 << WGM11)| (1 << COM1A1);; 
// 	TCCR1B |= (1 << WGM12) | (1 << WGM13) ;
//     // Prescaler 256
//     TCCR1B |= (1 << CS12);
//     TIMSK1 |= (1 << OCIE0A);
//     ICR1 = 1249; // 0,02* 16 MHz / 256

//     sei();
//     while(1){

//         if(TCNT1 >= 25 && TCNT1 <= 150){
//             if(TCNT1 >= 125 && bit_is_set(PORTB, PINB0)) PORTB &= ~(1 << PINB0); // Passado 2 ms o sinal passa a OFF // assim roda tudo
//             if(TCNT1 >= 60 && bit_is_set(PORTB, PINB1)) PORTB &= ~(1 << PINB1);// se for 60 já só roda cerca de 90 º
//             if(TCNT1 >= 80 && bit_is_set(PORTB, PINB2)) PORTB &= ~(1 << PINB2); 
//             if(TCNT1 >= 100 && bit_is_set(PORTB, PINB3)) PORTB &= ~(1 << PINB3);
//             if(TCNT1 >= 30 && bit_is_set(PORTB, PINB4)) PORTB &= ~(1 << PINB4);
//         }

//     }
// }

// ISR(TIMER1_COMPA_vect){
//     PORTB |= 0xFF; // Todos ON
// }