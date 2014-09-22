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