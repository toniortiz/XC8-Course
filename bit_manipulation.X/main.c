#include <xc.h>
#include <stdint.h>
#include "fuses.h"

#define _XTAL_FREQ 8000000

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    // TRIS -> Configure pin as digital input or output
    // PORT -> Read the pin voltage level
    // LAT -> Read and write the LATCH register of the pin

    TRISB = 0x00; // port B as digital output
    LATB = 0x00; // clean port B

    uint16_t value = 0x01;

    while (1) {

        //        LATAbits.LA1 = ~LATAbits.LA1;
        //        LATB = ~mask;
        //        __delay_ms(10);
        //        mask = LATB;

        LATB = value;
        __delay_ms(250);

        value <<= 1;
        // value >>= 1; 
        if (value > 0b10000000) {
            value = 0b00000001;
        }
    }

    return;
}