#include <xc.h>
#include "config.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000

char dec_to_hex(int digit) {
    return "0123456789ABCDEF"[digit];
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    lcd_t lcd = {
        &PORTD, // Puerto usado para conectar la LCD
        2, // RS
        3, // E
        4, // D4
        5, // D5
        6, // D6
        7 // D7
    };

    lcd_init(lcd);

    // Si queremos usar el pin RD0 o RD1 como entradas digitales, se tiene
    // que configurar despues de llamar a la función lcd_init
    // TRISDbits.RD0 = 1;
    // TRISDbits.RD1 = 1;

    // fila, columna
    lcd_cursor_blink();
    lcd_set_cursor(0, 0);
    lcd_putc('a');
    __delay_ms(2000);
    lcd_putc('b');

    __delay_ms(2000);
    lcd_clear();
    lcd_putrs_at(1, 0, "Hola");

    __delay_ms(2000);

    int i;
    for (i = 0; i < 10; i++) {
        lcd_shift_right();
        __delay_ms(200);
    }

    for (i = 0; i < 10; i++) {
        lcd_shift_left();
        __delay_ms(200);
    }

    lcd_clear();
    lcd_return_home();

    char cadena[10];
    for (i = 0; i < 10; i++) {
        sprintf(cadena, "i = %d", i);
        lcd_putrs_at(0, 0, cadena);
        __delay_ms(150);
    }

    lcd_clear();
    lcd_return_home();
    
    while (1) {
        for (i = 0; i < 16; i++) {
            lcd_putc_at(0, 0, dec_to_hex(i));
            __delay_ms(200);
        }

    }
    return;
}
