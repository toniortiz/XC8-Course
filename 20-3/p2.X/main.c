#include <xc.h>

int row = 1;
int col = 1;

// recorrer a la izquierda el cursor

void int_ext0() {
    // row = 1; 
    // col = 7;
    col--;
    if (row == 1 && col == 16) {
        row = 2;
        col = 1;
    }

    lcd_set_cursor(row, col);

}

// reccorre a la derecha

void int_ext1() {
    col++;
    lcd_set_cursor(row, col);
}

// muestra las letras
void int_ext2(){
    lcd_char_cp(caracter);
    lcd_set_cursor(row, col);
}

void main(void) {
    return;
}
