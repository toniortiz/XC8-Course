#include <xc.h>
#include "fuses.h"
#include <stdint.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000

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
        // Leer datos del puerto serial
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
    LATAbits.LA1 = 1;

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

    int contador = 0;

    // @led/motor/temp/humd\r\n

    float temp = 32.3;

    while (1) {
        LATAbits.LA0 = ~LATAbits.LA0;
        LATAbits.LA1 = ~LATAbits.LA1;
        char cadena [15];
        sprintf(cadena, "@%d/%d/%.1f\r\n", LATAbits.LA0, LATAbits.LA1, temp);
        putsUSART(cadena);
        __delay_ms(1000);
    }

    return;
}
