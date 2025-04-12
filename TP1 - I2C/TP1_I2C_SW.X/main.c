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

void I2C_Init(int);
void Start_Bit(void);
void Repeated_Start(void);
void Send_Byte_Data(uint8_t);
uint8_t Receive_Byte_Data(void);
void Send_ACK_Bit(void);
void Send_NACK_Bit(void);
void Stop_Bit(void);

void main(void) {
    TRISCbits.TRISC3 = 1;               // SCL as input
    TRISCbits.TRISC4 = 1;               // SDA as input

    int clock_init_value = 49;          // For 100kHz baud-rate
    I2C_Init(clock_init_value);

    uint8_t set_data = 0x00;
    uint8_t get_data = 0x00;

    while (1) {
        
    }

    return;
}

void I2C_Init(int clock_init_value) {
    SSPADD = (unsigned char)clock_init_value;
    SSPCON = 0x28;
}

void Start_Bit() {
    SSPCON2bits.SEN = 1;
    while (SSPCON2bits.SEN);            // Cleared automatically
    PIR1bits.SSPIF = 0;
}

void Repeated_Start() {
    SSPCON2bits.RSEN = 1;
    while (SSPCON2bits.RSEN);           // Cleared automatically
    PIR1bits.SSPIF = 0;
}

void Send_Byte_Data(uint8_t data) {
    SSPBUF = data;
    while (!PIR1bits.SSPIF);            // Set automatically
    PIR1bits.SSPIF = 0;

    if (SSPCON2bits.ACKSTAT) {          // No ACK
        Stop_Bit();
    }
}

uint8_t Receive_Byte_Data() {
    SSPCON2bits.RCEN = 1;
    while (!SSPSTATbits.BF);            // Set automatically
    return SSPBUF;
}

void Send_ACK_Bit() {
    SSPCON2bits.ACKDT = 0;
    SSPCON2bits.ACKEN = 1;
    while (SSPCON2bits.ACKEN);          // Cleared automatically
}

void Send_NACK_Bit() {
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
    while (SSPCON2bits.ACKEN);          // Cleared automatically
}

void Stop_Bit() {
    SSPCON2bits.PEN = 1;
    while (SSPCON2bits.PEN);            // Cleared automatically
    PIR1bits.SSPIF = 0;
}
