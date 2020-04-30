#define _XTAL_FREQ 8000000
#include <xc.h> 
#include <stdio.h> 
#include "HEADER.h"

//void __interrupt(high_priority) GetPWMLowValue() {
//    if (PIR3bits.IC1IF==1) 
//    {
//        SendLCD('a',1);
//        PIR3bits.IC1IF=0;
//        __delay_ms(1000);
//    }
//}
