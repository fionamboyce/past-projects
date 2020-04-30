#include <xc.h>
#include "HEADER.h"
#include <stdio.h>
#include <string.h>
#define _XTAL_FREQ 8000000
#pragma config OSC = IRCIO, WDTEN=OFF, MCLRE=OFF, LVP=OFF

//==============================================================================
// Submission by Danny O'Sullivan (01057419) and Fiona Boyce (00817980)
//==============================================================================


void __interrupt(high_priority) HP_Interrupt() {

    if (PIR3bits.IC1IF) {                       // IR Sensor Flag
        IR_buf = ((CAP1BUFH << 8) + CAP1BUFL);  // Interrupt triggered every 250 ms
                                                // Allows for most up to date IR readings
        IRValue = IR_buf / 100;                 // Filters out noisey fluctuations

        IRDiff = IRValue - IR_prev; 

        IR_prev = IRValue;
        
        PIR3bits.IC1IF = 0;
    }
    
    if (PIR1bits.RCIF) {            // RFid reading

     
        *RF_p = RCREG;             
        RF_p++; 
        
        EndApproach = 0;  // Key Flag that indicates end of approach and quits approach loop
        PIE3bits.IC1IE = 0;
        
    }

    if (INTCONbits.TMR0IF) {        // Timer Interrupt - for measuring time each robot move
        MovementTime++;             

        TMR0H = 0x3C;               //start value 15,536 on 65,356 TMR01 - scales for 0.1 s 
        TMR0L = 0xB2;
        INTCONbits.TMR0IF = 0;

    }
}

void main(void) {

    INTCONbits.GIEH = 1; // enable all global interrupts 
    RCONbits.IPEN = 1;   // no priority, all interrupts high - low does nothing
    TRISAbits.RA2 = 1;   // set up for IR measurement  

    ANSEL0 = 0;         // ensures pin receiving IR signal is is digital hi/low mode

    PIE1bits.RCIE = 1;  // receive data interrupt enable bit
    IPR1bits.RC1IP = 1; // receive interrupt priority
    
    
    MotorStartUp();
    LCDStartUp();
    IRInit();
    USART_Init();
    TimerInit();
    

    while (EndApproach) {       // Used as a flag which is triggered once the RFID is read. 
       
       Sweep4MaxIR();           // Function to perform an initial sweep for the maximum IR signal. 

        SearchForward(&motorL, &motorR); // moves towards the IR signal 

    }

    ReturnToBase();             // triggered once the RFID is read, robot returns to initial position.

    Disp_Clear();               // clears the screen ready for RFID

    Filter_n_PrintRFid();       // filters the RFID signal and prints the required information on the LCD screen.
    

    while (1);
}



