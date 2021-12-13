#include <xc.h>
#include "config.h"
#include "lcd.h"    
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000

uint16_t valor = 0;
uint16_t valor2 = 0;
bool ingresando_valor2 = false;

unsigned char leer_teclado() {
    unsigned char tecla = ' ';

    switch (PORTE) {
        case 0: tecla = '1'; // tecla = 49
            break;
        case 1:tecla = '2';
            break;
        case 2: tecla = '3'; // tecla = 51
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
        case 9:tecla = '8';
            break;
        case 10: tecla = '9'; // tecla = 57
            break;
        case 11:tecla = 'C';
            break;
        case 12:tecla = '*';
            break;
        case 13: tecla = '0'; // tecla = 48
            break;
        case 14:tecla = '=';
            break;
        case 15: tecla = 'D';
            break;
        default: break;
    }

    return tecla;
}

void int_ext0() {
    if (INTCONbits.INT0IF == 1) {
        unsigned char tecla = leer_teclado();

        if (isdigit(tecla)) {
            if (!ingresando_valor2) {
                valor = valor * 10 + (tecla - 48);
                char buf[6];
                sprintf(buf, "%d", valor);
                lcd_putrs_at(0, 6, buf);
            } else {
                valor2 = valor2 * 10 + (tecla - 48);
                char buf[6];
                sprintf(buf, "%d", valor2);
                lcd_putrs_at(1, 6, buf);
            }
        } else if (tecla == 'A') {
            lcd_putrs_at(1, 0, "num 2:");
            ingresando_valor2 = true;
        } else if (tecla == '=') {
            uint16_t suma = valor + valor2;
            lcd_clear();
            char buf[10];
            sprintf(buf, "sum:%d", suma);
            lcd_putrs_at(0, 0, buf);
        } else if (tecla == 'C') {
            lcd_clear();
            valor = 0;
            valor2 = 0;
            lcd_putrs_at(0, 0, "num 1:");
            ingresando_valor2 = false;
        }

        INTCONbits.INT0IF = 0;
    }
}

void __interrupt() interrupts() {
    int_ext0();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    lcd_t lcd = {&PORTD, 2, 3, 4, 5, 6, 7};
    lcd_init(lcd);

    PORTE = 0x0F; // puerto E como entrada digital

    // INT0
    TRISBbits.RB0 = 1;
    INTCONbits.INT0IE = 1; // habilitat int0
    INTCON2bits.INTEDG0 = 1; // flanco de subida
    INTCONbits.INT0IF = 0; // limpiar bandera

    INTCONbits.GIE = 1;

    lcd_putrs_at(0, 0, "num 1:");

    while (1) {

    }
    return;
}
