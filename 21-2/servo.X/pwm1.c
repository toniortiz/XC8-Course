#include "pwm1.h"

void config_pwm1(uint8_t pr2, uint8_t prescaler_tmr2) {
    // Configurar pwm a 50 Hz
    // 1. Periodo del PWM
    PR2 = pr2;

    // 2. Valor del ciclo de trabajo
    // Iniciamente el dc = 0
    CCPR1L = 0;
    CCP1CONbits.DC1B = 0b00;

    // 3. CCP1 como salida
    TRISCbits.RC2 = 0;

    // 4. Prescaler del TMR2 y encenderlo
    switch (prescaler_tmr2) {
        case 1:
            T2CONbits.T2CKPS = 0b00; // prescaler de 1
            break;
        case 4:
            T2CONbits.T2CKPS = 0b01; // prescaler de 4
            break;
        case 16:
            T2CONbits.T2CKPS = 0b11; // prescaler de 16
            break;
        default:
            T2CONbits.T2CKPS = 0b00; // prescaler de 1
            break;
    }

    T2CONbits.TMR2ON = 1;

    // 5. Configurar el CCP en modo PWM
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
}

void set_ct_pwm1(uint16_t ct){
    if (ct < 1024) {
        ct = ((float) ct / 1023.0) * MAX_DC;

        // ptr apunta al byte menos significativo de la variable ciclo_trabajo
        uint8_t* ptr = (uint8_t*) & ct;

        // Desplazar 6 bits hacia la izquierda
        ct <<= 6;

        // ptr apunta al MSB de ciclo_trabajo
        ptr++;

        CCPR1L = *ptr;

        // ptr apunta al LSB de ciclo_trabajo
        ptr--;

        CCP1CON = (CCP1CON & 0xCF) | (((*ptr) >> 2) & 0x30);
    }
}