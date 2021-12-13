#include <xc.h>
#include "config.h"
#include "lcd.h"

#define _XTAL_FREQ 8000000

void int_timer0() {
    if (INTCONbits.TMR0IF == 1) {
        LATAbits.LA0 = ~LATAbits.LA0;

        TMR0L = 0xCB;
        TMR0H = 0xF3;
        INTCONbits.TMR0IF = 0; // limpiar la bandera de int
    }
}

void __interrupt() interrupts() {
    int_timer0();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISAbits.RA0 = 0; // salida, led
    LATAbits.LA0 = 0;
    
    TRISAbits.RA1 = 0;
    LATAbits.LA1 = 0;

    // Configuracion timer0 a 10 Hz
    T0CONbits.TMR0ON = 0; // Deshabilita timer0
    T0CONbits.T08BIT = 0; // 16 bits
    T0CONbits.T0CS = 0; // configuracion como timer
    T0CONbits.PSA = 0; // Usar prescaler
    T0CONbits.T0PS = 0b100; // Prescaler de 32
    TMR0L = 0xCB;
    TMR0H = 0xF3;

    // Configurar interrupción del timer0
    INTCONbits.TMR0IE = 1; // habilitar int timer0
    INTCONbits.TMR0IF = 0; // limpiar la bandera

    T0CONbits.TMR0ON = 1; // Enciende timer0

    INTCONbits.GIE = 1; // Habilito todas las interrupciones
    INTCONbits.PEIE = 1; //  habilita interrupciones perifericas


    while (1) {
        __delay_ms(3000);
        LATAbits.LA1 = ~LATAbits.LA1;
               
    }
    return;
}
