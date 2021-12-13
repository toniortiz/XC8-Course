#ifndef TIMERS_H
#define	TIMERS_H

#include <stdint.h>
#include <stdbool.h>

/* used to hold 16-bit timer value */
typedef union Timers {
    uint16_t lt;

    struct {
        unsigned L : 8;
        unsigned H : 8;
    };
} Timers_t;

// Timer 0
void open_t0(uint8_t config, bool enint);
void close_t0();
uint16_t read_t0();
void write_t0(uint16_t timer0);


// Timer 1
void open_t1(uint8_t config, bool enint);
void close_t1();
uint16_t read_t1();
void write_t1(uint16_t timer1);

#endif

