#include <xc.h>
#include "fuses.h"
#include <stdint.h>
#include "lcd.h"
#include <stdio.h>

#define _XTAL_FREQ 8000000

uint8_t productos = 0;
uint8_t cajas = 0;

void int_ext_0() {
    if (INTCONbits.INT0IF) {
        productos++;

        if (productos == 12) {
            cajas++;
            productos = 0;
        }

        lcd_command(LCD_CLEAR);
        char buf[15];
        sprintf(buf, "Productos:%u", productos);
        lcd_text(1, 1, buf);

        char buf2[15];
        sprintf(buf2, "Cajas:%u", cajas);
        lcd_text(2, 1, buf2);

        INTCONbits.INT0IF = 0; // limpiar bandera
    }
}

void __interrupt() interrupts() {
    int_ext_0();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    // Configuracion int ext 0
    TRISBbits.RB0 = 1;
    INTCONbits.GIE = 1; // habilitar todas las interrupciones
    INTCONbits.INT0IE = 1; // habilitar int ext 0
    INTCON2bits.INTEDG0 = 1; // flanco de subida

    lcd_init();
    lcd_command(LCD_CLEAR);
    lcd_command(LCD_CURSOR_OFF);
    lcd_command(LCD_RETURN_HOME);

    lcd_text(1, 1, "Productos:0");
    lcd_text(2, 1, "Cajas:0");

    while (1) {

    }
    return;
}
