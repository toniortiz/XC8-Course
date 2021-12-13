#include <xc.h>
#include <stdint.h>
#include <pic18f4550.h>
#include "fuses.h"

#define _XTAL_FREQ 8000000

typedef union {
    uint16_t lt;

    struct {
        unsigned L : 8;
        unsigned H : 8;
    };
} timers_t;

typedef enum {
    A = 0, B, C, D
} digit_t;


timers_t timer_reg;
volatile digit_t next = A;
volatile uint16_t adc_val = 0;
volatile uint8_t a, b, c, d;

void set_value(uint8_t val, uint8_t pos) {
    LATB = (LATB & 0xCF) | pos;
    LATB = (LATB & 0xF0) | val;
}

void timer0_int() {
    if (INTCONbits.TMR0IF == 1) {

        switch (next) {
            case A:
                set_value(a, 0x00);
                next = B;
                break;

            case B:
                set_value(b, 0b00010000);
                next = C;
                break;
            case C:
                set_value(c, 0b00100000);
                next = D;
                break;
            case D:
                set_value(d, 0b00110000);
                next = A;
                break;
            default: break;
        }

        TMR0H = timer_reg.H;
        TMR0L = timer_reg.L;
        INTCONbits.TMR0IF = 0;
    }
}

void adc_int() {
    if (PIR1bits.ADIF == 1) {
        adc_val = (((uint16_t) ADRESH) << 8) | (ADRESL);

        a = adc_val / 1000; // extraer milesimas
        b = ((adc_val / 100) % 10); // extraer centenas
        c = ((adc_val / 10) % 10); // extraer decenas
        d = (adc_val % 10); // extreaer unidades

        __delay_us(20);
        ADCON0bits.GODONE = 1;
        PIR1bits.ADIF = 0;
    }
}

void __interrupt() interrupts() {
    adc_int();

    timer0_int();
}

void main(void) {
    OSCCON = 0x76;

    TRISB = 0b11000000;

    // Configuracion canal analogico 0
    TRISAbits.RA0 = 1;
    ADCON1bits.PCFG = 0b1110; // AN0 analogico todos los demas digitales
    ADCON1bits.VCFG = 0b00; // 0-5 v
    ADCON0bits.CHS = 0b0000; // seleccionar AN0
    ADCON2bits.ACQT = 0b010; // 4 TAD
    ADCON2bits.ADCS = 0b001; // FOSC/8
    ADCON2bits.ADFM = 1; // resultado justificado a la derecha
    ADCON0bits.ADON = 1; // enciende ADC
    PIR1bits.ADIF = 0; // limpiar bandera de int
    PIE1bits.ADIE = 1; // habilitar int de adc

    // Configuracion timer 0
    timer_reg.lt = 45536;

    // configurar timer 0 para que se desborde cada 10 ms
    T0CON = 0b00001000;
    TMR0H = 0; // Reset Timer0 to 0x0000
    TMR0L = 0;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    T0CONbits.TMR0ON = 1;
    TMR0H = timer_reg.H;
    TMR0L = timer_reg.L;

    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    ADCON0bits.GODONE = 1; // empieza la conversion del adc

    while (1) {
    }
}
