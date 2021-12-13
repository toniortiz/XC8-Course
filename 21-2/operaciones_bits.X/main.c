#include <xc.h>
#include "config.h"
#include <stdbool.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000

void corrimiento_izq(void);
void corrimiento_der(void);

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    // Definir puerto B como salida digital
    TRISB = 0;

    // Puerto como salida digital
    TRISD = 0;

    // RA0 como entrada digital
    TRISAbits.RA0 = 1;

    // Valor inicial al puerto B
    // Encender leds desde RB0 hasta RB3 y apagar leds desde RB4 hasta RGB7
    LATB = 0b00001111;

    // Valor inicial al puerto D
    // Solo se va a encender el pin RD0
    LATD = 0b00000001;

    uint8_t mask = 0b11111111;
    bool sentido = false; // false -> izquierda ---- true -> derecha

    while (1) {
        __delay_ms(1000);

        // operacion xor
        LATB = LATB^mask;

        if (PORTAbits.RA0 == 1) {
            sentido = ~sentido;
            while (PORTAbits.RA0 == 1) {
                __delay_ms(5);
            }
        }

        if (sentido == false) {
            // corrimiento de 1 bit hacia la izquierda
            corrimiento_izq();
        } else if (sentido == true) {
            // corrimiento hacia la derecha
            corrimiento_der();
        }
    }

    return;
}

void corrimiento_izq(void) {
    if (LATD == 0b10000000) {
        LATD = 0b00000001;
    } else {
        LATD = LATD << 1;
    }
}

void corrimiento_der(void) {
    if (LATD == 0b00000001) {
        LATD = 0b10000000;
    } else {
        LATD = LATD >> 1;
    }
}