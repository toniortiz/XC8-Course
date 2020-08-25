#include <xc.h>
#include "fuses.h"
#include <stdint.h>
#include "lcd.h"    
#include <stdio.h>

#define _XTAL_FREQ 8000000

void adcint();

void main(void) {
    OSCCON = 0x76;

    lcd_init();
    lcd_command(LCD_CURSOR_OFF);
    lcd_const_text(1, 1, "ADC test");
    __delay_ms(3000);

    TRISAbits.RA1 = 0;
    LATAbits.LA1 = 0;

    // 1. Configure the A/D module

    // 1.1. Configure analog pins, voltage reference and digital I/O (ADCON1)
    TRISAbits.RA0 = 1;
    ADCON1bits.PCFG = 0b1110; // AN0
    ADCON1bits.VCFG = 0b00; // (5-0)v

    // 1.2. Select A/D input channel (ADCON0)
    ADCON0bits.CHS = 0b0000; // Select AN0

    // 1.3. Select A/D acquisition time (ADCON2)
    ADCON2bits.ACQT = 0b010; // 4 TAD

    // 1.4. Select A/D conversion clock (ADCON2)
    ADCON2bits.ADCS = 0b001; // FOSC/8

    // 1.5. Select A/D result format (ADCON2) and turn on A/D module (ADCON0)
    ADCON2bits.ADFM = 1;
    ADCON0bits.ADON = 1;

    // 2. Configure A/D interrupt (if desired)
    // 2.1. Clear ADIF bit
    PIR1bits.ADIF = 0;

    // 2.2. Set ADIE bit
    PIE1bits.ADIE = 1;

    // 2.3. Set GIE bit
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    __delay_us(40);
    ADCON0bits.GODONE = 1;


    while (1) {
        // 3. Wait the required acquisition time (if required)
        // __delay_us(3);

        // 4. Start conversion: set GO/CONE bit (ADCON0)
        // ADCON0bits.GODONE = 1;

        // 5. Wait for A/D conversion to complete, by either: polling for the 
        // GODONE bit to be cleared OR waiting for the A/D interrupt
        // while (ADCON0bits.GODONE);

        // 6. Read A/D result registers (ADRESH:ADRESL); clear bit ADIF, if required
        // uint16_t adc_val = (((uint16_t) ADRESH) << 8) | (ADRESL);

        //        char buf[5];
        //        sprintf(buf, "%d", adc_val);
        //        lcd_command(LCD_CLEAR);
        //        lcd_text(1, 1, buf);
        //        __delay_ms(100);


        // 7. For next conversion, go to step 1 or step 2, as required. The A/D
        // conversion time per bit is defined as TAD. A minimum wait of 3 TAD is
        // required before the next acquisition starts
    }
}

void adcint() {
    if (!PIR1bits.ADIF)
        return;

    uint16_t adc_val = (((uint16_t) ADRESH) << 8) | (ADRESL);

    if (adc_val >= 511)LATAbits.LA1 = 1;
    else LATAbits.LA1 = 0;


    char buf[5];
    sprintf(buf, "%d", adc_val);
    lcd_command(LCD_CLEAR);
    lcd_text(1, 1, buf);
    __delay_ms(50);

    __delay_us(20);
    ADCON0bits.GODONE = 1;
    PIR1bits.ADIF = 0;
}

void __interrupt() interrupts() {
    adcint();
}