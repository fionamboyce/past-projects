#define _XTAL_FREQ 8000000
#include <xc.h> 
#include <stdio.h> 

//==============================================================================
// Submission by Danny O'Sullivan (01057419) and Fiona Boyce (00817980)
//==============================================================================

void IRInit(void) {
    TRISAbits.RA2 = 1;

    ANSEL0 = 0;
    PIR3bits.IC1IF = 0; //setting IC1 flag to zero

    PIE3bits.IC1IE = 1;
    IPR3bits.IC1IP = 1; //set priority to high

    T5CON = 0b00001101; //setting up Timer 5 for CAP 
    CAP1CON = 0b01000110; // Setting CAP1 to PWM measure mode falling-to -rising edge

}

