
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include "timer_tools.h"
#include "serial_printf.h"
#include <avr/interrupt.h>
#include "adxl345_i2c.h"
#include "adc_potenciometer.h"


// #define MAX_SERVO 4000 // 2 ms // mão fechada
//#define MIN_SERVO 2000 // 1 ms // mão aberta
// #define MAX_POT 1023
// #define MIN_POT 0
#define THRESHOLD_SERVO 30

// 20 ms = 40000
volatile uint16_t servos[7] = {4000, 2000, 4000, 2000, 4000, 2000, 22000}; // alterar ainda a posição de inicio
volatile uint8_t num_servo_atual = 1;
volatile uint16_t servo_pos_ant[6] = {1023, 1023, 1023, 1023, 1023, 1023};
uint16_t MAX_SERVO[6] = {4330, 4330, 4330, 4330, 4330, 4330};
uint16_t MIN_SERVO[6] = {1670, 1670, 1670, 1670, 1670, 1670};
uint16_t MAX_POT[6]= {605, 425, 440, 445, 480, 140}; // mão aberta, palma da mão para cima
uint16_t MIN_POT[6] ={357, 262, 177, 305, 190, 7}; // mão fechada, palma da mão para baixo

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
    uint8_t sum = 0;
    float m = 0, b = 0;
    uint16_t adc_value = 0, servo_pos = 0;
      
    int16_t datax, datay, dataz;
    float X_out, Y_out, Z_out, roll, pitch, rollF = 0, pitchF = 0;

    for (uint8_t channel = 0; channel < 7; channel++)
    { // todas as posições do array servos[]

        if (channel < 6)
        {
            m = (float)(MAX_SERVO[channel] - MIN_SERVO[channel]) / (float)(MIN_POT[channel]-MAX_POT[channel]);
            b = (float)(MAX_SERVO[channel]- m* MIN_POT[channel]);
            if(channel < 5){
                
                if(channel == 4){
                    adc_value = read_ADC(channel+2);
                }
                else adc_value = read_ADC(channel);
                servo_pos = m * adc_value + b;
            }
            else if(channel == 5){
                datax = read_Xdata();
                X_out = (float)datax/256.0;
                datay = read_Ydata();
                Y_out = (float)datay/256.0;
                dataz = read_Zdata();
                Z_out = (float)dataz/256.0;
                // roll and pitch
                
                roll = atan2(Y_out , Z_out) * 180 / PI;
                pitch = atan2((- X_out) , sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
                servo_pos = m * roll + b;

                // filtro low-pass
                rollF = 0.94*rollF + 0.06* roll;
                pitchF = 0.94*pitchF + 0.06 * pitch;
            }

            if (abs(servo_pos_ant[channel] - servo_pos) > THRESHOLD_SERVO)
            {
                sum = sum + servo_pos;
                servos[channel] = servo_pos;
                servo_pos_ant[channel] = servo_pos;
                
            }
            
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
    offx= (-17/4);
    printf("%d\n",offx);
    i2c_write(I2C_ADDR, 1, OFSX, &offx);

    offy= (23/4);
    printf("%d\n",offy);
    i2c_write(I2C_ADDR, 1, OFSY, &offy);

    while (1)
    {
        
        updatePositions();
        
    }
}
