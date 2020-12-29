// #define F_CPU 16000000
// #include <avr/io.h>
// #include <util/delay.h>
// #include "timer_tools.h"
// #include "serial_printf.h"
// #include <avr/interrupt.h>
// #include "adxl345_i2c.h"
// #include <math.h>
// // segundo a datasheet roda no máximo 120
// // 4000 -2000 - > roda 90 graus / 45
// // 4100 - 1500 -> 90 + 15
// // 4330 - 1670 -> 120
// #define MAX_SERVO 6000 // 2 ms
// #define MIN_SERVO 2000 // 1 ms
// #define MAX_POT 150    // palma da mão para cima
// #define MIN_POT 7      // palama da mão para baixo

// float roll_ant = 0;

// void init_tc1(void)
// {
//     TCCR1B = 0;           // stop tc1
//     TIFR1 |= (7 << TOV1); // clear pending intrpt
//                           /* Set Fast PWM non-inverting mode */
//     TCCR1A = (1 << WGM11) | (1 << COM1A1);
//     TCCR1B |= (1 << WGM12) | (1 << WGM13);
//     //init counter
//     TCNT1 = 0;
//     ICR1 = 40000; // 20 ms

//     TIMSK1 |= (1 << OCIE0A);
//     // Prescaler 8
//     TCCR1B |= (1 << CS11);
// }

// int main(void)
// {
//     int8_t offx, offy, offz;
//     float m = 0, b = 0;
//     uint16_t servo_pos = 0, servo_posF = 0;
//     m = (float)(MAX_SERVO - MIN_SERVO) / (float)(MIN_POT - MAX_POT);
//     int16_t datax, datay, dataz;
//     float X_out, Y_out, Z_out, roll, pitch, rollF = 0, pitchF = 0;
//     b = (float)(MAX_SERVO - m * MIN_POT);

//     DDRB |= (1 << PB1); // pin 9 arduino - servo

//     init_tc1();

//     printf_init();

//     i2c_init(100000);
//     ADXL345_init(RANGE_2G, DATA_RATE_100);

//     // offsets
//     // datax = 17
//     offx = (-17 / 4);
//     printf("%d\n", offx);
//     i2c_write(I2C_ADDR, 1, OFSX, &offx);

//     offy = (23 / 4);
//     printf("%d\n", offy);
//     i2c_write(I2C_ADDR, 1, OFSY, &offy);

//     while (1)
//     {
//         datax = read_Xdata();
//         X_out = (float)datax / 256.0;
//         datay = read_Ydata();
//         Y_out = (float)datay / 256.0;
//         dataz = read_Zdata();
//         Z_out = (float)dataz / 256.0;
//         // roll and pitch

//         roll = atan2(Y_out, Z_out) * 180 / PI;
//         pitch = atan2((-X_out), sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
//         if (roll < 0)
//         {
//             roll = roll + 360;
//         }

//         printf("%f\t", roll);
        
//         if (roll > MAX_POT && roll < 300)
//         {
//             roll = MAX_POT;
//             roll_ant = roll;
//         }
//         else if (roll < MIN_POT || roll >= 300)
//         {
//             roll = MIN_POT;
//             roll_ant = roll;
//         }
//         else if (fabs(roll - roll_ant) < 1)
//         {
//             roll = roll_ant;
//         }
//         else
//         {
//             roll_ant = roll;
//         }
//         printf("%f\t", fabs(roll - roll_ant) );
//         printf("%f\t", roll_ant);
//         printf("%f\n", roll);

//         servo_pos = m * roll + b;
//         servo_posF = 0.2 * servo_posF + 0.8 * servo_pos;
//         OCR1A = servo_posF; // 180 - um bocado por tentativa erro
//         _delay_ms(200);
//     }
// }
