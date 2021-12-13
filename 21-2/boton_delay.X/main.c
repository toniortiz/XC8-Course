#include <xc.h> 
#include "config.h"

#define _XTAL_FREQ 8000000

//!! regla de oro: siempre limpiar las banderas de interrupción

void int_ext_0() {
    if (INTCONbits.INT0IF == 1) {
        LATAbits.LA1 = 1;

        INTCONbits.INT0IF = 0; // limpiar la bandera
    }
}

void int_ext_1() {
    if (INTCON3bits.INT1IF == 1) {
        LATAbits.LA1 = 0;

        INTCON3bits.INT1IF = 0; // limpiar bandera
    }
}

void int_ext_2() {
    if (INTCON3bits.INT2IF == 1) {
        LATAbits.LA1 = 1;
        INTCON3bits.INT2IF = 0;
    }
}

// vector de interrupciones

void __interrupt() interrupts() {
    int_ext_0();

    int_ext_1();

    int_ext_2();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    // botttones conetados a la interreupcion externa 0 RB0 y externa 1 RB1
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;


    // leds
    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;
    LATAbits.LA0 = 0; //apagar led
    LATAbits.LA1 = 0; // apagar led

    // Habilita interrupciones
    INTCONbits.GIE = 1;

    // Configuracion de interrupcion externa 0
    INTCONbits.INT0IE = 1; // habilita int ext 0
    INTCON2bits.INTEDG0 = 1; // flanco de subida, 0 para bajada

    // Configuracion int ext 1
    INTCON3bits.INT1IE = 1; // habilita int ext 1
    INTCON2bits.INTEDG1 = 0; // flanco de bajada

    // configuracion int ext 2
    INTCON3bits.INT2IE = 1;
    INTCON2bits.INTEDG2 = 1;


    while (1) {
        __delay_ms(2000);
        LATAbits.LATA0 = ~LATAbits.LATA0;

    }

    return;
}
