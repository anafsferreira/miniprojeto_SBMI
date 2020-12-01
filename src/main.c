

// #include <avr/io.h>
// #include <util/delay.h>
// #include "timer_tools.h"
// #include "serial_printf.h"
// #include <avr/interrupt.h>

// volatile uint8_t T2;
// volatile uint8_t T1 = 0;

// volatile uint16_t servos[6] = {4000, 2000, 3000, 3000, 3000, 3000};
// volatile uint8_t num_servo_atual = 0;


// void init_tc1(void){
//     TCCR1B = 0;
//     TIFR1 |=(7 << TOV1);
//     TCCR1B = (1<<WGM12) | (1<<CS11); // Modo CTC, prescaler = 8
//     TIMSK1 |= (1<<OCIE1A);
//     TCNT1 = 0;
//     OCR1A = 3000; // Servo centrado
// }


// ISR(TIMER1_COMPA_vect){
//     PORTD = (1 << (num_servo_atual + 2)); // Desliga o servo atual e liga o proximo
//     OCR1A = 3000; // Time para o proximo
//     _delay_ms(2);
//     num_servo_atual++;
//     if(num_servo_atual == 6)
//         num_servo_atual = 0;
// }



// int main(void){
//     init_tc1();
//     sei();
//     printf_init();
//     DDRD = 0b11111100;
//     for(;;){
//         printf("%d\n", num_servo_atual);
//         //OCR1A = 4000;
//     }
// }

// #define F_CPU 16000000
// #include <avr/io.h>
// #include <util/delay.h>
// void Wait()
// {
//    uint16_t i;
//    for(i=0;i<50;i++)
//    {
//       _delay_loop_2(0);
//       _delay_loop_2(0);
//       _delay_loop_2(0);
//    }
// }
// void main()
// {
//   //FOR TIMER1
//    TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
//    TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)
//    ICR1=4999;  //fPWM=50Hz 
//    DDRD|=(1<<PD4)|(1<<PD5);   //PWM Pins as Output
//    while(1)
//    {
//       OCR1A=316;  //90 degree
//       Wait();
//       OCR1A=97;   //0 degree
//       Wait();
//      OCR1A=535;  //180 degree
//       Wait();
//    }
// }

// #include <avr/io.h>
// #include <util/delay.h>

// int main(void) {
//  DDRB |= 1 << PINB1; // Set pin 9 on arduino to output

//  /* 1. Set Fast PWM mode 14: set WGM11, WGM12, WGM13 to 1 */
//  /* 3. Set pre-scaler of 8 */
//  /* 4. Set Fast PWM non-inverting mode */
//  TCCR1A |= (1 << WGM11) | (1 << COM1A1);
//  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);

//  /* 2. Set ICR1 register: PWM period */
//  ICR1 = 39999;

//  /* Offset for correction */
//  int offset = 800;

//  /* 5. Set duty cycle */
//  while(1) {
//   OCR1A = 3999 + offset;

//   _delay_ms(5000);

//   OCR1A = 1999 - offset;

//   _delay_ms(5000);
//  }

//  return 0;
// }