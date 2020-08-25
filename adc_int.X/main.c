#include <xc.h>
#include "fuses.h"
#include <stdint.h>
#include "lcd.h"
#include <stdio.h>
#include <stdbool.h>
#define _XTAL_FREQ 8000000

typedef enum {
    AN0_t = 0, AN1_t, AN2_t
} channel_t;

volatile channel_t canal_adc = AN0_t;

volatile uint16_t val_adc_0_prev, val_adc_1_prev, val_adc_2_prev;

void adc_int();

void main(void) {
    OSCCON = 0x76;


    lcd_init();
    lcd_command(LCD_CURSOR_OFF);
    lcd_const_text(1, 1, "ADC int test");
    __delay_ms(3000);
    lcd_command(LCD_CLEAR);
    lcd_const_text(1, 1, "T=");
    lcd_const_text(1, 9, "H=");
    lcd_const_text(2, 1, "I=");

    val_adc_0_prev = val_adc_1_prev = val_adc_2_prev = 0;

    // 1. Configure the A/D module
    // 1.1. Configure analog pins, voltage referene and digital I/O (ADCON1)
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 1;
    ADCON1bits.PCFG = 0b1100; // AN0, AN1, AN2
    ADCON1bits.VCFG = 0b00; // (5-0)v

    // 1.2. Select A/D input channel (ADCON0)
    ADCON0bits.CHS = 0b0000; // select AN0

    // 1.3. Select A/D acquisition time (ADCON2)
    ADCON2bits.ACQT = 0b010; // 4 TAD

    // 1.4. Select A/D conversion clock (ADCON2)
    ADCON2bits.ADCS = 0b001; // FOSC/8

    // 1.5. Select A/D result format (ADCON2) and turn on A/D module (ADCON0)
    ADCON2bits.ADFM = 1; // right justified
    ADCON0bits.ADON = 1; // A/D module enabled

    // 2. Configure A/D interrupt
    // 2.1. Clear ADIF bit
    PIR1bits.ADIF = 0;

    // 2.2. Set ADIE bit
    PIE1bits.ADIE = 1;

    // 2.3. Set GIE and PEIE bit
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    __delay_us(20);

    // 4. Start conversion: set GO/CONE bit (ADCON0)
    ADCON0bits.GODONE = 1;


    while (1) {
    }
}

void adc_int() {
    if (PIR1bits.ADIF == 1) {
        uint16_t adc_val = (((uint16_t) ADRESH) << 8) | (ADRESL);

        char buf[5];

        switch (canal_adc) {
            case AN0_t: // simular sensor de temperatura lm35

                if (adc_val != val_adc_0_prev) {
                    float temperatura = (adc_val * (5.0 / 1024) / 0.01);

                    lcd_const_text(1, 3, "     ");
                    sprintf(buf, "%.1f", temperatura);
                    lcd_text(1, 3, buf);

                }

                // selecionar canar 1 para la proxima conversion del adc
                canal_adc = AN1_t;
                ADCON0bits.CHS = 0b0001;
                val_adc_0_prev = adc_val;
                break;
            case AN1_t: // simular sensor de humedad HMZ-433A
                if (adc_val != val_adc_1_prev) {
                    float humedad = (adc_val * (5.0 / 3.3)) / 10.24;

                    lcd_const_text(1, 11, "     ");
                    sprintf(buf, "%.1f", humedad);
                    lcd_text(1, 11, buf);

                }
                // selecionar canar 2 para la proxima conversion del adc
                canal_adc = AN2_t;
                ADCON0bits.CHS = 0b0010;
                val_adc_1_prev = adc_val;
                break;
            case AN2_t: // sensor de iluminacion TEMT6000
                if (adc_val != val_adc_2_prev) {
                    float iluminacion = (adc_val * (5.0 / 1024.0)) / 0.0025;

                    sprintf(buf, "%.1f", iluminacion);
                    lcd_const_text(2, 3, "          ");
                    lcd_text(2, 3, buf);

                }
                // selecionar canal 0 para la proxima conversion del adc
                canal_adc = AN0_t;
                ADCON0bits.CHS = 0b0000;
                val_adc_2_prev = adc_val;
                break;
            default: break;
        }

        __delay_us(20);
        ADCON0bits.GODONE = 1;
        PIR1bits.ADIF = 0;
    }
}

void __interrupt() interrupts() {
    adc_int();
}
