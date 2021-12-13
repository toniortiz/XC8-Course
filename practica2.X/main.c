#include <xc.h>
#include "fuses.h"
#include "lcd.h"
#include <stdint.h>

#define _XTAL_FREQ 8000000

uint8_t row = 1;
uint8_t col = 1;
uint8_t letter = 65;
uint8_t is_full = 0;

void int_ext_0() {
    if (INTCONbits.INT0IF) {
        if (col >= 1) {
            lcd_command(LCD_UNDERLINE_ON);
            if (row == 2 && col == 1) {
                row = 1;
                col = 16;
            } else {
                col--;
            }

            if (col <= 16) {
                is_full = 0;
            }
        } else {
            lcd_command(LCD_CURSOR_OFF);
        }

        lcd_set_cursor(row, col);

        INTCONbits.INT0IF = 0;
    }
}

void int_ext_1() {
    if (INTCON3bits.INT1IF) {
        if (!is_full) {
            lcd_set_cursor(row, col);
            lcd_char_cp(letter);
            lcd_set_cursor(row, col);

            letter++;
            if (letter > 90)
                letter = 65;
        }

        INTCON3bits.INT1IF = 0;
    }
}

void int_ext_2() {
    if (INTCON3bits.INT2IF) {
        if (col == 16 && row == 1) {
            row = 2;
            col = 1;
            is_full = 0;
            lcd_command(LCD_UNDERLINE_ON);
        } else if (col > 16 && row == 2) {
            // LCD lleno
            is_full = 1;
            lcd_command(LCD_CURSOR_OFF);
        } else {
            col++;
            is_full = 0;
            lcd_command(LCD_UNDERLINE_ON);
        }
        lcd_set_cursor(row, col);

        INTCON3bits.INT2IF = 0;
    }
}

void __interrupt() interrupts() {
    int_ext_0();

    int_ext_1();

    int_ext_2();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;

    TRISBbits.RB3 = 0; // green
    TRISBbits.RB4 = 0; // red

    INTCONbits.GIE = 1;

    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 1;

    INTCON3bits.INT1IE = 1;
    INTCON2bits.INTEDG1 = 1;

    INTCON3bits.INT2IE = 1;
    INTCON2bits.INTEDG2 = 1;

    lcd_init();
    lcd_command(LCD_CLEAR);
    lcd_command(LCD_CURSOR_OFF);
    lcd_command(LCD_UNDERLINE_ON);
    lcd_command(LCD_RETURN_HOME);

    lcd_char_cp(letter++);

    while (1) {
        if (is_full) {
            LATBbits.LB3 = 0;
            LATBbits.LB4 = 1;
        } else {
            LATBbits.LB3 = 1;
            LATBbits.LB4 = 0;
        }
    }



    return;
}
