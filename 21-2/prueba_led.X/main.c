#include <xc.h>
#include "config.h"

void main(void) {
    OSCCON = 0x76;
    ADCON1 = 0x0F;
    
    TRISAbits.RA0 = 0;
    LATAbits.LA0 = 1;
    
    while(1){
        
    }
    
    return;
}
