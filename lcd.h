/**************************************************************************************************/

/*
 * File: lcd.h
 * Team: Lambda^3
 * Members: Chris Houseman
 *          Randy Martinez
 *          Rachel Powers
 *          Chris Sanford
 *
 * Date: October 2, 2014
 *
 * Description: Header file for lcd functions
 *
 */

// ******************************************************************************************* //
// ******************************************************************************************* //

void LCDInitialize(void);
void LCDClear(void);
void LCDMoveCursor(unsigned char x, unsigned char y);
void LCDPrintChar(char c);
void LCDPrintString(const char* s);

// ******************************************************************************************* //

//for testing other LCD instructions
void WriteLCD(unsigned char word, unsigned commandType, unsigned usDelay);
void DelayUs(unsigned int usDelay);