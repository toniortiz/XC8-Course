#include <xc.h>
#include "config.h"
#include "lcd.h"
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

volatile uint8_t canales[3] = {0b0000, 0b0001, 0b0010};
volatile uint8_t j = 0;

void int_adc() {
    if (PIR1bits.ADIF == 1) {
        uint16_t resultado = (((uint16_t) ADRESH) << 8) | (ADRESL);

        char buf[8];
        if (j == 0) {
            sprintf(buf, "c0:%d", resultado);
            lcd_putrs_at(0, 0, buf);
        } else if (j == 1) {
            sprintf(buf, "c1:%d", resultado);
            lcd_putrs_at(0, 9, buf);
        }
        else if (j == 2) {
            sprintf(buf, "c2:%d", resultado);
            lcd_putrs_at(1, 0, buf);
        }

        j++;
        if (j > 2)
            j = 0;

        __delay_us(3);
        ADCON0bits.CHS = canales[j]; // seleccionar el sig canal a leer
        ADCON0bits.GODONE = 1;
        PIR1bits.ADIF = 0;
    }
}

void __interrupt() interrupts() {
    int_adc();
}

void main(void) {
    OSCCON = 0x76;

    lcd_t lcd = {&PORTD, 2, 3, 4, 5, 6, 7};
    lcd_init(lcd);

    // 1. Configurar el modulo A/D
    // 1.1. Configurar los pines analogicos, voltaje de referencia
    // y la entrada digital
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 1;
    ADCON1bits.PCFG = 0b1100; // AN0, AN1 y AN2 como analogico y los demas digitales
    ADCON1bits.VCFG = 00;

    // 1.2. Seleccionar el canal analogico
    ADCON0bits.CHS = canales[j];

    // 1.3. Seleccionar tiempo de adquisicón de A/D
    ADCON2bits.ACQT = 0b010; // 4 TAD

    // 1.4. Seleccionar el reloj de conversión
    ADCON2bits.ADCS = 0b001; // FOSC/8

    // 1.5. Seleccionar el formato del resultado y encender el A/D
    ADCON2bits.ADFM = 1;
    ADCON0bits.ADON = 1;

    // 2. Configurar la interrupcion (si se usar)
    PIR1bits.ADIF = 0; // limpiar bandera de int
    PIE1bits.ADIE = 1; // habilitamos la int del adc
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    // 4. Empezar la conversión
    ADCON0bits.GODONE = 1;

    while (1) {
        //        // 3. Esperar el tiempo de adquisicón
        //        __delay_us(3);
        //
        //        // 4. Empezar la conversión
        //        ADCON0bits.GODONE = 1;
        //
        //        // 5. Esperar a que la conversión se complete
        //        while (ADCON0bits.GODONE == 1);
        //
        //        // 6. Leer los registros del resultado de la conversion
        //        uint16_t resultado = (((uint16_t) ADRESH) << 8) | (ADRESL);
        //
        //        char buf[5];
        //        sprintf(buf, "%d", resultado);
        //        lcd_clear();
        //        lcd_putrs_at(0, 0, buf);

    }

    return;
}
