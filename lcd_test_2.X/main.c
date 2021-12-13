#include <xc.h>
#include "fuses.h"
#include "lcd.h"
#include <stdbool.h>

#define _XTAL_FREQ 8000000

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    lcd_t lcd = {&PORTD, 2, 3, 4, 5, 6, 7};
    lcd_init(lcd);

    TRISDbits.RD0 = 1;
    TRISDbits.RD1 = 1;

    lcd_set_cursor(0, 0);
    for (char c = 'a'; c < 'd'; ++c) {
        lcd_putc(c);
        __delay_ms(400);
    }

    lcd_clear();
    lcd_putrs("Hola");

    while (true) {
        lcd_putrs_at(0, 0, "Hola Tony");

        int i;
        for (i = 0; i < 10; i++) {
            lcd_shift_right();
            __delay_ms(150);
        }

        lcd_return_home();
        lcd_putrs_at(0, 0, "Hola Viki");
        __delay_ms(2000);
    }
}
