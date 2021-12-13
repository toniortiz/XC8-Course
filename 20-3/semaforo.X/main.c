#include <xc.h>
#include "fuses.h"
#include <stdint.h>

#define _XTAL_FREQ 8000000

void main(void) {
    // Configuracion de oscilador interno a 8 MHz
    OSCCON = 0x76;

    // Configuracion de todos los pines como digitales
    ADCON1 = 0x0F;

    // Puerto B como salida
    TRISB = 0x00;
    TRISAbits.RA0 = 1; // RA0 como entrada digital

    // Limpiar puerto B
    LATB = 0x00;

    while (1) {
        LATB = 0b10000001; // S1 -> verde   S2 -> rojo
        __delay_ms(3000);

        // Parpadeo luz amarilla de S1
        LATBbits.LATB0 = 0; // apagar luz verde de S1
        int i;
        for (i = 0; i < 3; i++) {
            LATBbits.LATB1 = ~LATBbits.LATB1;
            __delay_ms(100);
        }

        LATB = 0b00100100; // S1 -> rojo    S2 -> verde
        __delay_ms(3000);

        // Parpadeo luz amarilla S2
        LATBbits.LATB5 = 0; // apagar verde de S2
        int j;
        for (j = 0; j < 3; j++) {
            LATBbits.LATB6 = ~LATBbits.LATB6;
            __delay_ms(100);
        }


        if (PORTAbits.RA0 == 1) {
            LATBbits.LB4 = ~LATBbits.LB4;
            __delay_ms(3000);
        }

    }


    return;
}
