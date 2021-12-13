#include "timers.h"
#include <p18cxxx.h>

void open_t1(uint8_t config, bool enint) {
    T1CON = (0xfe & config);

    TMR1H = 0;
    TMR1L = 0;
    PIR1bits.TMR1IF = 0;

    PIE1bits.TMR1IE = enint;

    T1CONbits.TMR1ON = 1;

}

void close_t1() {
    T1CONbits.TMR1ON = 0;
    PIE1bits.TMR1IE = 0;
}

uint16_t read_t1() {
    Timers_t timer;

    timer.L = TMR1L;
    timer.H = TMR1H;

    return timer.lt;
}

void write_t1(uint16_t timer1) {
    Timers_t t1;

    t1.lt = timer1;

    TMR1H = t1.H;
    TMR1L = t1.L;

}