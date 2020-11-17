/*******************************************
 * Example 3
 *******************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "timer_tools.h"
#include "serial_printf.h"
#include <avr/interrupt.h>

#define LED PB5


// up to 8 servos on portB sequentially pulsed
// uses timer1 in CTC mode
// synchro gap is constant, 6 ms
// frame is variable
// min.frame = 8*1+6 = 14 ms
// max.frame = 8*2+6 = 22 ms

// Mega88, 8 MHz


// initial pulse times in us for servo 0, 1, 2...7 (last value 6000 is the synchro gap)
unsigned int servo[2] = {1500, 6000};

//----------------------------------------------------------------

ISR(TIMER1_COMPA_vect)
{
static unsigned char servo_num;

   PORTB = (1<<(servo_num + 1 ));          // end pulse for servo (n), start pulse for servo (n+1)            
   OCR1A = servo[servo_num];        // set width of pulse
   servo_num++;                     // prepare next servo 
   if(servo_num > 8) servo_num = 0; // again from servo 0;
}


////////////////// MAIN //////////////////////////////
int main (void)
{
      DDRB = 255;                        // portb output
      TCCR1B |= (1<<WGM12) | (1<<CS11);  // pwm mode 4,CTC, prescale=8
      TIMSK1 |= (1<<OCIE1A);             // enable T1_compareA interrupt 
      TCNT1 = 65530;
      sei();   

// test 
      for(;;)                   
      {
         servo[0] = 1000;          // servos 0 and 1 left
         servo[1] = 1000;
         _delay_ms(2000);

         servo[0] = 1500;          // servos 0 and 1 center
         servo[1] = 1500;
         _delay_ms(2000);

         servo[0] = 2000;          // servos 0 and 1 right
         servo[1] = 2000;
         _delay_ms(2000);  
      }
}
////////////////// END MAIN //////////////////////////