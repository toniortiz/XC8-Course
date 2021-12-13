#include <xc.h>
#include "fuses.h"
#include <stdint.h>

// Definimos esta macro para poder usar funciones de delay de XC8
#define _XTAL_FREQ 8000000

void _delay_(uint16_t d) {
    int i;
    for (i = 0; i < d; i++) {
        __delay_ms(1);
    }
}

void main(void) {
    // Configuracion del oscilador interno a 8MHz
    OSCCON = 0x76;

    // Configuracion de los pines como I/O digitales
    ADCON1 = 0x0F;

    // Pin RA0 y RA1 como entradas digitales
    TRISAbits.RA0 = 1; // aumenta el delay
    TRISAbits.RA1 = 1; // decrementa el delay

    // Pin RA2 como salida digital
    TRISAbits.RA2 = 0;

    LATAbits.LA2 = 0; // limpiar bit

    uint16_t delay = 100;

    while (1) {
        if (PORTAbits.RA0 == 1) {
            delay = delay + 1;
        }

        if (PORTAbits.RA1 == 1) {
            delay = delay - 1;
        }

        LATAbits.LA2 = ~LATAbits.LA2;
        _delay_(delay);

    }


    return;
}
