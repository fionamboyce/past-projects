// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef HEADER_H
#define	HEADER_H
#include <xc.h> // include processor files - each processor file is guarded.   

//==============================================================================
// Submission by Danny O'Sullivan (01057419) and Fiona Boyce (00817980)
//==============================================================================


//==============================================================================
// RFid GLOBALS
//==============================================================================
char ReadRFidBuf[16]; //character array to read the RFID information.
char *RF_p = &ReadRFidBuf[0]; // pointing alleviates need for extra counter variable
char EndApproach = 1; // Used as a flag that starts high, set low once the RFID is read, signaling the end of the approach.

//==============================================================================
// MOTOR GLOBAL VARIABLES AND PARAMETERS 
//==============================================================================

struct DC_motor { //definition (assigns memory) of DC_motor structure 
    char power; //motor power, out of 100
    char direction; //motor direction, forward(1), reverse(0)
    unsigned char *dutyLowByte; //PWM duty low byte address
    unsigned char *dutyHighByte; //PWM duty high byte address
    char dir_pin; // pin that controls direction on PORTB
    int PWM_base_period; //base period of PWM cycle
};

struct DC_motor motorL, motorR;
//==============================================================================
// NAVIGATION GLOBALS
//==============================================================================
char CommandDir=0;              //Command directions are 1 = left, 2 = right, 3 = forward. Used to remember route to trace back for return routine. 
char MovementTime=0;            //Records the time the robot spent during a move, in milliseconds. 
char MovementLog[2][50];        //Initialises the array for logging the moves, used for the return routine. 
char LogEntry=0;                // Index of the array for return routine. 
//==============================================================================
// IR GLOBALS   
//==============================================================================
unsigned int IRValue;           //IR reading
int IRDiff;                     //Difference between subsequent IR readings
unsigned int IR_prev;           // The previous IR reading.
unsigned int IR_buf;    
//==============================================================================
//MOTOR GLOBAL FUNCTIONS PROTOTYPES 
//==============================================================================
void MotorStartUp(void); //initialises both motor registers. 
void BothMotorForward(struct DC_motor *mL, struct DC_motor *mR, char pwr);
void setMotorSpeed(struct DC_motor *m, char pwr, char dir);
void stop(struct DC_motor *mL, struct DC_motor *mR);
void turnLeft(struct DC_motor *mL, struct DC_motor *mR);
void turnRight(struct DC_motor *mL, struct DC_motor *mR);
void BothMotorReverse(struct DC_motor *mL, struct DC_motor *mR, char pwr);
//==============================================================================
// LCD GLOBAL FUNCTION PROTOTYPES
//==============================================================================
void LCDStartUp(void); //initialises the LCD registers, sends an 'all ok' to ensure all is working. 
void SetLine(char line);
void LCD_String(char *string);
void Disp_Clear(void);
void SendLCD(unsigned char Byte, char type);
//==============================================================================
//IR GLOBAL FUNCTIONS
//==============================================================================
void IRInit(void); //initialises the registers for the IR sensor. 
//==============================================================================
// RDid  GLOBAL FUNCTIONS 
//==============================================================================
void USART_Init(void); // Initialises the serial communication with the RFID reader. 
void Filter_n_PrintRFid(void); //function to filter and print the RFID data to the LCD screen. 
//==============================================================================
// NAVIGATION GLOBAL FUNCTIONS
//==============================================================================
void Sweep4MaxIR(void);
void SearchForward(struct DC_motor *mL, struct DC_motor *mR);

void TimerInit(void);       // TMR0 used scaled to 0.1s
void TimerStart(void);
void TimerStop(void);

void ReturnToBase(void);
void ReturnTime(int time);
#endif	

