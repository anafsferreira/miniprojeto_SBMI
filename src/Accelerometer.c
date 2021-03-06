
// #include <avr/io.h>
// #include "i2c.h"

// #include <util/delay.h>
// #include <math.h>
// #include <avr/interrupt.h>
// #include "serial_printf.h"

// #define PI 3.14159265358979323846
// #define RANGE_2G            0b00001000 
// #define MEASURE             0x08 
// #define DATA_RATE_100       0x0A
// #define I2C_ADDR            0x53
// #define DEVID               0x00
// #define POWER_CTL           0x2D
// #define BW_RATE             0x2C
// #define DATA_FORMAT         0x31
// #define DATAX               0x32
// #define DATAX0              0x32 // X-Axis Data 0
// #define DATAX1              0x33 // X-Axis Data 1
// #define DATAY               0x34
// #define DATAY0              0x34 // Y-Axis Data 0
// #define DATAY1              0x35 // Y-Axis Data 1
// #define DATAZ               0x36
// #define DATAZ0              0x36 // Z-Axis Data 0
// #define DATAZ1              0x37 // Z-Axis Data 1
// #define OFSX                0x1E
// #define OFSY                0x1F
// #define OFSZ                0x20


// uint8_t ADXL345_ID;
// uint8_t lsb, msb;
// uint16_t x_acc1, y_acc1, z_acc1;

// void ADXL345_init(uint8_t range, uint8_t data_rate)
// {
//     uint8_t var = 0;
//     i2c_read(I2C_ADDR, 1, DEVID, &ADXL345_ID);
//     printf("Device ID=%d\n\r", ADXL345_ID);
//     i2c_write(I2C_ADDR, 1, POWER_CTL, &var);
//     i2c_write(I2C_ADDR, 1, BW_RATE, &data_rate);
//     i2c_write(I2C_ADDR, 1, DATA_FORMAT, &range);
//     var = MEASURE;
//     i2c_write(I2C_ADDR, 1, POWER_CTL, &var);
//     _delay_ms(20);
// }

// uint16_t read_Xdata(void){

//     int16_t data;
//     uint8_t buf[2];

//     i2c_read(I2C_ADDR, 2, DATAX, buf);
//     data = (int16_t)buf[1]<<8;
//     data += (int16_t)buf[0];
//     return(data);
// }

// uint16_t read_Ydata(void){

//     int16_t data;
//     uint8_t buf[2];

//     i2c_read(I2C_ADDR, 2, DATAY, buf);
//     data = (int16_t)buf[1]<<8;
//     data += (int16_t)buf[0];
//     return(data);
// }

// uint16_t read_Zdata(void){

//     int16_t data;
//     uint8_t buf[2];

//     i2c_read(I2C_ADDR, 2, DATAZ, buf);
//     data = (int16_t)buf[1]<<8;
//     data += (int16_t)buf[0];
//     return(data);
// }

// int main(void)
// {
    
//     int16_t datax, datay, dataz;
//     float X_out, Y_out, Z_out, roll, pitch, rollF = 0, pitchF = 0;
//     int8_t offx, offy, offz;
//     printf_init();
//     i2c_init(100000);
//     ADXL345_init(RANGE_2G, DATA_RATE_100);

//     // offsets
//     // datax = 17
//     offx= (-17/4);
//     printf("%d\n",offx);
//     i2c_write(I2C_ADDR, 1, OFSX, &offx);

//     offy= (23/4);
//     printf("%d\n",offy);
//     i2c_write(I2C_ADDR, 1, OFSY, &offy);


//     while(1){
//         datax = read_Xdata();
//         X_out = (float)datax/256.0;
//         datay = read_Ydata();
//         Y_out = (float)datay/256.0;
//         dataz = read_Zdata();
//         Z_out = (float)dataz/256.0;

         
//         // roll and pitch
        
//         roll = atan2(Y_out , Z_out) * 180 / PI;
//         pitch = atan2((- X_out) , sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
//         //printf("%d\t %d\t %d\n", datax, datay, dataz);
//         //printf("%f\t %f\t %f\n", X_out, Y_out, Z_out);
       

        
//         // Low-pass filter
//         rollF = 0.94 * rollF + 0.06 * roll;
//         pitchF = 0.94 * pitchF + 0.06 * pitch;

//          printf("RollF: %f\t PitchF: %f\n", rollF, pitchF);
//         _delay_ms(200);
//     }
// }
