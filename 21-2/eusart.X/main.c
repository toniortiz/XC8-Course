#include <xc.h>
#include "config.h"
#include <inttypes.h>
#include <string.h> 
#include <stdio.h>

volatile uint8_t buf_rx[4] = {0, 0, 0, 0};
volatile uint8_t i = 0;

void putcUSART(const unsigned char caracter){
    while(!TXSTAbits.TRMT);
    TXREG = caracter;
}

void putsUSART(const char* cadena){
    while(*cadena != 0){
        putcUSART(*cadena);
        cadena++;
    }
}

void int_rx() {
    if (PIR1bits.RCIF == 1) {
        buf_rx[i] = RCREG;

        if (buf_rx[0] == '@' && buf_rx[3] == '#') {
            LATAbits.LA0 = buf_rx[1];
            LATAbits.LA1 = buf_rx[2];

            i = 0;
            buf_rx[0] = 0;
            buf_rx[3] = 0;
            //memset(buf_rx,0,sizeof(buf_rx));
        } else {
            i++;
        }

        PIR1bits.RCIF = 0;
    }
}

void __interrupt() interrupts() {
    int_rx();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    LATAbits.LA0 = 1;
    LATAbits.LA1 = 0;
    

    // Configuracion EUSART
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;

    TXSTAbits.BRGH = 1;
    TXSTAbits.SYNC = 0; // modo asincrono
    TXSTAbits.TXEN = 1;
    TXSTAbits.TX9 = 0; // 8 bits de transmision
    RCSTAbits.CREN = 1;
    RCSTAbits.RX9 = 0; // recepción de 8 bits
    RCSTAbits.SPEN = 1; // habilitar puerto serial
    BAUDCONbits.BRG16 = 0; // BRG en modo de 8 bits
    SPBRG = 51; // Tabla 20-3

    // habilitar int de recepcion
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    float temperatura = 0.0;

    while (1) {
        char cadena[20];
        sprintf(cadena, "@%d/%d/%.1f\r\n", LATAbits.LA0, LATAbits.LA1, temperatura);
        putsUSART(cadena);
        
        temperatura += 1.1;
        __delay_ms(1000);
    }
    return;
}
