#include <xc.h>
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>
#include <stdint.h>
#include "fuses.h"
#include "lcd.h"
#include <stdbool.h>

#define _XTAL_FREQ 8000000

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    lcd_t lcd = {&PORTD, 2, 3, 4, 5, 6, 7};
    lcd_init(lcd);

    uint16_t i16 = 0xFABC;
    uint8_t* ptr = (uint8_t*) & i16;

    char str[10];
    sprintf(str, "%04X", i16);
    lcd_puts_at(0, 0, str);

    memset(str, 0, sizeof (str));
    sprintf(str, "%02X", *ptr);
    lcd_puts_at(1, 0, str);

    memset(str, 0, sizeof (str));
    ptr++;
    sprintf(str, "%02X", *ptr);
    lcd_puts_at(1, 5, str);

    while (true) {
    }
}
