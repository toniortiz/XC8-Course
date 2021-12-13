#include <xc.h>
#include "fuses.h"
#include "timers.h"

#define _XTAL_FREQ 8000000 

const uint16_t t0 = 65036 + 23;
const uint16_t t1 = 65036;

void int_timer0() {
    if (!INTCONbits.TMR0IF)
        return;

    LATBbits.LB0 = ~LATBbits.LB0;

    write_t0(t0);
    INTCONbits.TMR0IF = 0;
}

void int_timer1() {
    if (!PIR1bits.TMR1IF)
        return;

    LATBbits.LB1 = ~LATBbits.LB1;

    write_t1(t1);
    PIR1bits.TMR1IF = 0;
}

void __interrupt() interrupts() {

    int_timer0();

    int_timer1();
}

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISBbits.RB0 = 0;
    LATBbits.LB0 = 0;

    TRISBbits.RB1 = 0;
    LATBbits.LB0 = 0;

    // timer 0
    open_t0(0b00000000, true);
    write_t0(t0);

    // timer 1
    open_t1(0b00010000, true);
    write_t1(t1);

    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    while (1) {

    }

    return;
}
