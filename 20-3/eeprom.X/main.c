#include <xc.h>
#include "fuses.h"
#include "lcd.h"
#include <stdint.h>
#include <string.h>

#define _XTAL_FREQ 8000000

char entrada[4];
uint8_t i = 0;

unsigned char leer_teclado();
void int_ext_0();
void limpiar_entrada();

// Guarda en dato el valor almacenado en la direccion dir de la eeprom
void leer_eeprom(uint8_t dir, uint8_t* dato);

// Escribe dato en la direccion dir de la eeprom
void escribir_eeprom(uint8_t dir, uint8_t dato);

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;

    TRISE = 0x0F; // pueroto E como entrada digital

    TRISAbits.RA0 = 0; // RA0 como salida digital
    LATAbits.LA0 = 0;

    // INT0
    TRISBbits.RB0 = 1;
    INTCONbits.INT0IE = 1; // habilitar int ext 0
    INTCONbits.INT0IF = 0; // limpiar bandera
    INTCON2bits.INTEDG0 = 1; // flanco de subida

    INTCONbits.GIE = 1; // habilita ints globales

    lcd_init();
    lcd_command(LCD_BLINK_CURSOR_ON);

    escribir_eeprom(0, 65); // 65 -> A
    escribir_eeprom(1, 66); // 66 -> B
    escribir_eeprom(2, 67); // 67 -> C

    // Prueba para verificar que los datos se guardaron en la eeprom
    //    leer_eeprom(0, &entrada[0]);
    //    leer_eeprom(1, &entrada[1]);
    //    leer_eeprom(2, &entrada[2]);
    //
    //    lcd_char_cp(entrada[0]);
    //    lcd_char_cp(entrada[1]);
    //    lcd_char_cp(entrada[2]);


    while (1) {

    }
    return;
}

void leer_eeprom(uint8_t dir, uint8_t* dato) {
    EEADR = dir;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    *dato = EEDATA;
}

void escribir_eeprom(uint8_t dir, uint8_t dato) {
    EEADR = dir;
    EEDATA = dato;
    EECON1bits.EEPGD = 0; // Acceder a la memoria EEPROM
    EECON1bits.CFGS = 0; // Acceso a la EEPROM
    EECON1bits.WREN = 1; // Habilita escritura
    INTCONbits.GIE = 0; // Deshabilita interrupciones
    EECON2 = 0x55; // Porque el datashet me lo ordenó
    EECON2 = 0xAA;
    EECON1bits.WR = 1; // Iniciar la escritura
    INTCONbits.GIE = 1; // Habilita interrupciones
    EECON1bits.WREN = 0; // Deshabilita escritura en la EEPROM

    // Espera mientras no se complete el ciclo de escritura de la EEPROM
    while (EECON1bits.WR == 1);
}

void limpiar_entrada() {
    // limpiar arreglo
    i = 0;
    for (int j = 0; j < 3; j++) {
        entrada[j] = ' ';
    }
}

void int_ext_0() {
    if (INTCONbits.INT0IF == 1) {
        unsigned char tecla = leer_teclado();

        if (tecla == '*') {
            // leer valores de la eeprom
            char clave[4];
            for (int i = 0; i < 3; i++) {
                leer_eeprom(i, &clave[i]);
            }

            if (strcmp(clave, entrada) == 0) {
                lcd_command(LCD_CLEAR);
                lcd_const_text(1, 1, "Clave correcta");
                LATAbits.LA0 = 1;
                __delay_ms(1000);
                LATAbits.LA0 = 0;
                lcd_command(LCD_CLEAR);

                limpiar_entrada();
            } else {
                lcd_command(LCD_CLEAR);
                lcd_const_text(1, 1, "Clave incorrecta");
                __delay_ms(1000);
                lcd_command(LCD_CLEAR);

                limpiar_entrada();
            }
        } else {
            if (i < 3) {
                entrada[i] = tecla;
                i++;
                lcd_char_cp(tecla);
            }
        }

        INTCONbits.INT0IF = 0; // limpiar bandera
    }
}

void __interrupt() interrupts() {
    int_ext_0();
}

unsigned char leer_teclado() {
    unsigned char tecla = ' ';

    switch (PORTE) {
        case 0: tecla = '1';
            break;
        case 1: tecla = '2';
            break;
        case 2: tecla = '3';
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
        case 9: tecla = '8';
            break;
        case 10: tecla = '9';
            break;
        case 11: tecla = 'C';
            break;
        case 12: tecla = '*';
            break;
        case 13: tecla = '0';
            break;
        case 14: tecla = '=';
            break;
        case 15: tecla = 'D';
            break;
        default: break;
    }

    return tecla;
}