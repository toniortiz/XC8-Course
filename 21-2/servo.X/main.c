#include <xc.h>
#include "config.h"
#include <inttypes.h>
#include "pwm1.h"
#include <ctype.h>
#include "lcd.h"
#include <stdio.h>

volatile uint16_t valor = 0;
float pos_actual = 0;
float ciclo_trabajo = 0;

unsigned char leer_teclado() {
    unsigned char tecla = ' ';

    switch (PORTE) {
        case 0: tecla = '1'; // tecla = 49
            break;
        case 1:tecla = '2';
            break;
        case 2: tecla = '3'; // tecla = 51
            break;
        case 3: tecla = 'A';
            break;
        case 4: tecla = '4';
            break;
        case 5: tecla = '5';
            break;
        case 6: tecla = '6';
            break;
        case 7: tecla = 'B';
            break;
        case 8: tecla = '7';
            break;
        case 9:tecla = '8';
            break;
        case 10: tecla = '9'; // tecla = 57
            break;
        case 11:tecla = 'C';
            break;
        case 12:tecla = '*';
            break;
        case 13: tecla = '0'; // tecla = 48
            break;
        case 14:tecla = '=';
            break;
        case 15: tecla = 'D';
            break;
        default: break;
    }

    return tecla;
}

void angulo_servo(uint8_t ang) {
    if (ang >= 0 && ang <= 180) {
        float dif = ang - pos_actual;

        float pasos = dif * 0.281;
        ciclo_trabajo += pasos;

        if (ciclo_trabajo > 104)
            ciclo_trabajo = 104;
        else if (ciclo_trabajo < 52)
            ciclo_trabajo = 52;

        pos_actual = ang;

        set_ct_pwm1(ciclo_trabajo);
    }
}

void int_ext0() {
    if (INTCONbits.INT0IF == 1) {
        unsigned char tecla = leer_teclado();

        if (isdigit(tecla)) {
            valor = valor * 10 + (tecla - 48);
            char buf[6];
            sprintf(buf, "%d", valor);
            lcd_putrs_at(0, 0, buf);
        } else if (tecla == 'A') {
            if (valor >= 0 && valor <= 180) {
                angulo_servo(valor);
            }else{
                lcd_putrs_at(0, 0, "Invalido");
                __delay_ms(1000);
            }
            valor = 0;
            lcd_clear();
            lcd_return_home();
        }

        INTCONbits.INT0IF = 0;
    }
}

void __interrupt() interrupts() {
    int_ext0();
}

void main(void) {
    OSCCON = 0x36; // Oscilador interno a 500 kHz
    ADCON1 = 0x0F;

    config_pwm1(155, 16);

    lcd_t lcd = {&PORTD, 2, 3, 4, 5, 6, 7};
    lcd_init(lcd);

    PORTE = 0x0F; // puerto E como entrada digital

    // INT0
    TRISBbits.RB0 = 1;
    INTCONbits.INT0IE = 1; // habilitat int0
    INTCON2bits.INTEDG0 = 1; // flanco de subida
    INTCONbits.INT0IF = 0; // limpiar bandera

    INTCONbits.GIE = 1;

    // Poner el servo en la posición 0°
    set_ct_pwm1(52);
    ciclo_trabajo = 52;
    __delay_ms(1000);

    while (1) {
    }
    return;
}
