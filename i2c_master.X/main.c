#include <xc.h>
#include "fuses.h"
#include <stdint.h>

#define _XTAL_FREQ 8000000

void mi2c_init(const uint24_t clock) {
    SSPCON1 = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ / (4 * clock)) - 1;
    SSPSTAT = 0;
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
}

void mi2c_wait() {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}

void mi2c_start() {
    mi2c_wait();
    SSPCON2bits.SEN = 1;
}

void mi2c_repeated_start() {
    mi2c_wait();
    SSPCON2bits.RSEN = 1;
}

void mi2c_stop() {
    mi2c_wait();
    SSPCON2bits.PEN = 1;
}

uint8_t mi2c_write(uint8_t data) {
    mi2c_wait();
    SSPBUF = data;
    
    return SSPCON2bits.ACKSTAT;
}

uint8_t mi2c_read(uint8_t a) {
    mi2c_wait();
    SSPCON2bits.RCEN = 1;
    mi2c_wait();
    uint8_t data = SSPBUF;
    mi2c_wait();
    SSPCON2bits.ACKDT = (a) ? 0 : 1;
    SSPCON2bits.ACKEN = 1;

    return data;
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;


    while (1) {

    }
}
