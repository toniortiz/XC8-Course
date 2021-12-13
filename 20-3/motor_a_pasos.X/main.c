#include <xc.h>
#include <stdint.h>
#include "fuses.h"

#define _XTAL_FREQ 8000000
#define VEL 50

const uint8_t completo[4] = {0x01, 0x02, 0x04, 0x08};
const uint8_t completo_inv[4] = {0x01, 0x08, 0x04, 0x02};
const uint8_t medio[4] = {0x03, 0x06, 0x0C, 0x09};
const uint8_t micro[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};

void paso_completo();
void medio_paso();
void micro_paso();

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISA = 0;
    LATA = 0;

    paso_completo();
    // medio_paso();
    //micro_paso();

    while (1) {
    }
    return;
}

void paso_completo() {
    // paso completo
    // A B C D
    // 1 0 0 0  primer paso
    // 0 1 0 0  segundo paso
    // 0 0 1 0  tercer paso
    // 0 0 0 1  cuarto paso

    uint8_t i = 0;

    for (int j = 0; j < 48; j++) {
        LATA = 0;
        __delay_us(15);

        LATA = completo[i];
        __delay_ms(VEL);

        i++;
        if (i > 3)i = 0;
    }

}

void medio_paso() {
    // medio completo
    // A B C D
    // 1 1 0 0  primer paso
    // 0 1 1 0  segundo paso
    // 0 0 1 1  tercer paso
    // 1 0 0 1  cuarto paso

    uint8_t i = 0;

    for (int j = 0; j < 48; j++) {
        LATA = 0;
        __delay_us(15);

        LATA = medio[i];
        __delay_ms(VEL);

        i++;
        if (i > 3)i = 0;

    }

}

void micro_paso() {
    // micro paso
    // A B C D
    // 1 0 0 0
    // 1 1 0 0
    // 0 1 0 0
    // 0 1 1 0
    // 0 0 1 0
    // 0 0 1 1
    // 0 0 0 1
    // 1 0 0 1

    uint8_t i = 0;

    for (int j = 0; j < 48 * 2; j++) {
        LATA = 0;
        __delay_us(15);

        LATA = micro[i];
        __delay_ms(VEL);

        i++;
        if (i > 7)i = 0;
    }

}