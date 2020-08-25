#include <xc.h>
#include "fuses.h"
#include "lcd.h"
#include <stdint.h>

#define _XTAL_FREQ 8000000 

uint8_t letter = 65; // A en ASCII -> 65

void int_timer0() {
    if (INTCONbits.TMR0IF == 1) {
        lcd_command(LCD_RETURN_HOME);
        lcd_command(LCD_CLEAR);
        lcd_char_cp(letter);
        letter++;

        if (letter > 90) {
            letter = 65;
        }

        TMR0L = 0xEE;
        TMR0H = 0x85;

        INTCONbits.TMR0IF = 0; // limpiar bandera
    }
}

void __interrupt() interrupts() {
    int_timer0();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    // Configuración Timer0 a 1Hz
    T0CONbits.TMR0ON = 0; // Deshabilita timer0
    T0CONbits.T08BIT = 0; // 16 bits
    T0CONbits.T0CS = 0; // configuracion como timer
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0; // Usar Prescaler
    T0CONbits.T0PS2 = 1; // Prescaler de 64
    T0CONbits.T0PS1 = 0;
    T0CONbits.T0PS0 = 1;
    TMR0L = 0xEE;
    TMR0H = 0x85;

    // Configuración interrupción del timer0
    INTCONbits.TMR0IE = 1; // habilitar interrupción del timer 0
    INTCONbits.TMR0IF = 0; // limpiar bandera de interrupción del timer 0

    // Configuracion LCD
    lcd_init();
    lcd_command(LCD_CLEAR);
    lcd_command(LCD_RETURN_HOME);
    lcd_command(LCD_CURSOR_OFF);
    lcd_command(LCD_UNDERLINE_ON);

    T0CONbits.TMR0ON = 1; // Enciende el timer0

    INTCONbits.GIE = 1; // habilita todas las interrupciones
    INTCONbits.PEIE = 1; // habilita interrupciones perifericas



    while (1) {

    }


    return;
}
