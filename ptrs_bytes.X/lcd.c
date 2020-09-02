#include <xc.h>
#include "lcd.h"

#define _XTAL_FREQ 8000000

lcd_t lcd;

void lcd_out(char c) {
    if (c & 1) {
        *(lcd.PORT) |= 1 << lcd.D4;
    } else {
        *(lcd.PORT) &= ~(1 << lcd.D4);
    }

    if (c & 2) {
        *(lcd.PORT) |= 1 << lcd.D5;
    } else {
        *(lcd.PORT) &= ~(1 << lcd.D5);
    }

    if (c & 4) {
        *(lcd.PORT) |= 1 << lcd.D6;
    } else {
        *(lcd.PORT) &= ~(1 << lcd.D6);
    }

    if (c & 8) {
        *(lcd.PORT) |= 1 << lcd.D7;
    } else {
        *(lcd.PORT) &= ~(1 << lcd.D7);
    }
}

void lcd_write(unsigned char c) {
    *(lcd.PORT) &= ~(1 << lcd.RS); // => RS = 0
    lcd_out(c);

    *(lcd.PORT) |= 1 << lcd.EN; // => E = 1
    __delay_ms(4);
    *(lcd.PORT) &= ~(1 << lcd.EN); // => E = 0
}

bool lcd_init(lcd_t display) {
    lcd = display;

    // The function should clear only the appropriate bits, not the whole PORT

    if (lcd.PORT == &PORTA) {
        TRISA = 0x00;
    } else if (lcd.PORT == &PORTB) {
        TRISB = 0x00;
    } else if (lcd.PORT == &PORTC) {
        TRISC = 0x00;
    } else if (lcd.PORT == &PORTD) {
        TRISD = 0x00;
    } else if (lcd.PORT == &PORTE) {
        TRISE = 0x00;
    } else {
        return false;
    }

    // Give some time to the LCD to start function properly
    __delay_ms(20);

    // Send reset signal to the LCD
    lcd_write(0x03);
    __delay_ms(5);
    lcd_write(0x03);
    __delay_ms(16);
    lcd_write(0x03);

    // Specify the data lenght to 4 bits
    lcd_write(0x02);

    // Set interface data length to 8 bits, number of display lines to 2 and font to 5x8 dots
    lcd_cmd(0x28);

    // Set cursor to move from left to right
    lcd_cmd(0x06);

    lcd_display(true, false, false); // Turn on display and set cursor off

    lcd_clear();

    return true;
}

void lcd_putc(char c) {

    *(lcd.PORT) |= 1 << lcd.RS; // => RS = 1
    lcd_out((c & 0xF0) >> 4); //Data transfer

    *(lcd.PORT) |= 1 << lcd.EN;
    __delay_us(40);
    *(lcd.PORT) &= ~(1 << lcd.EN);

    lcd_out(c & 0x0F);

    *(lcd.PORT) |= 1 << lcd.EN;
    __delay_us(40);
    *(lcd.PORT) &= ~(1 << lcd.EN);
}

void lcd_putc_at(const size_t x, const size_t y, char a) {
    lcd_set_cursor(x, y);
    lcd_putc(a);
}

void lcd_puts(char *a) {
    for (int i = 0; a[i] != '\0'; ++i)
        lcd_putc(a[i]);
}

void lcd_puts_at(const size_t x, const size_t y, char *a) {
    lcd_set_cursor(x, y);
    lcd_puts(a);
}

void lcd_putrs(const char *a) {
    for (int i = 0; a[i] != '\0'; ++i)
        lcd_putc(a[i]);
}

void lcd_putrs_at(const size_t x, const size_t y, const char *a) {
    lcd_set_cursor(x, y);
    lcd_putrs(a);
}