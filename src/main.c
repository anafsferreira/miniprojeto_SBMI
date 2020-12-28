#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include "timer_tools.h"
#include "serial_printf.h"
#include <avr/interrupt.h>
#include "adc_potenciometer.h"
#include "adxl345_i2c.h"
// #define MAX_SERVO 4000 // 2 ms // mão fechada
//#define MIN_SERVO 2000 // 1 ms // mão aberta
// #define MAX_POT 800
// #define MIN_POT 100
#define THRESHOLD 30

// 20 ms = 40000
// volatile uint16_t servos[7] = {4000, 2000, 4000, 2000, 4000, 2000, 22000};
//volatile uint16_t servos[7] = {1670, 1670, 1670, 1670, 1670, 1670, 29980}; // alterar ainda a posição de inicio
volatile uint16_t servos[7] = {1670, 1670, 1670, 1670, 1670, 1670, 29980}; // alterar ainda a posição de inicio
volatile uint8_t num_servo_atual = 1;
volatile uint16_t servo_pos_ant[6] = {1023, 1023, 1023, 1023, 1023, 1023};
// int16_t MAX_SERVO[6] = {4330, 4330, 4330, 4330, 4330, 4330};
int16_t MIN_SERVO[6] = {4330, 4330, 4330, 4330, 4330, 4330};
int16_t MAX_SERVO[6] = {1670, 1670, 1670, 1670, 1670, 1670};
int16_t MAX_POT[6] = {440, 330, 440, 470, 480, 140}; // mão aberta, palma da mão para cima
int16_t MIN_POT[6] = {350, 230, 177, 280, 190, 7};   // mão fechada, palma da mão para baixo
uint16_t servo_posF[6] = {0, 0, 0, 0, 0, 0};
void init_tc1(void)
{
    TCCR1B = 0;           // stop tc1
    TIFR1 |= (7 << TOV1); // clear pending intrpt
                          /* Set CTC */
    TCCR1B |= (1 << WGM12);
    //init counter
    TCNT1 = 0;
    OCR1A = servos[0]; // 2 ms

    TIMSK1 |= (1 << OCIE0A);
    // Prescaler 8
    TCCR1B |= (1 << CS11);
}

ISR(TIMER1_COMPA_vect)
{
    // Desliga o servo atual e liga o proximo
    if (num_servo_atual < 6)
    {
        PORTB = (1 << (num_servo_atual));
        OCR1A = servos[num_servo_atual]; // Time para o proximo
        num_servo_atual++;
    }

    else if (num_servo_atual == 6)
    {
        OCR1A = servos[num_servo_atual];
        num_servo_atual = 0;
        PORTB &= ~(0xFF);
    }
}

// function that reads the value of each potenciometer that controls each servo
void updatePositions()
{
    uint16_t sum = 0;
    float m = 0, b = 0;
    uint16_t adc_value = 0, adc_ant = 0, servo_pos = 0;
    for (uint8_t channel = 0; channel < 7; channel++)
    { // todas as posições do array servos[]

        if (channel < 6)
        {

            m = (float)(MAX_SERVO[channel] - MIN_SERVO[channel]) / (float)(MIN_POT[channel] - MAX_POT[channel]);
            b = (float)(MAX_SERVO[channel] - m * MIN_POT[channel]);

            if (channel == 4)
            {
                adc_value = read_ADC(channel + 2);
            }
            else
                adc_value = read_ADC(channel);

            if (channel == 4)
                printf("%d\t", adc_value);
            if (adc_value > MAX_POT[channel])
            {
                adc_value = MAX_POT[channel];
            }
            else if (adc_value < MIN_POT[channel])
            {
                adc_value = MIN_POT[channel];
            }
            else if (abs(adc_value - adc_ant) < 5)
            {
                adc_value = adc_ant;
            }
            else
            {
                adc_ant = adc_value;
            }

            if (channel == 4)
                printf("%d\n", adc_value);
            servo_pos = m * adc_value + b;
            // filtro
            // if(channel == 1) printf("Servos filtro: %d\t", servo_posF[channel]);
            servo_posF[channel] = 0.94 * servo_posF[channel] + 0.06 * servo_pos;
            sum = sum + servo_posF[channel];
            // if(channel == 1){
            //     printf("servo_pos: %d\t servo_pos_F: %d\n", servo_pos, servo_posF[channel]);
            // }
            servos[channel] = servo_posF[channel]; // atualizar posição
            servo_pos_ant[channel] = servo_posF[channel];
            //servo_posF = 0;
        }
        else
        { // atualizar tempo restante

            servos[channel] = 40000 - sum;
        }
    }
}

int main(void)
{
    DDRB |= 0xFF; // make all output
    init_tc1();
    PORTB = (1 << 0); // ativar o primeiro servo até ser lançada a primeira interrupção
    sei();
    init_adc(); // iniciar ADC
    printf_init();

    while (1)
    {
        updatePositions();
    }
}