// ******************************************************************************************* //
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
// This option can be set by selecting "Configuration Bits..." under the Configure
// menu in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
          BKBUG_ON & COE_ON & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
// This option can be set by selecting "Configuration Bits..." under the Configure
// menu in MPLAB.

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

void Setup_UART1(void);

// ******************************************************************************************* //


volatile int flag = 0;      //variable that stops while loop in DebounceDelay()/

// ******************************************************************************************* //

//DebounceDelay() function to debounces button press
// and button release in software.
void DebounceDelay() {
	TMR1=0;
	T1CONbits.TON=1;

	while(flag!=1); //while loop will cycle until flag for timer one becomes 1
        flag=0;
        T1CONbits.TON=0;	//turn off timer
		//reset flag



}

// ******************************************************************************************* //

int main(void)
{
//Uncomment the line "void Setup_UART1(void);" below if you want to Setup the UART
    //to enable use of  printf() for debugging your code, if needed.
	//Make sure the heap size is set to 512 bytes (refer to lab 0 handout)

	//void Setup_UART1(void);

	// ****************************************************************************** //

// Configure AD1PCFG register for configuring input pins between analog input
// and digital IO.

	AD1PCFGbits.PCFG4 = 1;      //digital setting for switch

//  Configure TRIS register bits for Right and Left LED outputs.
	//I01 is RA0 and j PIN 2
    //I02 is RA1 and j PIN 3
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
    //I05 is RB2
    //pin number six on j2

	TRISBbits.TRISB2 = 1;

// Configure CNPU register bits to enable internal pullup resistor for switch input.
	CNPU1bits.CN6PUE = 1;



// Setup Timer 1 to use internal clock (Fosc/2).
	T1CONbits.TCS=0;
	T1CONbits.TGATE = 0;

// Setup Timer 1's prescaler to 1:256.
	T1CONbits.TCKPS1 = 1;   //prescale of 256
    T1CONbits.TCKPS0 = 1;
    
// Set Timer 1 to be initially off.
	T1CONbits.TON=0;

// Clear Timer 1 value and reset interrupt flag
	TMR1=0;

	IFS0bits.T1IF=0;
    IEC0bits.T1IE=1;

// Set Timer 1's period value register to value for 5 ms.
	PR1=287;  //for prescale of 256 need 287

//        if(PORTBbits.RB2 == 0 )
//        {LATA = 0x0000;}

	while(1)
	{
		// TODO: For each distinct button press, alternate which
		// LED is illuminate (on).
		if(PORTBbits.RB2 == 0)
		{
                    LATA ^= 0x0003;  //toggles leds a0 and a1;
                    DebounceDelay();
			while(PORTBbits.RB2 == 0) //wait for switch release
                      {
			DebounceDelay();

                       }
                       
		}


	}
	return 0;
}

void __attribute__((interrupt,auto_psv)) _T1Interrupt(void){
   //set flag to 1 if interrupt occurs
    flag = 1;
    IFS0bits.T1IF = 0;


}