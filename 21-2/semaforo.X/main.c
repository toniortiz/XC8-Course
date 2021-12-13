#include <xc.h>
#include "config.h"

#define _XTAL_FREQ 8000000

#define VERDE_S1 LATDbits.LD7
#define AMARILLO_S1 LATDbits.LD6
#define ROJO_S1 LATDbits.LD5

#define VERDE_S2 LATCbits.LC7
#define AMARILLO_S2 LATCbits.LC6
#define ROJO_S2 LATCbits.LC2

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISDbits.RD7 = 0; // verde 1
    TRISDbits.RD6 = 0; // amarilla 1
    TRISDbits.RD5 = 0; // roja 1

    TRISCbits.RC7 = 0; // verde 2
    TRISCbits.RC6 = 0; // amarillo 2
    TRISCbits.RC2 = 0; // roja 2

    // valores iniciales
    VERDE_S1 = 1; // encendida la luz verde del semaforo 1
    AMARILLO_S1 = 0;
    ROJO_S1 = 0;

    ROJO_S2 = 1; // encendeida la luz roja del semaforo 2
    VERDE_S2 = 0;
    AMARILLO_S2 = 0;

    while (1) {
        VERDE_S1 = 1;
        AMARILLO_S1 = 0;
        ROJO_S1 = 0;

        VERDE_S2 = 0;
        AMARILLO_S2 = 0;
        ROJO_S2 = 1;

        __delay_ms(3000);

        // Parpadeo luz amarilla S1
        VERDE_S1 = 0;
        int i;
        for (i = 0; i <= 3; i++) {
            AMARILLO_S1 = ~AMARILLO_S1;
            __delay_ms(300);
        }
        AMARILLO_S1 = 0;
        ROJO_S1 = 1;
        ROJO_S2 = 0;
        VERDE_S2 = 1;

        __delay_ms(3000);

        // Parpadeo luz amarilla S2
        VERDE_S2 = 0;
        for (i = 0; i <= 3; i++) {
            AMARILLO_S2 = ~AMARILLO_S2;
            __delay_ms(300);
        }
        AMARILLO_S2 = 0;
    }

    return;
}
