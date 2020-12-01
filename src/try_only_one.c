#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

void init_tc1(void){
    TCCR1B = 0; // stop tc1
    TIFR1 |=(7 << TOV1); // clear pending intrpt
    /* Set Fast PWM non-inverting mode */
	TCCR1A = (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) ;
    //init counter
    TCNT1 = 0;
    ICR1 = 1249; // 0,02* 16 MHz / 256

    //TIMSK1 |= (1 << OCIE0A);
    // Prescaler 256
    TCCR1B |= (1 << CS12);

}



int main(void){
    init_tc1();
    
    DDRB |= (1 << PB1);
    while (1)
    {
        
        OCR1A = 150; // 180 - um bocado por tentativa erro
        _delay_ms(500);
       
        OCR1A = 0; // 0 
        _delay_ms(500);
        
    }
    
}
