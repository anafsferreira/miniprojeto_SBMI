
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include "timer_tools.h"
#include "serial_printf.h"
#include <avr/interrupt.h>
#include "i2c.h"


#define PI 3.14159265358979323846
#define RANGE_2G            0b00001000 
#define MEASURE             0x08 
#define DATA_RATE_100       0x0A
#define I2C_ADDR            0x53
#define DEVID               0x00
#define POWER_CTL           0x2D
#define BW_RATE             0x2C
#define DATA_FORMAT         0x31
#define DATAX               0x32
#define DATAX0              0x32 // X-Axis Data 0
#define DATAX1              0x33 // X-Axis Data 1
#define DATAY               0x34
#define DATAY0              0x34 // Y-Axis Data 0
#define DATAY1              0x35 // Y-Axis Data 1
#define DATAZ               0x36
#define DATAZ0              0x36 // Z-Axis Data 0
#define DATAZ1              0x37 // Z-Axis Data 1
#define OFSX                0x1E
#define OFSY                0x1F
#define OFSZ                0x20

// #define MAX_SERVO 4000 // 2 ms // mão fechada
//#define MIN_SERVO 2000 // 1 ms // mão aberta
// #define MAX_POT 1023
// #define MIN_POT 0
#define THRESHOLD_SERVO 30

// 20 ms = 40000
volatile uint16_t servos[7] = {4000, 2000, 4000, 2000, 4000, 2000, 22000};
volatile uint8_t num_servo_atual = 1;
volatile uint16_t servo_pos_ant[6] = {1023, 1023, 1023, 1023, 1023, 1023};
uint16_t MAX_SERVO[6] = {4330, 4330, 4330, 4330, 4330, 4330};
uint16_t MIN_SERVO[6] = {1670, 1670, 1670, 1670, 1670, 1670};
uint16_t MAX_POT[6]= {605, 425, 440, 445, 480, 140};
uint16_t MIN_POT[6] ={357, 262, 177, 305, 190, 7};
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

void init_adc(void)
{
    ADMUX |= (1 << REFS0);  // definir Vref
    ADCSRA |= (7 << ADPS0); // prescaler 128
    ADCSRA |= (1 << ADEN);  // ativar adc
    DIDR0 |= 0xFF;          // desativar o buffer digital
}

unsigned int read_ADC(uint8_t channel)
{
    /* Seleciona canal e referência */
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    ADCSRA |= (1 << ADSC); // iniciar a conversao em modo manual

    while (ADCSRA & (1 << ADSC)); // esperar pelo fim da conversão

    return ADC;
}

uint8_t ADXL345_ID;


void ADXL345_init(uint8_t range, uint8_t data_rate)
{
    uint8_t var = 0;
    i2c_read(I2C_ADDR, 1, DEVID, &ADXL345_ID);
    printf("Device ID=%d\n\r", ADXL345_ID);
    i2c_write(I2C_ADDR, 1, POWER_CTL, &var);
    i2c_write(I2C_ADDR, 1, BW_RATE, &data_rate);
    i2c_write(I2C_ADDR, 1, DATA_FORMAT, &range);
    var = MEASURE;
    i2c_write(I2C_ADDR, 1, POWER_CTL, &var);
    _delay_ms(20);
}

int16_t read_Xdata(void){

    int16_t data;
    uint8_t buf[2];

    i2c_read(I2C_ADDR, 2, DATAX, buf);
    data = (int16_t)buf[1]<<8;
    data += (int16_t)buf[0];
    return(data);
}

int16_t read_Ydata(void){

    int16_t data;
    uint8_t buf[2];

    i2c_read(I2C_ADDR, 2, DATAY, buf);
    data = (int16_t)buf[1]<<8;
    data += (int16_t)buf[0];
    return(data);
}

int16_t read_Zdata(void){

    int16_t data;
    uint8_t buf[2];

    i2c_read(I2C_ADDR, 2, DATAZ, buf);
    data = (int16_t)buf[1]<<8;
    data += (int16_t)buf[0];
    return(data);
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
            else if(channel == 5){ // read accelerometer
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
