
#ifndef SelfTest_H
#define SelfTest_H


extern const unsigned char RFLogo[] ;

//config card
extern unsigned char CONFIG_CARD[1+2+1] ;	
//Request card with idle mode
extern unsigned char REQUEST_IDLE[1+2+1] ;		
//Request card with all mode
extern unsigned char REQUEST_ALL[1+2+1] ;		
//SNR; Get SNR
extern unsigned char ANTICOLL[1+6+1] ;	
//halt card
extern unsigned char HALT_CARD[1+1+1] ;	


char abeyant(char *msg);
char pcm_player(char *msg);
char SelfTest(char *msg);
//char TemporaryNotProvideThisFunction(char *msg);
void Print2Buf(const unsigned char srcbuff[], unsigned char len);

#endif

