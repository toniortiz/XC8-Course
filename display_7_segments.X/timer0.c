#include <p18cxxx.h>
#include "timers.h"

void open_t0(uint8_t config, bool enint) {
    T0CON = (0x7f & config); // Configure timer, but don't start it yet
    TMR0H = 0; // Reset Timer0 to 0x0000
    TMR0L = 0;
    INTCONbits.TMR0IF = 0; // Clear Timer0 overflow flag

    if (enint)
        INTCONbits.TMR0IE = 1; // Enable Timer0 overflow interrupt
    else
        INTCONbits.TMR0IE = 0;

    T0CONbits.TMR0ON = 1; // Start Timer0
}

void close_t0() {
    T0CONbits.TMR0ON = 0; // Disable Timer0
    INTCONbits.TMR0IE = 0; // Disable Timer0 overflow interrupts
}

uint16_t read_t0() {
    Timers_t timer;

    timer.L = TMR0L;
    timer.H = TMR0H;

    return timer.lt;
}

void write_t0(uint16_t timer0) {
    Timers_t timer;

    timer.lt = timer0;

    TMR0L = timer.L;
    TMR0H = timer.H;
}