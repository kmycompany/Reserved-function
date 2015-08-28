#ifndef Setting_H
#define Setting_H

#include "stm32f10x.h"
//#include "my_stdint.h"


#define STORESIZE			42		//if XStor  added entries, the STORESIZE  must added too

#define GPRS    0x47
#define WiFi    0x57


struct storeFile{
	unsigned char *Viarable;		//Viarable
	const unsigned char *initVal;	//init value
};


extern struct storeFile XStor[STORESIZE+1];
extern const unsigned char efile_Config[];
extern unsigned char dataledflag;
extern const unsigned char Configfile_des3key[25];



extern unsigned char LightPercent[4];				//light percent
extern unsigned char EnterAdvancedSetPassWord[22];	//高级设置密码
extern unsigned char Domain_IP[51];			//ip 地址219.134.25.231,81
extern unsigned char StorePIN[5];					//PIN name
extern unsigned char ApnApn[50+1];
extern unsigned char ApnUsername[21+1];
extern unsigned char ApnPassword[21+1];
extern unsigned char beepKG[2];
extern unsigned char ScreenOffTimeout[3];			//PIN name
extern unsigned char TerminalID[17];				//终端id
extern unsigned char TerminalPassword[17];			//终端密码
extern unsigned char VersionURL[100+1];
extern unsigned char UpgradeURL[100+1];
extern unsigned char NetworkSwitch[2];
extern unsigned char Upgrade_Flag[2];



/////////////////////the parameter is below is for canada orders
extern unsigned char RequestWebURL[150+1];
extern unsigned char CallbackURL[150+1];
extern unsigned char RejectReason[200+1];
extern unsigned char ResID[10+1];
extern unsigned char LoginWebUsername[20+1];
extern unsigned char LoginWebPassword[20+1];
extern unsigned char BeepDuration[2+1];
extern unsigned char AcceptTimeMode[1+1];
extern unsigned char CheckOrderInterval[3+1];
extern unsigned char NetOpenTime[10];
extern unsigned char NetCloseTime[10];
extern unsigned char PrintDensity[3+1];
extern unsigned char PrintDateFormat[1+1];
extern unsigned char PrintTimeFormat[1+1];
extern unsigned char PrintPageHeader[30+1];
extern unsigned char PrintPageFooter[100+1];
extern unsigned char PrintReceiptCount[1+1];
extern unsigned char AutoAcceptOrder[1+1];
extern unsigned char AcceptTimeOptions[150+1];
extern unsigned char TimeZone[50+1];

//WiFi
extern unsigned char LocalIpPortBuf[28];			//本机IP及端口
extern unsigned char GatewayBuff[28];				//网关
extern unsigned char DNSBuff[28];					//DNS服务器
extern unsigned char MaskBuff[28];					//子网掩码

extern unsigned char WLANWP[30];			//wifi wp
extern unsigned char dhcpclient[2];
extern unsigned char SSID[30];			//wifi ssid

#define KMY_printer			'0'
#define JieNa_printer		'1'
#define DEFAULT_PIN         ""


char BasicSettings(char *msg);
char MainMenuScreen_Settings(char *msg);
unsigned char RestoreConfigFile(void);
unsigned char RestoreGlobalVariable(void);
unsigned char StorageVariable (void);

char PinSet(char *msg);
char PIN_Store (char *msg);

char WANSet(char *msg);
static char LoclaIPSet(char *msg);
static char GatewaySet(char *msg);
static char MaskSet(char *msg);
static char DNSSet(char *msg);

char WIFISet(char *msg);
static char CSModeSet(char *msg);
static char DhcpSet(char *msg);

static char WIFIParmeterSet(char *msg);


#endif

