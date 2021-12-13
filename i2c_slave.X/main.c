#include <xc.h>
#include "fuses.h"

#define _XTAL_FREQ 8000000

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;


    while (1) {

    }

}
