
#define _XTAL_FREQ 8000000
#include <xc.h> 
#include <stdio.h> 
#include "HEADER.h"
#define PWMcycle 199;
#include <stdio.h>
#pragma config OSC = IRCIO, WDTEN=OFF, MCLRE=OFF, LVP=OFF

//==============================================================================
// Submission by Danny O'Sullivan (01057419) and Fiona Boyce (00817980)
//==============================================================================

//==============================================================================
//LOW LEVEL MOTOR FUNCTIONS 
//==============================================================================

void initPWM(void) {            // PWM base calculations are hard coded watch out
    TRISBbits.RB1 = 0;          //PWM1
    TRISBbits.RB0 = 0;
    LATBbits.LATB0 = 0;

    TRISBbits.RB3 = 0;          //PWM3
    TRISBbits.RB2 = 0;
    LATBbits.LATB2 = 0;

    PTCON0 = 0b00000000;        // free running mode, 1:64 prescaler = 32 us
    PTCON1 = 0b10000000;        // enable PWM timer

    PWMCON0 = 0b01101111;       // PWM0/1 enabled, all independent mode
    PWMCON1 = 0x00;             // special features, all 0 (default)

 
     PTPERL = PWMcycle          // base PWM period low byte

    PTPERH = 0x00;                              

}

void BothMotorInit(void) {
    motorL.power = 0;                                       //zero power to start
    motorL.direction = 1;                                   //set default motor direction
    motorL.dutyLowByte = (unsigned char *) (&PDC1L);        //store address of PWM duty low byte
    motorL.dutyHighByte = (unsigned char *) (&PDC1H);       //store address of PWM duty high byte
    motorL.dir_pin = 2;                                     //pin RB0/PWM0 controls direction
    motorL.PWM_base_period = PWMcycle;                      //store PWMperiod for motor

    motorR.power = 0;                                       //zero power to start
    motorR.direction = 1;                                   //set default motor direction
    motorR.dutyLowByte = (unsigned char *) (&PDC0L);        //store address of PWM duty low byte
    motorR.dutyHighByte = (unsigned char *) (&PDC0H);       //store address of PWM duty high byte
    motorR.dir_pin = 0;                                     //pin RB0/PWM0 controls direction
    motorR.PWM_base_period = PWMcycle;                      //store PWMperiod for motor
}

void setMotorPWM(struct DC_motor *m_p) {

    int PWMduty;                           //temporary variable to store PWM duty cycle

    if (m_p->direction) {                  //if forward low time increases with power
        PWMduty = m_p->PWM_base_period - ((int) (m_p->power)*(m_p->PWM_base_period)) / 100;
    }
    
    else {                                 //if reverse high time increases with power
        PWMduty = ((int) (m_p->power)*(m_p->PWM_base_period)) / 100;
    }

    PWMduty = (PWMduty << 2);             // two LSBs are reserved for other things

    *(m_p->dutyLowByte) = PWMduty & 0xFF; //set low duty cycle byte
    *(m_p->dutyHighByte) = (PWMduty >> 8) & 0x3F; //set high duty cycle byte

    if (m_p->direction) {                  // if direction is high,
        LATB = LATB | (1 << (m_p->dir_pin)); // set dir_pin bit in LATB to high
    } 
    
    else {                                  // if direction is low,
        LATB = LATB & (~(1 << (m_p->dir_pin))); // set dir_pin bit in LATB to low
    }
}

void setMotorSpeed(struct DC_motor *m, char pwr, char dir) {
    m->power = pwr;
    m->direction = dir;

    setMotorPWM(m);
}


//==============================================================================
// HIGHER LEVEL FUNCTION USE TO CONTROL ROBOT MORE EASILY
//==============================================================================

void MotorStartUp(void) {
    OSCCON = 0x72; //8MHz clock
    while (!OSCCONbits.IOFS);

    BothMotorInit();
    initPWM();

}

void BothMotorForward(struct DC_motor *mL, struct DC_motor *mR, char pwr) {
    // this fcn contains ramping up and down power
    char i;

    if (pwr > mL->power) {
        for (i = mL->power; i < pwr + 1; i++) {
            setMotorSpeed(mL, i, 1);
            setMotorSpeed(mR, i, 1);
            __delay_ms(10);
        }
    } 
    
    else{
        for (i = mL->power; i > pwr; i--) {
            setMotorSpeed(mL, i, 1);
            setMotorSpeed(mR, i, 1);
            __delay_ms(10);
        }
    }
}

void BothMotorReverse(struct DC_motor *mL, struct DC_motor *mR, char pwr) {
    // this fcn contains ramping up and down power
    char i;

    if (pwr > mL->power) {
        for (i = mL->power; i < pwr + 1; i++) {
            setMotorSpeed(mL, i, 0);
            setMotorSpeed(mR, i, 0);
            __delay_ms(10);
        }
    } 
    
    else{
        for (i = mL->power; i > pwr; i--) {
            setMotorSpeed(mL, i, 0);
            setMotorSpeed(mR, i, 0);
            __delay_ms(10);
        }
    }
}



void stop(struct DC_motor *mL, struct DC_motor *mR) {
   
   setMotorSpeed(mL, 0, 1);
   setMotorSpeed(mR, 0, 1);
}

void turnLeft(struct DC_motor *mL, struct DC_motor *mR) 
{

   setMotorSpeed(mL, 40, 0);
   setMotorSpeed(mR, 40, 1);
}

void turnRight(struct DC_motor *mL, struct DC_motor *mR) 
{
   setMotorSpeed(mL, 40, 1);
   setMotorSpeed(mR,40, 0);
}


