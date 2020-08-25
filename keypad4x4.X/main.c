#include <xc.h>
#include "fuses.h"
#include "lcd.h"
#include <stdio.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000 

uint16_t valor = 0;

unsigned char leer_tecla();
void int_ext0();

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISE = 0x0F;

    // INT0
    TRISBbits.TRISB0 = 1;
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 0;
    INTCON2bits.INTEDG0 = 1;

    INTCONbits.GIE = 1;

    lcd_init();
    lcd_command(LCD_BLINK_CURSOR_ON);

    while (1) {

    }
}

void int_ext0() {
    if (!INTCONbits.INT0IF)
        return;

    unsigned char tecla = leer_tecla();

    if (tecla == 'A') {
        lcd_command(LCD_CLEAR);
        lcd_command(LCD_RETURN_HOME);
        valor = 0;
    } else if (tecla >= 48 && tecla <= 57) {
        valor = valor * 10 + (tecla - 48);
        char txt[6];
        sprintf(txt, "%u", valor);
        lcd_command(LCD_CLEAR);
        lcd_text(1, 1, txt);
    }

    INTCONbits.INT0IF = 0;
}

void __interrupt() interrupts() {
    int_ext0();
}

unsigned char leer_tecla() {
    unsigned char tecla = ' ';

    switch (PORTE) {
        case 0: tecla = '1';
            break;
        case 1: tecla = '2';
            break;
        case 2: tecla = '3';
            break;
        case 3: tecla = 'A';
            break;
        case 4: tecla = '4';
            break;
        case 5: tecla = '5';
            break;
        case 6: tecla = '6';
            break;
        case 7: tecla = 'B';
            break;
        case 8: tecla = '7';
            break;
        case 9: tecla = '8';
            break;
        case 10: tecla = '9';
            break;
        case 11: tecla = 'C';
            break;
        case 12: tecla = '*';
            break;
        case 13: tecla = '0';
            break;
        case 14: tecla = '#';
            break;
        case 15: tecla = 'D';
            break;
        default: break;
    }

    return tecla;
}