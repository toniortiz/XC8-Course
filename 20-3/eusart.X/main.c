#include <xc.h>
#include "fuses.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 8000000

uint8_t i = 0;
uint8_t buffer_rx[4] = {0, 0, 0, 0};

void putcUSART(const unsigned char caracter) {
    while (!TXSTAbits.TRMT);
    TXREG = caracter;
}

void putsUSART(const char *cadena) {
    while (*cadena != 0) {
        putcUSART(*cadena);
        cadena++;
    }
}

void rx_int() {
    if (PIR1bits.RCIF == 1) {
        buffer_rx[i] = RCREG;

        // @ A_0 A_1 #
        // caracter de inicio = @
        // caracter de fin #
        if (buffer_rx[0] == 64 && buffer_rx[3] == 35) {
            LATAbits.LA0 = buffer_rx[1];
            LATAbits.LA1 = buffer_rx[2];

            i = 0;
            buffer_rx[0] = 0;
            buffer_rx[3] = 0;
        } else {
            i++;
        }

        PIR1bits.RCIF = 0;
    }
}

void __interrupt() interrupts() {
    rx_int();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;

    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;
    LATAbits.LA0 = 0;
    LATAbits.LA1 = 0;

    TXSTAbits.TRMT = 1;
    TXSTAbits.BRGH = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.TX9 = 0;
    RCSTAbits.CREN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.SPEN = 1;
    BAUDCONbits.BRG16 = 0;
    SPBRG = 51;

    // habilitar int de recepción
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    float temp = 1.4;

    while (1) {
        char cadena [20];
        sprintf(cadena, "@%d/%d/%.1f\r\n", LATAbits.LA0, LATAbits.LA1, temp);
        putsUSART(cadena);
        temp += 1.2;
        __delay_ms(1000);
    }

    return;
}
