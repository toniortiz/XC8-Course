// incluir archivos os librerias necesarias
#include <xc.h>
#include <stdint.h>

// configuracion de los bits especiales
#include "configuration_bits.h"

// velocidad del oscilador, macro usada por la macro __delay_ms
#define _XTAL_FREQ 8000000

// Declariacion de prototipo de funciones y variables globales

// funcion main

void main(void) {
    // configuracion inicial del modo de trabajo de micro
    OSCCON = 0x76;

    // Configuracion de todos los pines como I/O digitales
    ADCON1 = 0x0F;

    // TRIS -> configurar pin como entrada o salida digital
    // PORT -> Lee los niveles de voltaje del pin
    // LAT -> Lee o escribe el registro LATCH el pin
    //    TRISAbits.RA0 = 1; // entrada digital
    //    TRISAbits.RA1 = 0; // salida digital
    //
    //    LATAbits.LA1 = 0; // limpiar el bit

    TRISB = 0x00; // puerto B como salida
    LATB = 0x00; // limpiar puerto B (todos los pines a 0)

    // entradas digitales
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;

    //    uint8_t mask = 0b11110000;
    uint16_t value = 0x01;

    // ciclo principal
    while (1) {

        //        LATAbits.LA1 = ~LATAbits.LA1;
        //        LATB = ~mask;
        //        __delay_ms(10);
        //        mask = LATB;

        // si ambos botones son igual a 0
        if (PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0) {
            // corrimiento hacia la izquierda
            LATB = value;
            __delay_ms(10);

            value <<= 2; //value = value << 1;
            // value >>= 1; corrimiento a la derecha
            if (value > 0b10000000) {
                value = 0b00000001;
            }
        }
    }

    return;
}

// Implementacion de las funciones
