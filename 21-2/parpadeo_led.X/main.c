#include <xc.h>
#include "config.h"
#include <stdint.h>

// Definimos esta macro para poder usar las funciones de delay de XC8
#define _XTAL_FREQ 8000000

void _delay_(uint16_t delay) {
    int i;
    for (i = 0; i < delay; i++) {
        __delay_ms(1);
    }
}

void main(void) {
    OSCCON = 0x76; // configuracion oscilador interno a 8 MHz
    ADCON1 = 0x0F; // Todos lo pines del micro como I/O digitales

    // boton conectado a este pin, tenemos que declarar como entrada digital
    TRISAbits.RA0 = 1;

    // led conectado a este pin, tenemos que declarar como salida digital
    TRISAbits.RA3 = 0;
    LATAbits.LA3 = 0; // limpiar bit

    uint16_t delay = 50;

    while (1) {
        if (PORTAbits.RA0 == 1) {
            delay += 50;
            if (delay > 1000)
                delay = 50;

            while (PORTAbits.RA0 == 1)
                __delay_ms(1);
        }

        _delay_(delay);

        LATAbits.LA3 = ~LATAbits.LA3;
    }
    return;
}
