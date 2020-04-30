#define _XTAL_FREQ 8000000
#include <xc.h> 
#include <stdio.h> 

//==============================================================================
// Submission by Danny O'Sullivan (01057419) and Fiona Boyce (00817980)
//==============================================================================


//==============================================================================
// LOW LEVEL LCD FUNCTIONS
//==============================================================================

void LCD_pin_setup(void) {
    LATDbits.LATD0 = 0;
    LATDbits.LATD1 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC2 = 0;
    LATCbits.LATC0 = 0;
    LATAbits.LATA6 = 0;

    TRISCbits.RC0 = 0; //enable
    TRISCbits.RC1 = 0; //DB4
    TRISCbits.RC2 = 0; //DB5
    TRISDbits.RD0 = 0; // DB6 
    TRISDbits.RD1 = 0; //DB7
    TRISAbits.RA6 = 0;
}

void E_TOG(void) {
    LATCbits.LATC0 = 1;
    __delay_us(10);

    LATCbits.LATC0 = 0;
    __delay_us(10);
}

void LCDout(unsigned char number) { //function to send four bits to the LCD
    LATDbits.LATD1 = (number & 0b1000) >> 3;
    LATDbits.LATD0 = (number & 0b0100) >> 2;
    LATCbits.LATC2 = (number & 0b0010) >> 1;
    LATCbits.LATC1 = number & 0b0001;
    E_TOG(); //timing pulse
    __delay_us(5);
}

void SendLCD(unsigned char Byte, char type) {// set RS pin whether it is a Command (0) or Data/Char (1)
    //set data-pins using the four bits from number
    //toggle the enable bit to send data
    //function to send data/commands over a 4bit interface

    LATAbits.LATA6 = type;

    unsigned char nibble1, nibble2;  //4-bits known as nibble

    nibble1 = Byte & 0b00001111;
    nibble2 = Byte & 0b11110000;
    nibble2 = nibble2 >> 4;

    LCDout(nibble2);        // send high bits of Byte using LCDout function
    __delay_us(10); 
   
    LCDout(nibble1);        // send low bits of Byte using LCDout function
    __delay_us(50);
}

void LCD_Init(void) {
    __delay_ms(15);
    LCDout(0b0011);
    __delay_us(5);

    LCDout(0b0011);
    __delay_us(200);

    LCDout(0b0011);
    __delay_us(50);

    LCDout(0b0010);
    __delay_us(50);


    SendLCD(0b00101000, 0);
    __delay_us(50);

    SendLCD(0b00001000, 0); //display
    __delay_us(50);

    SendLCD(0b00000001, 0);
    __delay_ms(2);

    SendLCD(0b00000110, 0);
    __delay_us(50);

    SendLCD(0b00001110, 0);
    __delay_us(50);
}


void SetLine(char line) {       //Sets cursor on line 1 or 2 of LCD 
    if (line == 1) {
        SendLCD(0x80, 0);
    }
    if (line == 2) {
        SendLCD(0xC0, 0);
    }
    __delay_us(50); 
}

//==============================================================================
//HIGH LEVEL LCD FUNCTIONS
//==============================================================================

void Disp_Clear(void) {
    SendLCD(0b00000001, 0);
    __delay_ms(5);
}

void LCDStartUp(void) {
    OSCCON = 0x72; //8MHz clock
    while (!OSCCONbits.IOFS);
    LCD_pin_setup();
    LCD_Init();
    SetLine(1);
    Disp_Clear();

    SendLCD('a', 1);
    SendLCD('l', 1);
    SendLCD('l', 1);
    SendLCD(' ', 1);
    SendLCD('o', 1);
    SendLCD('k', 1);
    char i;
    for (i = 0; i < 3; i++) {
        __delay_ms(1000);
    }

    Disp_Clear();
}

