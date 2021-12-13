#ifndef PWM_1_H
#define	PWM_1_H

#include <xc.h>   
#include <inttypes.h>
#include "config.h"
/**
 * Configura el ccp1 como pwm
 * @param pr2 Valor del registro PR2
 * @param prescaler_tmr2 Prescaler del timer 2 {1, 4, 16}
 */
void config_pwm1(uint8_t pr2, uint8_t prescaler_tmr2);

void set_ct_pwm1(uint16_t ct);


#endif	

