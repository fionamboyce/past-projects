#define _XTAL_FREQ 8000000
#include <xc.h> 
#include <stdio.h> 
#include "HEADER.h"

//==============================================================================
// Submission by Danny O'Sullivan (01057419) and Fiona Boyce (00817980)
//==============================================================================

void USART_Init(void) {
    TRISCbits.RC7 = 1;
    TRISCbits.RC6 = 1;

    OSCCON = 0b11110010;
    while (!OSCCONbits.IOFS);

    SPBRG = 203; //set baud rate to 9600
    SPBRGH = 0;

    BAUDCONbits.BRG16 = 1;
    TXSTAbits.BRGH = 1;
    RCSTAbits.CREN = 1;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;

}

void Filter_n_PrintRFid(void) {
    char i;
    SetLine(1);

    for (i = 1; i < 11; i++) {
        
        SendLCD(ReadRFidBuf[i], 1);
        
    }
}
