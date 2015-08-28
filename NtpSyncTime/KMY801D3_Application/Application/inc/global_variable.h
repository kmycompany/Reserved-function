#ifndef global_variable_H
#define global_variable_H



MyCallFuction	CurProc_Return;
MyCallFuction	CurProc;

unsigned long  SyncSec;
char SyncFlag=0;
char ShortcutKeyEnterFlag=0;				//快捷键进入标志
char InitFlag = 0x01;
char NoElecFlashLed;





const unsigned char softwareversion[9]="20130806";


char RequirePin=0;
unsigned char gEnterPowerDownFlag=0;
unsigned char gFlushDataFlag=0;

char ATsendreceive_Lock=0;
char gCheckOrderFail=0;
char Net_ONOFF=0;//Net_ONOFF=0 on; Net_ONOFF=1 off;
char Net_State=0;
char WiFiState=0; 

#endif

