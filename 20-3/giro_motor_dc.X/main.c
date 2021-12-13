#include <xc.h>
#include "fuses.h"

#define _XTAL_FREQ 8000000

void int_ext0();
void int_ext1();
void int_ext2();

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    // Pines de salida para el IN1 e IN2
    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;

    // INT0
    TRISBbits.RB0 = 1;
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 0;
    INTCON2bits.INTEDG0 = 1;

    // INT1
    TRISBbits.RB1 = 1;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1IF = 0;
    INTCON2bits.INTEDG1 = 1;

    // INT2
    TRISBbits.RB2 = 1;
    INTCON3bits.INT2IE = 1;
    INTCON3bits.INT2IF = 0;
    INTCON2bits.INTEDG2 = 1;

    INTCONbits.GIE = 1;

    while (1) {

    }

    return;
}

void int_ext0() {
    // Hace girar al motor en sentido horario
    if (INTCONbits.INT0IF == 1) {
        // detener motor
        LATAbits.LA0 = 0;
        LATAbits.LA1 = 0;
        __delay_ms(500);
        
        // cambio de giro
        LATAbits.LA0 = 1;
        LATAbits.LA1 = 0;

        INTCONbits.INT0IF = 0;
    }
}

void int_ext1() {
    // Hace girar el motor en sentido anti-horario
    if (INTCON3bits.INT1IF == 1) {
        // detener motor
        LATAbits.LA0 = 0;
        LATAbits.LA1 = 0;
        __delay_ms(500);
        
        // cambio de giro
        LATAbits.LA0 = 0;
        LATAbits.LA1 = 1;

        INTCON3bits.INT1IF = 0;
    }
}

void int_ext2() {
    // Detiene el motor
    if (INTCON3bits.INT2IF == 1) {
        LATAbits.LA0 = 0;
        LATAbits.LA1 = 0;

        INTCON3bits.INT2IF = 0;
    }
}

void __interrupt() interrupts() {
    int_ext0();

    int_ext1();

    int_ext2();
}
