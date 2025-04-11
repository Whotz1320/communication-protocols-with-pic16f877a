#include <xc.h>

#define _XTAL_FREQ 20000000

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = ON
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void I2C_Init(int clock_init_value) {
    SSPADD = (unsigned char)clock_init_value;
    SSPCON = 0x28;
}

void Start_Bit() {
    SSPCON2bits.SEN = 1;
    while (SSPCON2bits.SEN);
    PIR1bits.SSPIF = 0;
}

void main(void) {
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    PIR1bits.SSPIF = 0;

    TRISCbits.TRISC3 = 0;       /* SCL LINE AS OUTPUT */
    TRISCbits.TRISC4 = 0;       /* SDA LINE AS OUTPUT */

    int clock_init_value = 49;
    I2C_Init(clock_init_value);

    while (1) {
        Start_Bit();            /* SEND THE START BIT CONDITION */
        __delay_ms(20);
    }

    return;
}
