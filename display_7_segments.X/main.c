#include <xc.h>
#include "fuses.h"
#include <stdint.h>
#include "timers.h"

#define _XTAL_FREQ 8000000

const uint16_t t0 = 45536;

volatile uint16_t counter = 0;
volatile uint8_t a, b, c, d;

//typedef enum {
//    A = 0, B, C, D
//} digitset_t;

//digitset_t next = A;

void set_value(uint8_t val, uint8_t pos) {
    LATB = (LATB & 0xCF) | pos;
    LATB = (LATB & 0xF0) | val;
}

void int_timer0() {
    if (!INTCONbits.TMR0IF)
        return;

    switch (LATB & 0b00110000) {
        case 0b00110000:
            set_value(a, 0x00);
            //next = B;
            break;
        case 0:
            set_value(b, 0b00010000);
            //next = C;
            break;
        case 0b00010000:
            set_value(c, 0b00100000);
            //next = D;
            break;
        case 0b00100000:
            set_value(d, 0b00110000);
            // next = A;
            break;
        default: break;
    }

    write_t0(t0);
    INTCONbits.TMR0IF = 0;
}

void __interrupt() interrupts() {
    int_timer0();

}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISB = 0b11000000;

    open_t0(0b00001000, true);
    write_t0(t0);

    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    while (1) {
        a = counter / 1000; // holds 1000's digit
        b = ((counter / 100) % 10); // holds 100's digit
        c = ((counter / 10) % 10); // holds 10th digit
        d = (counter % 10); // holds unit digit value

        if (counter >= 9999) counter = 0;
        else counter++;

        __delay_ms(100);
    }
}
