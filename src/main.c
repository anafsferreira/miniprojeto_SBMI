

#include <avr/io.h>
#include <util/delay.h>
#include "timer_tools.h"
#include "serial_printf.h"
#include <avr/interrupt.h>

volatile uint8_t T2;
volatile uint8_t T1 = 0;

// 20 ms = 40000
volatile uint16_t servos[7] = {4000, 2000, 4000, 2000, 4000, 2000, 22000 };
volatile uint8_t num_servo_atual = 1;


void init_tc1(void){
    TCCR1B = 0; // stop tc1
    TIFR1 |=(7 << TOV1); // clear pending intrpt
    /* Set CTC */
	
	TCCR1B |= (1 << WGM12);
    //init counter
    TCNT1 = 0;
    OCR1A = servos[0]; // 2 ms

    TIMSK1 |= (1 << OCIE0A);
    // Prescaler 8
    TCCR1B |= (1 << CS11);

}



ISR(TIMER1_COMPA_vect){
     // Desliga o servo atual e liga o proximo
    if(num_servo_atual < 6){
        PORTB = (1 << (num_servo_atual));
        OCR1A = servos[num_servo_atual]; // Time para o proximo
        num_servo_atual++;
    }
    
    else if(num_servo_atual == 6){
        OCR1A = servos[num_servo_atual];
        num_servo_atual = 0;
        PORTB &= ~(0xFF);

    }
        
}



int main(void){
    DDRB |= 0xFF; // make all output
    init_tc1();
    PORTB = (1 << 0);
    sei();
    printf_init();
    
    while(1){
        printf("%d\n", num_servo_atual);
        //OCR1A = 4000;
    }
}
