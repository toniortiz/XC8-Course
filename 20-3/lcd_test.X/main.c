#include <xc.h> 
#include "fuses.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 8000000

char dec_to_hex(int digit) {
    return "0123456789ABCDEF"[digit];
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    lcd_init();

    lcd_command(LCD_CLEAR);
    lcd_command(LCD_CURSOR_OFF);

    lcd_const_text(1, 1, "Antonio");
    __delay_ms(3000);

    //    for (int i = 0; i < 7; i++) {
    //        lcd_command(LCD_SHIFT_LEFT);
    //        __delay_ms(300);
    //    }

    lcd_command(LCD_CLEAR);

    char buf[10] = "abc";
    //    for (int i = 0; i < 20; i++) {
    //        sprintf(buf, "i = %d", i);
    //        lcd_text(1, 1, buf);
    //        __delay_ms(300);
    //    }


    lcd_command(LCD_RETURN_HOME);
    for (int i = 0; i < 16; i++) {
        lcd_char_cp(dec_to_hex(i));
        __delay_ms(300);
    }

    // copiar strings
    char buf2[10];
    strcpy(buf2, buf);
    lcd_command(LCD_CLEAR);
    lcd_text(1, 1, buf2);


    //        lcd_command(LCD_CLEAR);
    //        lcd_command(LCD_RETURN_HOME);
    //    
    //        lcd_char_cp('A');

    while (1) {

    }


    return;
}
