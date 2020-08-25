#include <xc.h>
#include "fuses.h"
#include <stdint.h>
#include "lcd.h"
#include <stdio.h>

#define _XTAL_FREQ 8000000

//uint16_t leer_pot1() {
//    // Potenciometro 1
//    ADCON0bits.CHS = 0b0000; // select AN0
//
//    // 3. Wait the required acquisition time (if required)
//    __delay_us(3);
//
//    // 4. Start conversion: set GO/DONE bit (ADCON0)
//    ADCON0bits.GODONE = 1;
//
//    // 5. Wait for A/D conversion to complete, by either: polling for the 
//    // GODONE bit to be cleared OR waiting for the A/D interrupt
//    while (ADCON0bits.GODONE == 1);
//
//    // 6. Read A/D result registers (ADRESH:ADRESL); clear bit ADIF, if required
//    uint16_t adc_val = (((uint16_t) ADRESH) << 8) | (ADRESL);
//    return adc_val;
//}

uint16_t leer_canal_analogico(uint8_t canal) {
    ADCON0bits.CHS = canal; // select channel

    // 3. Wait the required acquisition time (if required)
    __delay_us(3);

    // 4. Start conversion: set GO/DONE bit (ADCON0)
    ADCON0bits.GODONE = 1;

    // 5. Wait for A/D conversion to complete, by either: polling for the 
    // GODONE bit to be cleared OR waiting for the A/D interrupt
    while (ADCON0bits.GODONE == 1);

    // 6. Read A/D result registers (ADRESH:ADRESL); clear bit ADIF, if required
    uint16_t adc_val = (((uint16_t) ADRESH) << 8) | (ADRESL);
    return adc_val;
}

void main(void) {
    OSCCON = 0x76;

    lcd_init();
    lcd_command(LCD_CURSOR_OFF);
    lcd_const_text(1, 1, "ADC test");
    __delay_ms(3000);

    // 1. Configure the A/D module
    // 1.1. Configure analog pins, voltage referene and digital I/O (ADCON1)
    TRISAbits.RA0 = 1;
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
    // 2.2. Set ADIE bit
    // 2.3. Set GIE bit


    while (1) {
        // Potenciometro 1
        ADCON0bits.CHS = 0b0000; // select AN0

        // 3. Wait the required acquisition time (if required)
        __delay_us(3);

        // 4. Start conversion: set GO/DONE bit (ADCON0)
        ADCON0bits.GODONE = 1;

        // 5. Wait for A/D conversion to complete, by either: polling for the 
        // GODONE bit to be cleared OR waiting for the A/D interrupt
        while (ADCON0bits.GODONE == 1);

        // 6. Read A/D result registers (ADRESH:ADRESL); clear bit ADIF, if required
        uint16_t adc_val = (((uint16_t) ADRESH) << 8) | (ADRESL);

        //  uint16_t adc_val = leer_pot1();

        char buf1[15];
        sprintf(buf1, "P1=%d", adc_val);
        lcd_command(LCD_CLEAR);
        lcd_text(1, 1, buf1);

        // 7. For next conversion, go to step 1 or step 2, as required. The A/D
        // conversion time per bit is defined as TAD. A minimum wait of 3 TAD is
        // required before the next acquisition starts
        __delay_us(3);

        // Potenciometro 2
        ADCON0bits.CHS = 0b0001; // Select AN1
        __delay_us(3);
        ADCON0bits.GODONE = 1; // Start conversion
        while (ADCON0bits.GODONE == 1); // wait for conversion
        uint16_t adc_val2 = (((uint16_t) ADRESH) << 8) | (ADRESL); // read result

        char buf2[15];
        sprintf(buf2, "P2=%d", adc_val2);
        lcd_text(1, 9, buf2);

        __delay_us(3);

        // Potenciometro 3
        ADCON0bits.CHS = 0b0010; // Select AN2
        __delay_us(3);
        ADCON0bits.GODONE = 1; // Start conversion
        while (ADCON0bits.GODONE == 1); // wait for conversion
        uint16_t adc_val3 = (((uint16_t) ADRESH) << 8) | (ADRESL); // read result

        char buf3[15];
        sprintf(buf3, "P3=%d", adc_val3);
        lcd_text(2, 1, buf3);

        __delay_ms(100);



        //            uint16_t pot1 = leer_canal_analogico(0b00000000);
        //            uint16_t pot2 = leer_canal_analogico(0b00000001);
        //            uint16_t pot3 = leer_canal_analogico(0b00000010);
        //
    }

}
