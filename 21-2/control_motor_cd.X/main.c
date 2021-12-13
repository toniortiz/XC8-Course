#include <xc.h>
#include "config.h"
#include "lcd.h"

#define _XTAL_FREQ 8000000

typedef enum {
    SH = 0, SA, OFF
} estado_motor;

volatile estado_motor estado = OFF;

void int_ext_0() {
    // Hace girar el motor en sentido horario
    if (INTCONbits.INT0IF == 1) {
        if (estado == SA) {
            // detener motor
            LATAbits.LA0 = 0;
            LATAbits.LA1 = 0;
            __delay_ms(100);
        }

        LATAbits.LA0 = 1;
        LATAbits.LA1 = 0;
        estado = SH;
        lcd_putrs_at(0, 7, "-> ");

        INTCONbits.INT0IF = 0;
    }
}

void int_ext_1() {
    // Hace girar el motor en sentido anti-horario
    if (INTCON3bits.INT1IF == 1) {
        if (estado == SH) {
            // detener motor
            LATAbits.LA0 = 0;
            LATAbits.LA1 = 0;
            __delay_ms(100);
        }

        LATAbits.LA0 = 0;
        LATAbits.LA1 = 1;
        estado = SA;
        lcd_putrs_at(0, 7, "<- ");

        INTCON3bits.INT1IF = 0;
    }
}

void int_ext_2() {
    // Detiene el motor
    if (INTCON3bits.INT2IF == 1) {
        LATAbits.LA0 = 0;
        LATAbits.LA1 = 0;
        estado = OFF;
        lcd_putrs_at(0, 7, "OFF");

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

    lcd_t lcd={&PORTD, 2, 3, 4, 5, 6, 7};
    lcd_init(lcd);

    // Control IN1 e IN2
    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;
    LATAbits.LA0 = 0;
    LATAbits.LA1 = 0;

    // INT0
    TRISBbits.RB0 = 1;
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 1;
    INTCONbits.INT0IF = 0;

    // INT1
    TRISBbits.RB1 = 1;
    INTCON3bits.INT1IE = 1;
    INTCON2bits.INTEDG1 = 1;
    INTCON3bits.INT1IF = 0;

    // INT2
    TRISBbits.RB2 = 1;
    INTCON3bits.INT2IE = 1;
    INTCON2bits.INTEDG2 = 1;
    INTCON3bits.INT2IF = 0;

    INTCONbits.GIE = 1;
    
    lcd_putrs_at(0, 0, "Estado:OFF");
    

    while (1) {

    }

    return;
}
