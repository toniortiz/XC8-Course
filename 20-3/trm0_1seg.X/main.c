#include <xc.h>
#include "fuses.h"
#include <stdint.h>

#define _XTAL_FREQ 8000000;

typedef union {
    uint16_t lt;

    struct {
        unsigned L : 8;
        unsigned H : 8;
    };
} Timers_t;

uint16_t t0 = 3036;
uint16_t t01 = 34286;
uint8_t flag = 1;
Timers_t timer0reg;

void int_timer0() {
    if (INTCONbits.TMR0IF == 1) {
        LATBbits.LB0 = ~LATBbits.LB0;

        if (flag == 1) {
            timer0reg.lt = t0;
            TMR0L = timer0reg.L;
            TMR0H = timer0reg.H;
            flag = 0;
        } else {
            timer0reg.lt = t01;
            TMR0L = timer0reg.L;
            TMR0H = timer0reg.H;
            flag = 1;
        }

        INTCONbits.TMR0IF = 0;
    }
}

void __interrupt() interrupts() {
    int_timer0();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISBbits.RB0 = 0;
    LATBbits.LB0 = 0;

    timer0reg.lt = t0;

    // Configurar timer 0 para que se desborde cada 1 s
    T0CONbits.TMR0ON = 0; // Apaga timer 0
    T0CONbits.T08BIT = 0; // modo de 16 bits
    T0CONbits.T0CS = 0; // configurada como temporizador
    T0CONbits.T0SE = 0; // no importa
    T0CONbits.PSA = 0; // Asignacion de prescaler
    T0CONbits.T0PS = 0b100; // Pescaler de 32
    TMR0L = timer0reg.L;
    TMR0H = timer0reg.H;

    T0CONbits.TMR0ON = 1; // enciende timer 0

    INTCONbits.TMR0IE = 1; // habilitar int del timer 0
    INTCONbits.TMR0IF = 0; // limpiar bandera de int del timer 0

    INTCONbits.GIE = 1; // habilitar todas las ints
    INTCONbits.PEIE = 1; // habilita int perifericas

    while (1) {

    }

    return;
}
