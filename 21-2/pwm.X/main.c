#include <xc.h>
#include "config.h"
#include <stdint.h>
#include "lcd.h"

#define _XTAL_FREQ 8000000
#define TMR2_P 4
#define F_PWM 2000
#define MAX_DC _XTAL_FREQ/(F_PWM*TMR2_P)

void pwm1_ct(uint16_t ciclo_trabajo);

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    lcd_t lcd = {&PORTD, 2, 3, 4, 5, 6, 7};
    lcd_init(lcd);

    // 1. Periodo del PWM
    PR2 = 249;

    // 2. Valor del ciclo de trabajo
    // Iniciamente el dc = 0
    CCPR1L = 0;
    CCP1CONbits.DC1B = 0b00;

    // 3. CCP1 como salida
    TRISCbits.RC2 = 0;

    // 4. Prescaler del TMR2 y encenderlo
    T2CONbits.T2CKPS = 0b01; // prescaler de 4
    T2CONbits.TMR2ON = 1;

    // 5. Configurar el CCP en modo PWM
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;

    uint16_t ciclo_trabajo = 5;

    while (1) {
        pwm1_ct(ciclo_trabajo);
        ciclo_trabajo += 5;
        __delay_ms(20);
        if (ciclo_trabajo > 1023)
            ciclo_trabajo = 5;
    }
    return;
}

void pwm1_ct(uint16_t ciclo_trabajo) {
    if (ciclo_trabajo < 1024) {
        ciclo_trabajo = ((float) ciclo_trabajo / 1023.0) * MAX_DC;
        
        // ptr apunta al byte menos significativo de la variable ciclo_trabajo
        uint8_t* ptr = (uint8_t*) & ciclo_trabajo;

        // Desplazar 6 bits hacia la izquierda
        ciclo_trabajo <<= 6;

        // ptr apunta al MSB de ciclo_trabajo
        ptr++;

        CCPR1L = *ptr;

        // ptr apunta al LSB de ciclo_trabajo
        ptr--;

        CCP1CON = (CCP1CON & 0xCF) | (((*ptr) >> 2) & 0x30);
    }


    //    switch (*ptr) {
    //        case 0:
    //            CCP1CONbits.DC1B = 0b00;
    //            break;
    //        case 64:
    //            CCP1CONbits.DC1B = 0b01;
    //            break;
    //        case 128:
    //            CCP1CONbits.DC1B = 0b10;
    //            break;
    //        case 192:
    //            CCP1CONbits.DC1B = 0b11;
    //        default:
    //            break;
    //}

}