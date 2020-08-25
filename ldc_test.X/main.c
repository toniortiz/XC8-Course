#include <xc.h>
#include <stdio.h>
#include "fuses.h"
#include <stdlib.h>
#include <string.h>
#include "lcd.h"

#define _XTAL_FREQ 8000000

char digit_to_hex_char(int digit) {
    return "0123456789ABCDEF"[digit];
}

int count_spaces(const char s[]) {
    int count = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == ' ')
            count++;
    }
    return count;
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    lcd_init();
    lcd_command(LCD_CLEAR);
    lcd_command(LCD_BLINK_CURSOR_ON);

    lcd_text(1, 1, "Antonio Ortiz");
    lcd_set_cursor(2, 8);
    lcd_text_cp("AOG");


    for (int i = 1; i <= 2; i++) {
        for (int j = 1; j <= 16; j++) {
            lcd_set_cursor(i, j);
            __delay_ms(1000);
        }

    }


    //    //    char buf[20];
    //    //
    //    //    for (int i = 0; i < 20; i++) {
    //    //        sprintf(buf, "%d   ", i);
    //    //        lcd_text(1, 1, buf);
    //    //        __delay_ms(300);
    //    //
    //    //    }
    //    //
    //    //
    //    //    lcd_command(LCD_CLEAR);
    //    //    lcd_const_text(1, 1, "LCD lib");
    //    //    lcd_const_text(2, 1, "Curso micros");
    //    //    __delay_ms(2000);
    //    //
    //    //    lcd_command(LCD_CLEAR);
    //    //    lcd_command(LCD_RETURN_HOME);
    //    //
    //    //    // In the array version, the characters can be modified, like the elements 
    //    //    // of any array. In the pointer version, it points to a string literal, and
    //    //    // that string literal shouldn't be modified.
    //    //    // In the array version, buf3 is an array name. In the pointer version, buf2
    //    //    // is a variable that can be made to point to other strings during program
    //    //    // execution. If we need a string that can be modified, it's our 
    //    //    // responsibility to set up an array of characters.
    //    //    //    char *buf2 = "micros";
    //    //    //    char buf3[] = "micros";
    //    //    //    while (*buf2) {
    //    //    //        lcd_char_cp(*buf2);
    //    //    //        buf2++;
    //    //    //        __delay_ms(400);
    //    //    //    }
    //    //
    //    //
    //    //    lcd_command(LCD_CLEAR);
    //    //    lcd_command(LCD_RETURN_HOME);
    //    //    for (int i = 0; i < 16; i++) {
    //    //        lcd_char_cp(digit_to_hex_char(i));
    //    //        __delay_ms(400);
    //    //    }
    //    //
    //    //    char date1[8] = "June 25"; // J u n e 2 5 \0
    //    //    char date2[9] = "June 25"; // J u n e 2 5 \0 \0
    //    //    char date3[] = "June 25";
    //    //    lcd_command(LCD_CLEAR);
    //    //    lcd_text(1, 1, date1);
    //    //
    //    //    lcd_command(LCD_CLEAR);
    //    //    char name[10] = " Antonio ";
    //    //    int count = count_spaces(name);
    //    //    char cnt[5];
    //    //    sprintf(cnt, "%d", count);
    //    //    lcd_text(1, 1, cnt);
    //
    //    char str2[10];
    //    char str3[10];
    //    // str2 = "abc"; wrong
    //    strcpy(str2, "abc");
    //    lcd_text(1, 1, str2);
    //
    //    strcpy(str3, str2);
    //    lcd_text(2, 1, str3);
    //    __delay_ms(1000);
    //
    //    lcd_command(LCD_CLEAR);
    //    size_t sz = strlen(str3);
    //    char szc[4];
    //    sprintf(szc, "%d", sz);
    //    lcd_text(1, 1, szc);
    //    __delay_ms(1000);
    //
    //    char str4[10] = "def";
    //    char* str5 = strcat(str2, str4);
    //    lcd_command(LCD_CLEAR);
    //    lcd_text(1, 1, str5);
    //    lcd_text(2, 1, str2);
    //    __delay_ms(1000);





    while (1) {

    }

}
