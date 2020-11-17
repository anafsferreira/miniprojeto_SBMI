#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1 << 1);
    // PB1 is now an output - corresponds to OCR0A

    TCCR1A |= (1 << COM1A1);
    // set non-inverting mode

    TCCR1A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

    ICR1 = 39999;

    OCR1A = ICR1 - 2000;
    // gives servo motor position

    TCCR1B |= (1 << CS11);
    // set prescaler to 8 and starts PWM


    while(1)
    {
        // fazer isto
    }
}