#ifndef Tool_H
#define Tool_H

char MainMenuScreen_ToolBox(char *msg);


void GetCurrentDateTime(char* pDate, char* pTime, char* pDelimiter);
void TrimDelimiter(char* pDate, char* pTime, char* pDate1, char* pTime1) ;

int ASCtoBCD(const unsigned char* pAsc, unsigned  char* pBcd) ;
void BCDtoASC( const unsigned char* pBcd, int nBcdLen, unsigned  char* pAsc ) ;

void ASCtoHex(const char* pAsc, char* pHex) ;

#endif

