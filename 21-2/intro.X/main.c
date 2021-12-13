#include <xc.h>
#include "config.h"
#include <stdbool.h>
#include <stdint.h>

// Macro usada por la funcion de delay
#define _XTAL_FREQ 8000000

void main(void) {
    // Configuración del micro
    //    OSCCONbits.SCS0 = 0;
    //    OSCCONbits.SCS1 = 1;
    //    OSCCONbits.IOFS = 1;
    //    OSCCONbits.OSTS = 0;
    //    OSCCONbits.IRCF0 = 1;
    //    OSCCONbits.IRCF1 = 1;
    //    OSCCONbits.IRCF2 = 1;
    //    OSCCONbits.IDLEN = 0;

    //    OSCCON = 0b01110110;
    //    OSCCON = 118;
    OSCCON = 0x76;

    //    ADCON1bits.PCFG0 = 1;
    //    ADCON1bits.PCFG1 = 1;
    //    ADCON1bits.PCFG2 = 1;
    //    ADCON1bits.PCFG3 = 1;
    //    ADCON1bits.VCFG0 = 0;
    //    ADCON1bits.VCFG1 = 0;

    ADCON1 = 0x0F;

    // Configurar el pin 3 como salida digital
    // 1 -> entrada
    // 0 -> salida
    TRISAbits.RA1 = 0;
    LATAbits.LA1 = 0; // Limpio registro latch

    TRISAbits.RA2 = 0;
    LATAbits.LA2 = 1;

    bool x = false;

    while (1) {
        // logica del programa 
        // Encender y apagar un led conectado al pin 3 RA1

        LATAbits.LA1 = x;
        LATAbits.LA2 = ~x;

        __delay_ms(200);
        x = ~x;

    }

    return;
}