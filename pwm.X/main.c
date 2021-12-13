#include <xc.h>
#include <stdint.h>
#include "fuses.h"
#include <math.h>

#define _XTAL_FREQ 8000000 
#define TMR2_P 16
#define F_PWM 2000
#define MAX_DC _XTAL_FREQ/(F_PWM*TMR2_P)

typedef union {
    uint16_t lpwm;
    uint8_t bpwm[2];
} pwmdc_t;

void pwm1_dc(uint16_t duty);

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    uint16_t dc = 0;


    // 1. Set the PWM period by writing to the PR2 register
    PR2 = ceil(61.5);

    // 2. Set the PWM duty cycle by writing to the CCPRxL register and
    // CCPxCPM<4:5> bits
    pwm1_dc(dc);

    // 3. Make the CCPx pin an output by clearing the appropriate TRIS bit
    TRISCbits.RC2 = 0;

    // 4. Set the TMR2 prescale value, the enable Timer2 by writing
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.TMR2ON = 1;

    // 5. Configure the CCPx module PWM operation
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;

    while (1) {
        pwm1_dc(dc);
        dc += 5;
        __delay_ms(25);
        if (dc > 1024) dc = 0;
    }

    return;
}

void pwm1_dc(uint16_t duty) {
    if (duty < 1024) {

        duty = ((float) duty / 1023.0) * MAX_DC;

        pwmdc_t DCycle;

        // Save the dutycycle value in the union
        DCycle.lpwm = duty << 6;

        // Write the high byte into CCPR1L
        CCPR1L = DCycle.bpwm[1];

        // Write the low byte into CCP1CON5:4
        CCP1CON = (CCP1CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);
    }
}
