// El servomotor trabaja con una frecuencia de 50 Hz, por lo tanto,
// es necesario usar el oscilador interno a 500 kHz.
// 1 ms = 0°
// 2 ms = 180°
// Sabemos que un ciclo de trabajo de 1023 equivale a 20 ms
// Por regla de tres se puede calcular el intervalo para el ciclo de trabajo
#include <xc.h>
#include "config.h"
#include <stdint.h>
#include "lcd.h"

#define _XTAL_FREQ 500000
#define TMR2_P 16
#define F_PWM 50
#define MAX_DC _XTAL_FREQ/(F_PWM*TMR2_P)
#define MIN_0 52
#define MAX_180 110
#define DC_PER_ANGLE (MAX_180-MIN_0)/180.0 // 0.3

float ciclo_trabajo = 52;
float cur_pos = 0;

void pwm1_ct(uint16_t ciclo_trabajo);

void grados(uint8_t ang) {
    float dif = (float) ang - cur_pos;

    float pasos = DC_PER_ANGLE * dif;
    ciclo_trabajo += pasos;

    if (ciclo_trabajo > MAX_180)
        ciclo_trabajo = MAX_180;
    else if (ciclo_trabajo < MIN_0)
        ciclo_trabajo = MIN_0;

    cur_pos = ang;
}

void main(void) {
    OSCCON = 0x36; // 0011 0110
    ADCON1 = 0x0F;

    lcd_t lcd = {&PORTD, 2, 3, 4, 5, 6, 7};
    lcd_init(lcd);

    // 1. Periodo del PWM
    PR2 = 155;

    // 2. Valor del ciclo de trabajo
    // Iniciamente el dc = 0
    CCPR1L = 0;
    CCP1CONbits.DC1B = 0b00;

    // 3. CCP1 como salida
    TRISCbits.RC2 = 0;

    // 4. Prescaler del TMR2 y encenderlo
    T2CONbits.T2CKPS = 0b11; // prescaler de 16
    T2CONbits.TMR2ON = 1;

    // 5. Configurar el CCP en modo PWM
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;

    //uint16_t ciclo_trabajo = 110; // 180°
    //uint16_t ciclo_trabajo = 52; // 0°
    pwm1_ct(ciclo_trabajo);

    while (1) {
        grados(0);
        pwm1_ct(ciclo_trabajo);
        __delay_ms(2000);


        grados(10);
        pwm1_ct(ciclo_trabajo);
        __delay_ms(2000);

        grados(15);
        pwm1_ct(ciclo_trabajo);
        __delay_ms(2000);

        grados(80);
        pwm1_ct(ciclo_trabajo);
        __delay_ms(2000);

        grados(156);
        pwm1_ct(ciclo_trabajo);
        __delay_ms(2000);

        grados(5);
        pwm1_ct(ciclo_trabajo);
        __delay_ms(2000);

        grados(180);
        pwm1_ct(ciclo_trabajo);
        __delay_ms(2000);

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
}
