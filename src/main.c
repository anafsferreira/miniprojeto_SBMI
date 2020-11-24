

#include <avr/io.h>
#include <util/delay.h>
#include "timer_tools.h"
#include "serial_printf.h"
#include <avr/interrupt.h>


void init_tc1(void){
      TCCR1B = 0; // stop tc1
      TIFR1 |=(7 << TOV1); // clear pending intrpt
      /* Set Fast PWM non-inverting mode */
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) ;
      //init counter
      TCNT1 = 0;
      ICR1 = 1250; // 0,02* 16 MHz / 256

      TIMSK1 |= (1 << OCIE0A);
      // Prescaler 256
      TCCR1B |= (1 << CS12);

}

void init_tc0(void){
      TCCR0B = 0;
      TIFR0 |= (7 << TOV0);
      TCCR0A |= (1 << WGM01);// modo ctc

      TCNT0 = 0;
      OCR0A = 25; // 100 us * 16Mhz/ 64

      TIMSK0 |= (1 << OCIE0A);
      TCCR1B |= (1 << CS01) | (1 << CS00); // prescaler 64
}

ISR(TIMER1_COMPA_vect){
      
}

int main(void){



      while(1){




      }
}