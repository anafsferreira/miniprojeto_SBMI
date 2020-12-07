// /*******************************************************
// Chip type               : ATmega328P
// Program type            : Application
// AVR Core Clock frequency: 16.000000 MHz
// Memory model            : Small
// External RAM size       : 0
// Data Stack size         : 512
// *******************************************************/

// //#include <io.h>
// // #include <mega328p.h>
// #include <avr/io.h>
// #include <util/delay.h>
// #include <math.h>
// #include <avr/interrupt.h>
// #include "serial_printf.h"

// //Register addresses for ADXL345

// #define RANGE_16G           0x0B
// #define MEASURE             0x08
// #define DEVID               0x00 // Device ID
// #define THRESH_TAP          0x1D // Tap threshold
// #define OFSX                0x1E // X-axis offset
// #define OFSY                0x1F // Y-axis offset
// #define OFSZ                0x20 // Z-axis offset
// #define DUR                 0x21 // Tap duration
// #define LATENT              0x22 // Tap latency
// #define WINDOW              0x23 // Tap window
// #define THRESH_ACT          0x24 // Activity threshold
// #define THRESH_INACT        0x25 // Inactivity threshold
// #define TIME_INACT          0x26 // Inactivity time
// #define ACT_INACT_CTL       0x27 // Axis enable control for activity and inactivity detection
// #define THRESH_FF           0x28 // Free-fall threshold
// #define TIME_FF             0x29 // Free-fall time
// #define TAP_AXES            0x2A // Axis control for single tap/double tap
// #define ACT_TAP_STATUS      0x2B // Source of single tap/double tap
// #define BW_RATE             0x2C // Data rate and power mode control
// #define POWER_CTL           0x2D  //Power saving Control
// #define INT_ENABLE          0x2E  // interrupt enable control
// #define INT_MAP             0x2F   //interrupt mapping control
// #define INT_SOURCE          0x30 //source of interrupts
// #define DATA_FORMAT         0x31//Data Format
// #define DATAX0              0x32 // X-Axis Data 0
// #define DATAX1              0x33 // X-Axis Data 1
// #define DATAY0              0x34 // Y-Axis Data 0
// #define DATAY1              0x35 // Y-Axis Data 1
// #define DATAZ0              0x36 // Z-Axis Data 0
// #define DATAZ1              0x37 // Z-Axis Data 1
// #define FIFO_CTL            0x38 //Fifo control
// #define FIFO_STATUS         0x39 //Fifo status

// //
// // Expresed as output data rate
// #define DATA_RATE_3200            0x0F
// #define DATA_RATE_1600            0x0E
// #define DATA_RATE_800            0x0D
// #define DATA_RATE_400            0x0C
// #define DATA_RATE_200            0x0B
// #define DATA_RATE_100            0x0A
// #define DATA_RATE_50            0x09
// #define DATA_RATE_25            0x08

// #define W 0x00
// #define R 0x80



// //Function prototypes
// void ADXL345_init(char range, char data_rate);
// void ADXL345_write(unsigned char addr,unsigned char data);
// unsigned char ADXL345_read(unsigned char addr);
// void read_accelerometer_1(void);
// void spi_init(void);

// // Declare your global variables here
// uint8_t ADXL345_ID;
// unsigned char lsb, msb;
// signed int x_acc1, y_acc1, z_acc1;

// int main(void)
// {

//     // Crystal Oscillator division factor: 2 (8MHz)
//         // #pragma optsize-
//         // CLKPR=0x80;
//         // CLKPR=0x01;
//         // #ifdef _OPTIMIZE_SIZE_
//         // #pragma optsize+
//         // #endif

//     // Input/Output Ports initialization
//     // SCK(PB5):Output, MISO(PB4):Input, MOSI(PB3):Output, SS(PB2):Output
//     PORTB=0x04;     // 0b 0000 0100   SS set to 1
//     DDRB=0x2C;      // 0b 0010 1100   SS , SCK , MOSI and as outputs      


//     // USART initialization
//         // Communication Parameters: 8 Data, 1 Stop, No Parity
//         // USART Receiver: On
//         // USART Transmitter: On
//         // USART0 Mode: Asynchronous
//         // USART Baud Rate: 2 Mbps (Double Speed Mode)
//         // UCSR0A=0x02;
//         // UCSR0B=0x18;
//         // UCSR0C=0x06;
//         // UBRR0H=0x00;
//         // UBRR0L=0x00;
//     printf_init();
      
//     spi_init();


//     ADXL345_init(RANGE_16G, DATA_RATE_100);

//     // while(1)
//     // {
//     //     read_accelerometer_1();

//     //   //    
//     // //   printf("%d,%d,%d", x_acc1, y_acc1, z_acc1);
//     // //   printf(",__,");    

//     // }
//     return 0;
// }

// //spi initialization

// void spi_init(void)

// {

//     PORTB=0x04;     
//     DDRB=0x2C;

//     SPCR=(0<<SPIE) | (1<<SPE) | (0<<DORD) | (1<<MSTR) | (1<<CPOL) | (1<<CPHA) | (0<<SPR1) | (0<<SPR0);
//     SPSR=(0<<SPI2X);

// }

// //Accelerometer functions

// void ADXL345_init(char range, char data_rate)
// {
//     ADXL345_ID=ADXL345_read(0);
//     printf("Device ID=%d\n\r", ADXL345_ID);
//     ADXL345_write(POWER_CTL,0x00); //Standby mode
//     ADXL345_write(BW_RATE, data_rate);          //Setup the device bandwidth and output data rate
//     ADXL345_write(DATA_FORMAT, range);          //Setup data
//     ADXL345_write(POWER_CTL, MEASURE);     //Measurement mode
//     _delay_ms(20);
// }

// void ADXL345_write(unsigned char addr,unsigned char data)   //write to the ADXL345
// {
//     PORTB &= ~0b00000100;       //    

//     SPDR = W|addr;                //

//     while (!(SPSR & 0x80));      //

//     SPDR=data;                  //

//     PORTB |= 0b00000100;        //   

//     _delay_ms(10);
// }   

// unsigned char ADXL345_read( uint8_t addr)
// // read from the ADXL345

// {
//     uint8_t ans;

//     PORTB &= ~0b00000100;

//     SPDR = R|addr;                       
//     while(!(SPSR & 0x80));              
//     ans=SPDR;                                 
//     PORTB |= 0b00000100;                       
//     return (ans);
// }

// void read_accelerometer_1(void)
// {
// // unsigned char x_raw, y_raw, z_raw;
// //sensitivity

//         lsb=ADXL345_read(DATAX0);
//         msb=ADXL345_read(DATAX1);  
//         x_acc1=((msb<<8) | lsb);     // x_raw=((msb<<8) | lsb);

//         //x_acc=x_raw/sensitivity;

//         lsb=ADXL345_read(DATAY0);
//         msb=ADXL345_read(DATAY1);  
//         y_acc1=((msb<<8) | lsb);    // y_raw=((msb<<8) | lsb);

//         //  y_acc=y_raw/sensitivity;

//         lsb=ADXL345_read(DATAZ0);
//         msb=ADXL345_read(DATAZ1);     
//         z_acc1=((msb<<8) | lsb);    //        z_raw=((msb<<8) | lsb);

//         //        z_acc=z_raw/sensitivity;      
// }   
