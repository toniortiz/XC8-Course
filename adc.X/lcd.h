#ifndef LCD_H
#define	LCD_H

#define _XTAL_FREQ 8000000 


//==============================================================================
// Lcd Connection.
// Set in  digital mode these pins in your code.
// Pin RW connect to GND.
//==============================================================================
#define LCD_RD7     PORTDbits.RD7       // D7
#define TRISRD7     TRISDbits.TRISD7

#define LCD_RD6     PORTDbits.RD6       // D6
#define TRISRD6     TRISDbits.TRISD6

#define LCD_RD5     PORTDbits.RD5       // D5
#define TRISRD5     TRISDbits.TRISD5

#define LCD_RD4     PORTDbits.RD4       // D4
#define TRISRD4     TRISDbits.TRISD4

#define LCD_EN      PORTDbits.RD3       // EN
#define TRISEN      TRISDbits.TRISD3

#define LCD_RS      PORTDbits.RD2       // RS
#define TRISRS      TRISDbits.TRISD2

//==============================================================================
// Available Lcd Commands.
//==============================================================================
#define      LCD_FIRST_ROW           128
#define      LCD_SECOND_ROW          192
#define      LCD_THIRD_ROW           148
#define      LCD_FOURTH_ROW          212
#define      LCD_CLEAR               1
#define      LCD_RETURN_HOME         2
#define      LCD_CURSOR_OFF          12
#define      LCD_UNDERLINE_ON        14
#define      LCD_BLINK_CURSOR_ON     15
#define      LCD_MOVE_CURSOR_LEFT    16
#define      LCD_MOVE_CURSOR_RIGHT   20
#define      LCD_TURN_OFF            0
#define      LCD_TURN_ON             8
#define      LCD_SHIFT_LEFT          24
#define      LCD_SHIFT_RIGHT         28

//==============================================================================
// Function Prototypes.
//==============================================================================
void lcd_init(void);
void lcd_const_text(unsigned char y, unsigned char x, const char *buffer);
void lcd_text(unsigned char y, unsigned char x, char *buffer);
void lcd_write_char(unsigned char y, unsigned char x, char c);
void lcd_char_cp(char c);
void lcd_command(unsigned char command);
void lcd_set_cursor(unsigned char y, unsigned char x);
void lcd_text_cp(const char *buffer);

#endif
