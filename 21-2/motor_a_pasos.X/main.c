// Paso compleo
// A B C D
// 1 0 0 0 
// 0 1 0 0
// 0 0 1 0
// 0 0 0 1 

// Medio paso
// A B C D
// 1 1 0 0
// 0 1 1 0
// 0 0 1 1
// 1 0 0 1

// Micro paso
// A B C D
// 1 0 0 0
// 1 1 0 0
// 0 1 0 0
// 0 1 1 0
// 0 0 1 0
// 0 0 1 1
// 0 0 0 1
// 1 0 0 1

#include <xc.h>
#include "config.h"
#include <inttypes.h>   

#define GRADOS_POR_PASO 7.5
#define NUM_PASOS_REV 360.0/GRADOS_POR_PASO
#define VEL 50

const uint8_t completo[4] = {0b1, 0b10, 0b100, 0b1000};
const uint8_t completo_inv[4] = {0b1, 0b1000, 0b100, 0b10};
const uint8_t medio[4] = {0b11, 0b110, 0b1100, 0b1001};
const uint8_t micro[8] = {0b1, 0b11, 0b10, 0b110, 0b100, 0b1100, 0b1000, 0b1001};

void revolucion(const uint8_t* tipo_paso, uint8_t n) {
    uint8_t j;
    uint8_t i;
    for (j = 0; j < NUM_PASOS_REV; j++) {
        LATA = 0;
        __delay_us(15);
        LATA = tipo_paso[i];
        __delay_ms(VEL);

        i++;
        if (i >= n) i = 0;
    }
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISA = 0;
    LATA = 0;

    int i;
    for (i = 0; i < 5; i++) {
        revolucion(completo_inv, 4);
    }

    while (1) {

    }

    return;
}
