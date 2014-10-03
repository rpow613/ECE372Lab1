/**************************************************************************************************/

/*
 * File: Lab1Part2
 * File: lab1p2.c
 * Team: Lambda^3
 * Members: Chris Houseman
 *          Randy Martinez
 *          Rachel Powers
 *          Chris Sanford
 *
 * Date: October 2, 2014
 *
 * Description: Code that runs stopwatch timer
 *
 */

/**************************************************************************************************/
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
          BKBUG_ON & COE_ON & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //
// Defines to simply UART's baud rate generator (BRG) regiser
// given the osicllator freqeuncy and PLLMODE.

#define XTFREQ          7372800         	  // On-board Crystal frequency
#define PLLMODE         4               	  // On-chip PLL setting (Fosc)
#define FCY             (XTFREQ*PLLMODE)/2    // Instruction Cycle Frequency (Fosc/2)

#define BAUDRATE         115200
#define BRGVAL          ((FCY/BAUDRATE)/16)-1

// ******************************************************************************************* //

// Variable utilized to store the count that is incremented within the Timer 1 interrupt
// service routine every second.
// Notes:
//        1. This variable is declared are volatile becuase it is updated in the interrupt
//           service routine but will be read in the main execution loop.
//        2. Declared as unsigned char as the varaible will only store the values between
//           0 and 10.
//ADDED CODE
volatile int state = 0; // variable to keep track of timer state
//END ADDED CODE
volatile unsigned char cnt;
unsigned char command;
// ******************************************************************************************* //

int main(void)
{
    //ADDED CODE
    // Configure AD1PCFG register for configuring input pins between analog input
    // and digital IO.
	AD1PCFGbits.PCFG4 = 1;      //digital setting for switch

    // Configure TRIS register bits for Right and Left LED outputs.
    // I01 is RA0 and j PIN 2
    // I02 is RA1 and j PIN 3
	TRISAbits.TRISA0 = 0; //set IO1 to output
        TRISAbits.TRISA1 = 0; //set IO2 to output



    // Configure LAT register bits to initialize Right LED to on.
        LATAbits.LATA0 = 0; //initialize red led to on
	LATAbits.LATA1 = 1; //initialize green LED to off


    // Configure ODC register bits to use open drain configuration for Right
    // and Left LED output.
	ODCAbits.ODA0 = 1; //open drain enable for A0
	ODCAbits.ODA1 = 1; //open drain enable	for A1


    // Configure TRIS register bits for swtich input.
    // I05 is RB2
    // pin number six on j2
	TRISBbits.TRISB2 = 1;

    // Configure CNPU register bits to enable internal pullup resistor for switch input.
	CNPU1bits.CN6PUE = 1;
    //END ADDED CODE


    // The following provides a demo configuration of Timer 1 in which
    // the Timer 1 interrupt service routine will be executed every 1 second
	PR1 = 57599;
	TMR1 = 0;
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;
	T1CONbits.TCKPS = 3;
	T1CONbits.TON = 1;

	// printf by default is mapped to serial communication using UART1.
	// NOTES:
	//        1. You must specify a heap size for printf. This is required
	//           becuase printf needs to allocate its own memory, which is
	//           allocated on the heap. This can be set in MPLAB by:
	//           a.) Selecting Build Options...->Project from the Project menu.
	//           b.) Selecting the MPLABLINK30 Tab.
	//           c.) Entering the size of heap, e.g. 512, under Heap Size
	//        2. printf function is advanced and using printf may require
	//           significant code size (6KB-10KB).
	printf("Lab 2: Debugging Statements\n\r");

	// The following code will not work until you have implemented the
	// the required LCD functions defined within lcd.c
	LCDInitialize();
/*******************************/
////	//below is for testing MoceCursor command
////	LCDMoveCursor(0,2);
////	LCDPrintString("Hello");
////	LCDMoveCursor(1,2);
////	LCDPrintString("Test");
////	command = 0xC;
/*******************************/

	LCDPrintString("Running:");
	LCDMoveCursor(1,0);
	LCDPrintString("00:00.00");
//	LCDPrintChar('0');
//	LCDPrintChar('0');
//	LCDPrintChar(':');
//	LCDPrintChar('0');
//	LCDPrintChar('0');
//	LCDPrintChar('.');
//	LCDPrintChar('0');
//	LCDPrintChar('0');

	while(1)
	{
        //ADDED CODE:
            switch(state){
                //State 0: Initialization State
                case 0:
                    //initaliza LCD, if RB2 is pressed, switch to start state
                    LCDInitialize();
                    break;
                //State 1: Start Stopwatch
                case 1:
                    //turn on timer and display output value
                    //if RB2 is pressed, switch to stop state
                    //if RB(reset) is pressed, switch to 0 state
                    break;
                //State 2: Stop Stopwatch
                case 2:
                    //stop timer and wait for reset or start
                    break;
            }
        //END ADDED CODE:
//			  LCDMoveCursor(1,0);
//            LCDPrintChar(cnt+'0');
//            LCDMoveCursor(1,1);
//            LCDPrintChar(cnt+'0');
//            LCDMoveCursor(1,3);
//            LCDPrintChar(cnt+'0');
//            //given
			  LCDMoveCursor(1,4);
			  LCDPrintChar(cnt+'0');
//            //given above
//            LCDMoveCursor(1,6);
//            LCDPrintChar(cnt+'0');
//            LCDMoveCursor(1,7);
//            LCDPrintChar(cnt+'0');
//
	}
	return 0;
}

// ******************************************************************************************* //
// Defines an interrupt service routine that will execute whenever Timer 1's
// count reaches the specfied period value defined within the PR1 register.
//
//     _ISR and _ISRFAST are macros for specifying interrupts that
//     automatically inserts the proper interrupt into the interrupt vector
//     table
//
//     _T1Interrupt is a macro for specifying the interrupt for Timer 1
//
// The functionality defined in an interrupt should be a minimal as possible
// to ensure additional interrupts can be processed.

void __attribute__((interrupt,auto_psv)) _T1Interrupt(void)
//void _ISR _T1Interrupt(void)
{
	// Clear Timer 1 interrupt flag to allow another Timer 1 interrupt to occur.
	IFS0bits.T1IF = 0;

	// Updates cnt to wraparound from 9 to 0 for this demo.
	cnt = (cnt<9)?(cnt+1):0;

/*******************************/
	//make the LCD blink;
////	command ^= 0x4;
////	WriteLCD(command, 0, 40);
/*******************************/
}

//ADDED CODE:
void __attribute__((interrupt,auto_psv)) _CNInterrupt(void)
{
    // Clear CN interrupt flag to allow another CN interrupt to occur.
    IFS1bits.CNIF = 0;
    if(state == 0 && PORTBbits.RB2 == 1){
        state = 1;
    }
    else if(state == 1 && PORTBbits.RB2 == 1){
        state = 2;
    }
    else if(state == 2 && PORTBbits.RB2 == 1){
        state = 1;
    }
}
//END ADDED CODE
// ******************************************************************************************* //
