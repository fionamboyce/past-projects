#define _XTAL_FREQ 8000000
#include <xc.h> 
#include <stdio.h> 
#include "HEADER.h"

//==============================================================================
// Submission by Danny O'Sullivan (01057419) and Fiona Boyce (00817980)
//==============================================================================

void Sweep4MaxIR(void) { 
    char EndCond = 1;
    char RightCond = 1;

    TimerStart(); //=============TIMER START====================================

    turnLeft(&motorL, &motorR);
    CommandDir = 1;

    while (EndCond) {
        
        if ((IRValue > 470)&&(IRValue < 510)) {
            EndCond = 0;
        } 
        
        else if ((IRDiff < -80) && RightCond) {
            TimerStop(); //=======TIMER STOP====================================

            TimerStart(); //======TIMER START===================================

            turnRight(&motorL, &motorR);
            CommandDir = 2;
            RightCond = 0; //ensures right turn is recorded once in log, and not every loop cycle
        }
    }

    TimerStop(); //=============TIMER STOP======================================
    stop(&motorL, &motorR);
}

void SearchForward(struct DC_motor *mL, struct DC_motor *mR) {
    char EndCond = 1;
    
    TimerStart(); //=========================TIMER START========================

    BothMotorForward(mL, mR, 60);
    CommandDir = 3; // Forward 
    __delay_ms(350);
    
    while ((IRDiff > -80) && EndCond) {
        
        if (!EndApproach) {
            
            stop(mL, mR);
            EndCond = 0;    
        } 
        
        else {
            BothMotorForward(mL, mR, 60);
        }
    }
    
    TimerStop(); //=====================TIMER START=============================
    stop(mL, mR);
}

void TimerInit(void) {
    INTCONbits.TMR0IE = 0;
    INTCON2bits.TMR0IP = 1;
    INTCONbits.TMR0IF = 0;

    T0CON = 0b10000001;         //enable TMR0, 8-bit, Fosc/4,T0SE not in use, Prescaler assigned, Prescaler = 4
    TMR0H = 0x3C;               //start value 15,536 on 65,356 TMR01
    TMR0L = 0xB2;
}

void TimerStart(void) {
    INTCONbits.TMR0IE = 1;
}

void TimerStop(void) {
    INTCONbits.TMR0IE = 0;

    MovementLog[0][LogEntry] = MovementTime;
    MovementLog[1][LogEntry] = CommandDir;
    LogEntry++;

    MovementTime = 0;
}

void Filter4Twirl(void) {
    char i;
    for (i = 49; i > 0; i--) {
        if (MovementLog[1][i] == 1 || MovementLog[1][i] == 2) {

            if (MovementLog[0][i] > 140) {
                MovementLog[0][i] = MovementLog[0][i] - 140;

            }
        }
    }
}

void ReturnToBase(void) {
    
    char ReturnEnd = 1;

    Filter4Twirl();

    do {
        if (LogEntry == 0) {
            ReturnEnd = 0;
        }

        if (MovementLog[1][LogEntry] == 1) {
            
            turnRight(&motorL, &motorR);
            ReturnTime(MovementLog[0][LogEntry]);
            
            stop(&motorL, &motorR);
            LogEntry--;
        } 
        
        else if (MovementLog[1][LogEntry] == 2) {
            
            turnLeft(&motorL, &motorR);
            ReturnTime(MovementLog[0][LogEntry]);
            
            stop(&motorL, &motorR);
            LogEntry--;
        } 
        
        else if (MovementLog[1][LogEntry] == 3) {

            BothMotorReverse(&motorL, &motorR, 58);
            ReturnTime(MovementLog[0][LogEntry]);
            
            stop(&motorL, &motorR);
            LogEntry--;
        }
        
        else {
            LogEntry--;   // accounts for zero entries in MovementLog
        }
        
    } while (ReturnEnd);
}

void ReturnTime(int time) {
    int i;
    for (i = 0; i < time; i++) {
        __delay_ms(100);

    }
}


















