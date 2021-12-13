#include <xc.h>
#include "fuses.h"

#define _XTAL_FREQ 8000000

void int_ext_0() {
    if (INTCONbits.INT0IF) {
        // LATBbits.LB3 = ~LATBbits.LB3;
        LATBbits.LB3 = 1;

        INTCONbits.INT0IF = 0; // limpiar bandera
    }
}

void int_ext_1() {
    if (INTCON3bits.INT1IF) {
        LATBbits.LB3 = 0;

        INTCON3bits.INT1IF = 0; // limpiar bandera
    }
}

void int_ext_2() {
    if (INTCON3bits.INT2IF) {
        LATBbits.LB3 = 0;

        INTCON3bits.INT2IF = 0; // limpiar la bandera
    }
}

void __interrupt() interrupts() {
    int_ext_0();

    int_ext_1();

    int_ext_2();
}

void main(void) {
    // Configuracion oscilador interno a 8MHz
    OSCCON = 0x76;

    // Configuracion todos los pines I/O como digitales
    ADCON1 = 0x0F;

    TRISBbits.RB0 = 1; // RB0  como entrada digital
    TRISBbits.RB1 = 1; // RB1 como entrada digital
    TRISBbits.RB2 = 1; // RB2 como entrada digital
    TRISBbits.RB3 = 0; // RB3 como salida digital
    LATBbits.LB3 = 0; // limpio RB3

    INTCONbits.GIE = 1; // habilita interrupciones

    // Configuraion de INT0 (interrupcion externa 0)
    INTCONbits.INT0IE = 1; // habilita int ext 0
    INTCON2bits.INTEDG0 = 1; // flanco de subida 0 -> 1

    // Configuracion de INT1 (interrupcion externa 1)
    INTCON3bits.INT1IE = 1; // habilitar int ext 1
    INTCON2bits.INTEDG1 = 0; // flanco de bajada 1 -> 0

    // Configuracion de INT2 (interrupcion externa 2)
    INTCON3bits.INT2IE = 1; // habilitar in ext 2
    INTCON2bits.INTEDG2 = 1; // flanco de subida

    while (1) {

    }

    return;
}
