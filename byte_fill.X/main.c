#include <xc.h>
#include "fuses.h"
#include <stdint.h>

// Macro usada por las funciones delay.
#define _XTAL_FREQ 8000000

// Logica para el llenado hacia la derecha

uint8_t value = 0b10000000;

// value = 11111111; valor maximo que puede tomar

void llenado_derecha() {
    // Manera larga, tediosa y propensa a equivocarme
    //    LATB = 0b10000000;
    //    __delay_ms(100);
    //
    //    LATB = 0b11000000;
    //    __delay_ms(100);
    //
    //    LATB = 0b11100000;
    //    __delay_ms(100);
    //
    //    LATB = 0b11110000;
    //    __delay_ms(100);
    //
    //    LATB = 0b11111000;
    //    __delay_ms(100);
    //
    //    LATB = 0b11111100;
    //    __delay_ms(100);
    //
    //    LATB = 0b11111110;
    //    __delay_ms(100);
    //
    //    LATB = 0b11111111;
    //    __delay_ms(100);

    // Manera inteligente
    LATB = value;

    if (value == 0b11111111) {
        value = 0b10000000;
    } else {
        value = value | (value >> 1);
    }

    __delay_ms(100);

}

uint8_t salida00 = 0b10000000;
uint8_t salida01 = 0b00000001;

void corrimiento_izq_der() {
    salida01 = 0b00000001;

    LATB = salida00;

    if (salida00 == 0b00000001) {
        salida00 = 0b10000000;
    } else {
        salida00 >>= 1;
    }

    __delay_ms(100);
}

void corrimiento_der_izq() {
    salida00 = 0b10000000;

    LATB = salida01;

    if (salida01 == 0b10000000) {
        salida01 = 0b00000001;
    } else {
        salida01 <<= 1;
    }

    __delay_ms(100);
}

void main(void) {
    // Configuracion del oscilador interno a 8 MHz
    OSCCON = 0x76;

    // Configuracion de todos los pines I/O como digitales
    ADCON1 = 0x0F;

    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;

    TRISB = 0x00; // puerto B como salida digital
    LATB = 0x00; // todos los bits del puerto son 0

    while (1) {
        // Si RA0 y RA1 son iguales a 0, hacer corrimiento de izq a der
        if (PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0) {
            corrimiento_izq_der();
        }

        // Si RA0 == 0 y RA1 == 1, corremiento de der a izq
        if (PORTAbits.RA0 == 0 && PORTAbits.RA1 == 1) {
            corrimiento_der_izq();
        }

        // Si RA1 == 1 y RA0 == 0, llenado izq a der
        if (PORTAbits.RA1 == 1 && PORTAbits.RA0 == 0) {
            // llenado_izq_der();
        }

        // Si RA1 == 1 y RA0 == 1, llenado der a izq
        if (PORTAbits.RA1 == 1 && PORTAbits.RA0 == 1) {
            // llenado_der_izq();
        }
    }

    return;
}
