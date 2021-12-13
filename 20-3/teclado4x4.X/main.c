#include <xc.h>
#include "fuses.h"
#include "lcd.h"

#define _XTAL_FREQ 8000000

unsigned char leer_teclado();
void int_ext_0();

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISE = 0x0F; // pueroto E como entrada digital

    // INT0
    TRISBbits.RB0 = 1;
    INTCONbits.INT0IE = 1; // habilitar int ext 0
    INTCONbits.INT0IF = 0; // limpiar bandera
    INTCON2bits.INTEDG0 = 1; // flanco de subida

    INTCONbits.GIE = 1; // habilita ints globales

    lcd_init();
    lcd_command(LCD_BLINK_CURSOR_ON);

    while (1) {

    }
    
    return;
}

void int_ext_0() {
    if (INTCONbits.INT0IF == 1) {
        unsigned char tecla = leer_teclado();

        if (tecla == 'A') {
            lcd_command(LCD_CLEAR);
            lcd_command(LCD_RETURN_HOME);
        } else if (tecla >= 49 && tecla <= 57) { // se presiono un numero entre 0 y 9
            lcd_char_cp(tecla);
        }

        INTCONbits.INT0IF = 0; // limpiar bandera
    }
}

void __interrupt() interrupts() {
    int_ext_0();
}

unsigned char leer_teclado() {
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
        case 14: tecla = '=';
            break;
        case 15: tecla = 'D';
            break;
        default: break;
    }

    return tecla;
}