#ifndef LCD_H
#define	LCD_H

#include <stdbool.h>

typedef struct {
    volatile unsigned char* PORT; // Pointer to the LCD port e.g &PORTC
    unsigned RS : 3; // The RS bit of the LCD PORT e.g. 2
    unsigned EN : 3; // The EN bit of the LCD PORT e.g. 3
    unsigned D4 : 3; // The D4 bit of the LCD PORT e.g. 4
    unsigned D5 : 3; // The D5 bit of the LCD PORT e.g. 5
    unsigned D6 : 3; // The D6 bit of the LCD PORT e.g. 6
    unsigned D7 : 3; // The D7 bit of the LCD PORT e.g. 7
} lcd_t;

extern lcd_t lcd;

// Macros that correspond to LCD commands
#define lcd_clear() lcd_cmd(0x01)
#define lcd_return_home() lcd_cmd(0x02)
#define lcd_cursor_off() lcd_cmd(0x0C)
#define lcd_cursor_steady() lcd_cmd(0x0E)
#define lcd_cursor_blink() lcd_cmd(0x0F)
#define lcd_decrement_cursor() lcd_cmd(0x04)
#define lcd_increment_cursor() lcd_cmd(0x05)
#define lcd_shift_display_right() lcd_cmd(0x06)
#define lcd_shift_display_left() lcd_cmd(0x07)
#define lcd_shift_right() lcd_cmd(0x1C)
#define lcd_shift_left() lcd_cmd(0x18)

// Sets the LCD cursor position
#define lcd_set_cursor( x, y )           \
do {                                     \
    if ( x == 0 ) {                      \
        lcd_cmd(0x80 + y);               \
    }                                    \
    else if ( x == 1 ) {                 \
        lcd_cmd(0xC0 + y);               \
    }                                    \
    else if ( x == 2 ) {                 \
        lcd_cmd(0x94 + y);               \
    }                                    \
    else if ( x == 3 ) {                 \
        lcd_cmd(0xD4 + y);               \
    }                                    \
} while ( false )

// [Internal use only] Sets the display on/off, the cursor on/off and if it will blink or not
#define lcd_display( on, cursor, blink ) \
do {                                     \
    unsigned char cmd = 0x08;            \
                                         \
    if ( on == true ) {                  \
        cmd |= 1 << 2;                   \
    }                                    \
                                         \
    if ( cursor == true ) {              \
        cmd |= 1 << 1;                   \
    }                                    \
                                         \
    if ( blink == true ) {               \
        cmd |= 1;                        \
    }                                    \
                                         \
    lcd_cmd(cmd);                        \
} while ( false )

// [Internal use only] Sends a command to the LCD
#define lcd_cmd( c )                     \
do {                                     \
    lcd_write( (c & 0xF0) >> 4 );        \
    lcd_write( c & 0x0F);                \
} while ( false )

// Initializes the LCD (See LCD struct)
bool lcd_init(lcd_t display);

// Prints a character on the LCD
void lcd_putc(char a);
void lcd_putc_at(const size_t x, const size_t y, char a);

// Prints a string on the LCD that is allocated on the RAM
void lcd_puts(char *a);
void lcd_puts_at(const size_t x, const size_t y, char *a);

// Prints a string on the LCD that is allocated on the program memory
void lcd_putrs(const char *a);
void lcd_putrs_at(const size_t x, const size_t y, const char *a);

// [Internal use only] Write a byte to the LCD
void lcd_write(unsigned char c);

// [Internal use only] Outputs the data to the LCD Port
void lcd_out(char a);

#endif	/* LCD_H */