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
#define MEASURES_NUM_ACCEL 5
#define data_size 6
// 20 ms = 40000
// volatile uint16_t servos[7] = {4000, 2000, 4000, 2000, 4000, 2000, 22000};
//volatile uint16_t servos[7] = {1670, 1670, 1670, 1670, 1670, 1670, 29980}; // alterar ainda a posição de inicio
volatile uint16_t servos[7] = {1670, 1670, 1670, 1670, 1670, 1670, 29980}; // alterar ainda a posição de inicio
volatile uint8_t num_servo_atual = 1;
volatile uint16_t servo_pos_ant[6] = {1023, 1023, 1023, 1023, 1023, 1023};
volatile float roll_ant = 0;

// int16_t MAX_SERVO[6] = {4330, 4330, 4330, 4330, 4330, 4330};
int16_t MIN_SERVO[6] = {4330, 4330, 4330, 4330, 4330, 4800};
int16_t MAX_SERVO[6] = {1670, 1670, 1670, 1670, 1670, 1088};
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
    int16_t datax, datay, dataz;
    float X_out, Y_out, Z_out, roll, pitch, rollF = 0, pitchF = 0;
    float filter[data_size][MEASURES_NUM_ACCEL];
    for (uint8_t channel = 0; channel < 7; channel++)
    { // todas as posições do array servos[]

        if (channel < 6)
        {

            m = (float)(MAX_SERVO[channel] - MIN_SERVO[channel]) / (float)(MIN_POT[channel] - MAX_POT[channel]);
            b = (float)(MAX_SERVO[channel] - m * MIN_POT[channel]);
            if (channel < 5)
            {
                for (int k = 0; k < MEASURES_NUM_ACCEL; k++)
                {
                    if (channel == 4)
                    {
                        adc_value = read_ADC(channel + 2);
                    }
                    else
                        adc_value = read_ADC(channel);

                    filter[channel][k] = adc_value;
                }
                adc_value = filter[channel][(MEASURES_NUM_ACCEL - 1) / 2]; // mediana

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

                servo_pos = m * adc_value + b;
                // filtro
                // if(channel == 1) printf("Servos filtro: %d\t", servo_posF[channel]);
                servo_posF[channel] = 0.94 * servo_posF[channel] + 0.06 * servo_pos;
            }
            else if (channel == 5)
            { // read acelerometer

                for (int k = 0; k < MEASURES_NUM_ACCEL; k++)
                {
                    datax = read_Xdata();
                    X_out = (float)datax / 256.0;
                    datay = read_Ydata();
                    Y_out = (float)datay / 256.0;
                    dataz = read_Zdata();
                    Z_out = (float)dataz / 256.0;
                    roll = atan2(Y_out, Z_out) * 180 / PI;
                    filter[channel][k] = roll;
                }

                // roll and pitch

                // roll = atan2(Y_out, Z_out) * 180 / PI;
                // pitch = atan2((-X_out), sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
                roll = filter[channel][(MEASURES_NUM_ACCEL - 1) / 2];
                if (roll < 0)
                {
                    roll = roll + 360;
                }

                // printf("%f\t", roll);
                if (roll > MAX_POT[channel] && roll < 300)
                {
                    roll = MAX_POT[channel];
                    roll_ant = roll;
                }
                else if (roll < MIN_POT[channel] || roll >= 300)
                {
                    roll = MIN_POT[channel];
                    roll_ant = roll;
                }
                else if (fabs(roll - roll_ant) < 1)
                {
                    roll = roll_ant;
                }
                else
                {
                    roll_ant = roll;
                }
                // printf("%f\t", roll);
                // printf("%f\n", roll_ant);

                servo_pos = m * roll + b;
                servo_posF[channel] = 0.2 * servo_posF[channel] + 0.8 * servo_pos;
                printf("%d\n", servo_posF[channel]);
            }
            sum = sum + servo_posF[channel];
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

    int8_t offx, offy, offz;
    printf_init();
    i2c_init(100000);
    ADXL345_init(RANGE_2G, DATA_RATE_100);

    // offsets
    // datax = 17
    offx = (-17 / 4);
    printf("%d\n", offx);
    i2c_write(I2C_ADDR, 1, OFSX, &offx);

    offy = (23 / 4);
    printf("%d\n", offy);
    i2c_write(I2C_ADDR, 1, OFSY, &offy);
    while (1)
    {
        updatePositions();
    }
}