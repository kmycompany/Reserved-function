#include "FoodOrder.h"
#include "stm32f10x.h"

#include "MenuOperate.h"
#include "global_extern_variable.h"
#include "lcd.h"
#include "gprs.h"
#include <string.h>
#include "setting.h"
#include "ShuRuFa.h"
#include "mypure_library.h"
#include "my_library.h"
#include <stdio.h>
#include "key.h"
#include <stdlib.h>
#include "EepromFileSystem.h"
#include "SelfTest.h"
#include "Version.h"

#include "kmy_Time.h"
#include "kmy_KeyDrv.h"
#include "kmy_EEPROMDrv.h"
#include "kmy_USART2Drv.h"
#include "kmy_GprsDrv.h"
#include "kmy_LEDDrv.h"
#include "kmy_USART1Drv.h"
#include "kmy_PrinterDrv.h"
#include "kmy_BeepDrv.h"
#include "kmy_FlashDrv.h"
#include "WiFi.h"
#include "kmy_wifi.h"
#include "kmy_WiFi_SPI.h"
#include "SyncTime.h"
#include "ntp.h"

#define AUTOCHECK_TIMEOUT 6000

#define DISPLAY_MAXORDERID 25

// eeprom M24512 0x00~ 0x10 000
// |0x00000~0x08000 eeprom file system |0x08000~0x0A000 confirm order|0x0A000~0x0C000 reject order | 0x0C000~0x10000 reserve|
//
#define TOTAL_CONFIRMADDR 0x08000
#define CONFIRMORDER_BASE TOTAL_CONFIRMADDR+1

#define TOTAL_REJECTADDR  0x0A000
#define	REJECTORDER_BASE TOTAL_REJECTADDR+1

#define ORDRER_SIZE 1024

//Order limit count #...# #...# #..# order number
#define ORDER_LIMIT 10

char gOrderBuf[3072]={0};
char pBufNonBlock[256]={0};

static char writeto_gOrderBuf=0;
static char ReceiveOrderFlag=0;
static char Writting_gOrderBuf=0;
static char connectfail_conunt=0;
static int check_Interval=0;
static char LcdBlackLightFlash=0;
static char AutoPrint=0;

static char SyncTimeFlag=1;
const unsigned char logo_mab[]=
{
	/*------------------------------------------------------------------------------
	;  源文件 / 文字 : E:\My_Project\胡小俊英国客户—mab\logo_mab.BMP字模
	;  宽×高（像素）: 307×51
	------------------------------------------------------------------------------*/
	0x00,0x00,0x0C,0x3C,0x7C,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xF8,0xE0,0xC0,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,
	0xF8,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0x7C,0x1C,0x0C,0x00,0x00,0x00,0x00,0x00,
	0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,
	0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,
	0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0xFC,
	0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0x7C,
	0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,
	0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0xFC,0xFC,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0F,0x1F,0x7F,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFC,0xF8,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFC,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,
	0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xE0,
	0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,
	0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x01,0x03,0x0F,0x1F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xF8,0xFC,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x3F,0x1F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,
	0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,
	0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
	0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xC0,0xF0,0xFC,0xFE,0xFE,0xFF,0x3F,0x1F,0x0F,0x07,0x07,0x03,
	0x03,0x03,0x03,0x03,0x03,0x07,0x07,0x0F,0x1F,0x3F,0x7F,0xFF,0xFE,0xFC,0xF8,0xE0,
	0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,0xF8,0xFC,0xFE,0xFF,0x7F,0x1F,0x0F,
	0x0F,0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x07,0x07,0x0F,0x0F,0x1F,0x7F,0xFF,0xFE,
	0xFC,0xF8,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF8,0xFC,0xFE,0xFF,
	0x7F,0x3F,0x0F,0x0F,0x07,0x03,0x03,0x03,0x03,0x03,0x07,0x07,0x0F,0x0F,0x1F,0x7E,
	0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
	0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,
	0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFC,0xE0,
	0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xC0,0xF8,0xFF,0xFF,0xFF,0xFF,0x3F,0x0F,0x00,0x00,0x00,0x00,0x0F,0x7F,0xFF,0xFF,
	0xFF,0xFF,0xF0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x80,0xF0,0xFF,0xFF,0xFF,0xFF,0x7F,0x0F,0x00,0x00,0x00,0x00,0x00,
	0x3F,0xFF,0xFF,0xFF,0xFF,0xFE,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x80,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,
	0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
	0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
	0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
	0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
	0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
	0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
	0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,
	0x0F,0x1F,0x3F,0x3F,0x7E,0x7E,0x7C,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x7C,
	0x7C,0x7E,0x3F,0x3F,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0x07,0x0F,0x1F,0x1F,0x3F,0x7F,0x7E,0x7C,0x7C,0xF8,0xF8,0xF8,0xF8,0xF8,
	0xF8,0xF8,0xFC,0x7C,0x7E,0x3F,0x3F,0x1F,0x1F,0x0F,0x07,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x0F,0x1F,0x3F,0x3F,0x7E,0x7C,0xFC,0xF8,0xF8,
	0xF8,0xF8,0xF8,0xF8,0x78,0x7C,0x3E,0x3F,0x1F,0x0F,0x7F,0x7F,0x7F,0x7F,0x7F,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,
};


// replace the string c1 to string c2 from the Buff to DesBuff, no more than 10 c1
void my_string_replace2 (char *Buff, char* DesBuff, char* c1,  char* c2)
{
	char *p=NULL,*p1=NULL;
	int len=0;
	char i=0;

	len=strlen(c1);
	p=Buff;
	p1=p;

	for(i=0;i<10;i++)
	{
		p=strstr(p,c1);
		
		if(p==NULL)break;
		
		strncat(DesBuff,p1,p-p1);
		strcat(DesBuff,c2);

		p=p+len;
		p1=p;
	}
	strcat(DesBuff,p1);
}


void kmy_EnableDisableTime3(char OnOFF)
{
	if(OnOFF==0)
	{
		TIM_Cmd(TIM3, DISABLE);
	}
	else
	{
		TIM_Cmd(TIM3, ENABLE);
	}
}

char SeperateIpAndPort(char *IPandPort, char *IP, char *Port)
{
	char *p;

	if((IPandPort==NULL)||(IP==NULL)||(Port==NULL))return 1;
	p=IPandPort;
	
	p=strchr(p,',');
	if(p==NULL)
	{
		strcpy(IP,IPandPort);
		strcpy(Port,"80");
	}
	else
	{
		strncat(IP,IPandPort,p-IPandPort);
		p++;
		strcpy(Port,p);
	}

	return 0;//IP correct
}

void kmy_NetStateSet(void)
{
	unsigned char o_hour=0;
	unsigned char o_min=0;
	unsigned char c_hour=0;
	unsigned char c_min=0;
	unsigned char hour=0;
	unsigned char min=0;
	char *p=NULL;
	
	
	p=(char*)NetOpenTime;
	p=strstr(p,":");
	if(p==NULL)
	{
		Net_State=0;
		printf("Close Network all the time\r\n");
		return;
	}
	o_hour=atoi((char*)NetOpenTime);
	p++;
	o_min=atoi(p);

	p=(char*)NetCloseTime;
	p=strstr(p,":");
	if(p==NULL)
	{
		Net_State=1;
		printf("Open Network all the time\r\n");
		return;
	}
	
	c_hour=atoi((char*)NetCloseTime);
	p++;
	c_min=atoi(p);
	kmy_TimeGetTime(NULL,NULL,NULL,&hour,&min,NULL,NULL);

	if((o_hour>24)||(c_hour>24)||(o_min>60)||(c_min>60))return;
	if((o_hour==c_hour)&&(o_min==c_min))
	{
		Net_State=1;
		printf("Open Network all the time\r\n");
		return;
	}
	
	if((c_hour>o_hour)||((c_hour==o_hour)&&c_min>=o_min))
	{
		if(((hour>o_hour)&&(hour<c_hour))||((hour==o_hour)&&(min>=o_min))||((hour==c_hour)&&(min<c_min)))
		{
			Net_State=1;
			printf("Open Network time hour=%d,min=%d\r\n",o_hour,o_min);
		}
		else
		{
			Net_State=0;
			printf("Close Network time hour=%d,min=%d\r\n",c_hour,c_min);
		}
	}
	else
	{
		if((hour>o_hour||hour<c_hour)||((hour==o_hour)&&(min>=o_min))||((hour==c_hour)&&(min<c_min)))
		{
			Net_State=1;
			printf("Open Network time hour=%d,min=%d\r\n",o_hour,o_min);
		}	
		else
		{
			Net_State=0;
			printf("Close Network time hour=%d,min=%d\r\n",c_hour,c_min);			
		}
	}
} 

void GetHttpGETString(char* destbuff,const char* OrderRequestWebUrl, 
	                  const char *RestaurantID, const char *UserName, const char *Password, 
					  const char *IP)
{
	//GET http://www.yellfood.com/orderrequest.php?a=25&u=demo&p=123456789 HTTP/1.1
	strcpy(destbuff, "GET ");
	strcat(destbuff, OrderRequestWebUrl);
	
    if(strstr(OrderRequestWebUrl,"?")==NULL)
	{
		strcat(destbuff, "?");
	}
	strcat(destbuff, "a=");
	strcat(destbuff, RestaurantID);
/*	strcat(destbuff, "&u=");
	strcat(destbuff, UserName);
	strcat(destbuff, "&p=");
	strcat(destbuff, Password);
*/	
	strcat(destbuff, " HTTP/1.1");
	strcat(destbuff, "\r\n");
	
	//Host: www.yellfood.com
	strcat(destbuff, "Host: ");
	strcat(destbuff, IP);
	strcat(destbuff, "\r\n");
	
	//Pragma: no-cache
	strcat(destbuff, "Pragma: no-cache");
	strcat(destbuff, "\r\n");
	
	//Accept: */*
	strcat(destbuff, "Accept: */*");
	strcat(destbuff, "\r\n");
	
	//Proxy-Connection: Keep-Alive
	strcat(destbuff, "Proxy-Connection: Keep-Alive");
	strcat(destbuff, "\r\n");	
	strcat(destbuff, "\r\n");
	
}

void GetHttpCallbackString(char* destbuff,
					   const char* OrderCallbackWebUrl, const char *WebServerIp, const char *WebServerPort,
					   const char *RestaurantID, const char *UserName, const char *Password, const char* OrderNO, const char* Answer,
					   const char* RejectReason, const char* DeliverTime)
{
	
	//GET http://www.yellfood.com/printer.php?a=25&o=322&ak=Accepted&m=ok&dt=9:50&u=demo&p=123456789 HTTP/1.1	
	strcpy(destbuff, "GET ");
	strcat(destbuff, OrderCallbackWebUrl);
	if(strstr(OrderCallbackWebUrl,"?")==NULL)
	{
		strcat(destbuff, "?");
	}	
	strcat(destbuff, "a=");
	strcat(destbuff, RestaurantID);
	strcat(destbuff, "&o=");
	strcat(destbuff, OrderNO);
	strcat(destbuff, "&ak=");
	strcat(destbuff, Answer);
	strcat(destbuff, "&m=");
	strcat(destbuff, RejectReason);
	strcat(destbuff, "&dt=");
	strcat(destbuff, DeliverTime);
/*	strcat(destbuff, "&u=");
	strcat(destbuff, UserName);
	strcat(destbuff, "&p=");
	strcat(destbuff, Password);
*/	
	strcat(destbuff, " HTTP/1.1");
	strcat(destbuff, "\r\n");
	
	//Host: www.yellfood.com
	strcat(destbuff, "Host: ");
	strcat(destbuff, WebServerIp);

	strcat(destbuff, "\r\n");
	
	//Pragma: no-cache
	strcat(destbuff, "Pragma: no-cache");
	strcat(destbuff, "\r\n");
	
	//Accept: */*
	strcat(destbuff, "Accept: */*");
	strcat(destbuff, "\r\n");
	
	//Proxy-Connection: Keep-Alive
	strcat(destbuff, "Proxy-Connection: Keep-Alive");
	strcat(destbuff, "\r\n");
	strcat(destbuff, "\r\n");
}

//POST http://192.168.5.1:81/ibacstel/printer.php HTTP/1.1
//Host: 192.168.5.1:81
//Pragma: no-cache
//Accept: */*
//Proxy-Connection: Keep-Alive
//Content-Length: 54
//Content-Type: application/x-www-form-urlencoded

//a=2&o=111654&ak=Accepted&m=ok&dt=19:30&u=demo&p=123456

void GetHttpPOSTString(char* destbuff,
					   const char* OrderPostWebUrl, const char *WebServerIp, const char *WebServerPort,
					   const char *RestaurantID, const char *UserName, const char *Password, 
					   const char* OrderNO, const char* Answer, const char* RejectReason, const char* DeliverTime)
{
	char PostFiledDatas[100] = {0};
	char ContentLengthBuf[50] = {0};
	int  ContentLengthBufIndex = 0;
	
	//make post field
	strcpy(PostFiledDatas, "a=");
	strcat(PostFiledDatas, RestaurantID);
	strcat(PostFiledDatas, "&o=");
	strcat(PostFiledDatas, OrderNO);
	strcat(PostFiledDatas, "&ak=");
	strcat(PostFiledDatas, Answer);
	strcat(PostFiledDatas, "&m=");
	strcat(PostFiledDatas, RejectReason);
	strcat(PostFiledDatas, "&dt=");
	strcat(PostFiledDatas, DeliverTime);
	strcat(PostFiledDatas, "&u=");
	strcat(PostFiledDatas, UserName);
	strcat(PostFiledDatas, "&p=");
	strcat(PostFiledDatas, Password);

	//make Content-Length
	strcpy(ContentLengthBuf, "Content-Length: ");
	ContentLengthBufIndex = strlen(ContentLengthBuf);
	sprintf(ContentLengthBuf+ContentLengthBufIndex, "%d", strlen(PostFiledDatas));

	//POST http://192.168.5.1:81/ibacstel/printer.php HTTP/1.1
	strcpy(destbuff, "POST ");
	strcat(destbuff, OrderPostWebUrl);
	strcat(destbuff, " HTTP/1.1");
	strcat(destbuff, "\r\n");
	
	//Host: 192.168.5.1:81
	strcat(destbuff, "Host: ");
	strcat(destbuff, WebServerIp);
	strcat(destbuff, ":");
	strcat(destbuff, WebServerPort);
	strcat(destbuff, "\r\n");
	
	//Pragma: no-cache
	strcat(destbuff, "Pragma: no-cache");
	strcat(destbuff, "\r\n");
	
	//Accept: */*
	strcat(destbuff, "Accept: */*");
	strcat(destbuff, "\r\n");
	
	//Proxy-Connection: Keep-Alive
	strcat(destbuff, "Proxy-Connection: Keep-Alive");
	strcat(destbuff, "\r\n");
	
	//Content-Length: 37
	strcat(destbuff, ContentLengthBuf);
	strcat(destbuff, "\r\n");
	
	//Content-Type: application/x-www-form-urlencoded
	strcat(destbuff, "Content-Type: application/x-www-form-urlencoded");
	strcat(destbuff, "\r\n");

	//a=2&o=111654&ak=Accepted&m=ok&dt=19:30&u=demo&p=123456
	strcat(destbuff, "\r\n");
	strcat(destbuff, PostFiledDatas);
	strcat(destbuff, "\r\n");
	strcat(destbuff, "\r\n");
	
}

int GetCountOfOrders(const char *HttpContents)
{
	char *p1 = NULL;
	char *p2 = NULL;
	int Count = 0;

	//find the 1st "#"
	p1 = strstr(HttpContents, "#");
	if(p1 == NULL)
		return Count;
	
	while(1)
	{
		/* ignore the 1st "#" */
		p1 += 1; 
		
		/* find the 2nd "#" */
		p2 = strstr(p1, "#");
		if(p2 == NULL)
			break;
		/* ignore the 2nd "#" */
		p2 += 1;
		
		if(p1 != NULL && p2 != NULL)
		{
			if(p2 - p1 > 0)
				Count++;
		}
		else
		{
			break;
		}
		
		/* find the 1st "#" */
		p1 = strstr(p2, "#");
		if(p1 == NULL)
			break;
	}
	return Count;
}

void Check_RequestTime(ORDER_STRUCT *order, char index)
{
	unsigned char R_hour=0;
	unsigned char R_min=0;
	unsigned char hour=0;
	unsigned char min=0;
	char *p=NULL;
		
	p=order->RequestTime;
	p=strstr(p,":");
	if(p==NULL)
	{
		return;
	}
	R_hour=atoi(order->RequestTime);
	p++;
	R_min=atoi(p);
	
	kmy_TimeGetTime(NULL,NULL,NULL,&hour,&min,NULL,NULL);

	if(R_hour<hour||((R_hour==hour)&&R_min<=min))
	{
	    memset(order->RequestTime,0,sizeof(order->RequestTime));
		strcpy(order->RequestTime,"ASAP");
	}
}

void AddSpaces(char *FoodName,char *Food_Amount,unsigned char NumPerLine)
{
	char *p1 = NULL;
	char *p2 = NULL;

    unsigned char i,len;
	char FoodAmount[30]={0};

	if(strlen(Food_Amount)<2)
	{
		//strcat(FoodName,"\n");
		return;
	}

	//strcpy(FoodAmount,"\x9c");
	strcat(FoodAmount,Food_Amount);

    p2 = FoodName;

    if(strstr(FoodName,"\n")==NULL)
    {    
		len = NumPerLine - (strlen(FoodName) + strlen(FoodAmount));
    }
	else
	{
		p1 = strstr(FoodName,"\n");
		while(strstr(p1,"\n"))
		{
		    p1 = strstr(p1,"\n");
			p1++;			
		}
		len = NumPerLine - ((strlen(FoodName) - (p1 - p2)) + strlen(FoodAmount));
	}
    if(len>NumPerLine)
	{
		strcat(FoodName,"\n");
		len = NumPerLine - strlen(FoodAmount);
		if(len > NumPerLine)
		{
    		strcat(FoodName,FoodAmount);
    		return;			
		}
		for(i=0;i<len;i++)
    	{
    		strcat(FoodName," ");
    	}		
		strcat(FoodName,FoodAmount);
		return;
	}
	for(i=0;i<len;i++)
	{
		strcat(FoodName," ");
	}
	strcat(FoodName,FoodAmount);
}

//  brief  To ensure the integrity of the words in case line feed.
//  param  buff:       Data buffer 
//  param  NumPerLine: A line of number of characters
//  retval None
void AddCarriage(char *buff, unsigned char NumPerLine)
{
	char *p1 = NULL;
	char *p2 = NULL;
	unsigned char len=0;
	
	p1 = buff;
	p2 = p1;
	
	while(1)
	{	
	    if(*p1 == '\n')
		{
		    len = 0;
			p2 = p1;
		}
		else if(*p1 == ' ')
		{
		    p2 = p1;
		    len++;
		}
		else 
		{
    		len++;
		}
       
		if(len==NumPerLine)// zoom1 32 ,zoom2 16
		{
			*p2='\n';
			len=p1-p2;			
		}
		p1++;
		if(*p1 == '\0')return;
		if(p1 == NULL)break;
	}
}

char* GetOneOrder(char *OrderString, ORDER_STRUCT *order, char *gOrderFood, char *CustomerComments)
{
	char *p1 = NULL;
	char *p2 = NULL;
	char State = 0;
	int i = 0;
	int pLength = 0;
	int pIndex = 0;//PublicIndex
	char buff[400]={0};

	typedef struct 
	{ 
	    char FoodCategory[30];
		char FoodQuantity[5];//ItemQuantity
		char FoodName[300];//ItEMTitle
		char FoodAmount[10];//ItemPrice
	}FOOD;
	FOOD food={0};

/*step1 --- check if there is a string start with "#" && end with "#" */
	p1 = strstr(OrderString, "#");
    if(p1 == NULL)return NULL;
	/* ignore the 1st "#" */
	p1 += 1; 

	/* check if there is a "#" in the end */
	p2 = strstr(p1, "#");
	if(p2 == NULL)return NULL;
	/* ignore the 2nd "#" */
	p2 += 1; 

	/* get the pLength of datas between the two "#" */
	pLength = p2 - p1;

/*step2 --- Analysis datas */
	for(i=0; i<pLength; )
	{
		switch(State)
		{
		case 0:/* preparetion */
			pIndex = 0;
			State++;
			break;

		case 1:/* get RestaurantID, end with "*" */
			if(*p1 != '*')
			{
				if(pIndex+1 < sizeof(order->RestaurantID))
				{
					order->RestaurantID[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;

		case 2:/* get OrderNumber, end with "*" */
			if(*p1 != '*')
			{
				if(pIndex+1 < sizeof(order->OrderType))
				{
					order->OrderType[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;
			
		case 3:/* get RestaurantID, end with "*" */
			if(*p1 != '*')
			{
				if(pIndex+1 < sizeof(order->OrderNumber))
				{
					order->OrderNumber[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
			    State++;
			}
			break;
			
/* ----------------------------------------------------------------------------------------- */
        case 4:/* get OrderFood[pIndex].FoodCategory, end with "|" */

			if(*p1 != '|')
			{
				if(pIndex+1 < sizeof(food.FoodCategory))
				{
				    food.FoodCategory[pIndex] = *p1;
				    pIndex++;
				}
				p1++;i++;
			}
			else
			{
			    //strcat(gOrderFood,"\n");
			    if(strlen(food.FoodCategory))
			    {
					strcat(gOrderFood,food.FoodCategory);	
					strcat(gOrderFood,"\n");
			    }
				memset(food.FoodCategory,0,sizeof(food.FoodCategory));
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;
			
		case 5:/* get OrderFood[pIndex].FoodQuantiy, start with "0~9" && end with "|" */
			
			if(*p1 != '|')
			{
				if(pIndex+1 < sizeof(food.FoodQuantity))
				{
					food.FoodQuantity[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;
			
		case 6:/* get OrderFood[pIndex].FoodName, end with "|" */
			if(*p1 != '|')
			{
				if(pIndex+1 < sizeof(food.FoodName))
				{
					food.FoodName[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				 p1++;i++;
				 pIndex = 0;
				 State++;
			}
			break;
						
		case 7:/* get OrderFood[pIndex].FoodAmount, end with ";" */
			if(*p1 == '*')
			{
				if(strlen(food.FoodQuantity))
			    {
			    	strcpy(buff,food.FoodQuantity);
					strcat(buff," X ");
			    }
				else if(strlen(food.FoodName))
				{
					strcat(buff,"  + ");
				}
				strcat(buff,food.FoodName);				
				AddCarriage(buff,31);
				AddSpaces(buff,food.FoodAmount,31);
				strcat(gOrderFood,buff);
				strcat(gOrderFood,"\n");
				memset(buff,0,sizeof(buff));
				memset(food.FoodQuantity,0,sizeof(food.FoodQuantity));
				memset(food.FoodName,0,sizeof(food.FoodName));
				memset(food.FoodAmount,0,sizeof(food.FoodAmount));	
				p1++;i++;
				pIndex = 0;
				/* will jump to case 8:, End get OrderFood[] */
				State++;
			}
			else
			{
				if(*p1 != ';')
				{
					if(pIndex+1 < sizeof(food.FoodAmount))
					{
						food.FoodAmount[pIndex] = *p1;
						pIndex++;
					}
					p1++;i++;
				}
				else if(strlen(food.FoodName))
				{
				    if(strlen(food.FoodQuantity))
				    {
				    	strcpy(buff,food.FoodQuantity);
						strcat(buff," X ");
				    }
					else
    				{
    					strcat(buff,"  + ");
    				}
					strcat(buff,food.FoodName);
					AddCarriage(buff,31);
					AddSpaces(buff,food.FoodAmount,31);	
					strcat(gOrderFood,buff);
					strcat(gOrderFood,"\n");
					memset(buff,0,sizeof(buff));
					memset(food.FoodQuantity,0,sizeof(food.FoodQuantity));
					memset(food.FoodName,0,sizeof(food.FoodName));
					memset(food.FoodAmount,0,sizeof(food.FoodAmount));					
					p1++;i++;
					pIndex = 0;
					/* will jump to case 4:, Continue to get OrderFood[] */
					State = 4;
				}
			}
			break;
/* ----------------------------------------------------------------------------------------- */
/*		case 8:// get DeliveryCharge, end with ";" 
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->DeliveryCharge))
				{
				    order->DeliveryCharge[pIndex] = *p1;
				    pIndex++;				
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;

		case 9:// get CC_HandelingFees, end with ";"
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->CC_HandelingFees))
				{
				    order->CC_HandelingFees[pIndex] = *p1;
				    pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;
*/			
		case 8:// get Discount, end with ";" 
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->Discount))
				{
				    order->Discount[pIndex] = *p1;
				    pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;			

		case 9:// get Total, end with ";" 
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->Total))
				{
					order->Total[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;

		case 10:// get CustomerType, end with ";" 
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->CustomerType))
				{
					order->CustomerType[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;

		case 11:// get CustomerName, end with ";"
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->CustomerName))
				{
					order->CustomerName[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;

		case 12:// get CustomerAddress, end with ";"
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->CustomerAddress))
				{
					order->CustomerAddress[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;

		case 13:/* get RequestTime, end with ";"*/ 
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->RequestTime))
				{
					order->RequestTime[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;

		case 14:/* get PreviousNumberOfOrders, end with ";" */
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->PreviousNumberOfOrders))
				{
					order->PreviousNumberOfOrders[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				if(strstr(order->OrderType,"3"))/*if OrderType=3, Into reserve mode*/
				{
					State = 18;
				}
				else State++;
			}
			break;

		case 15:// get PaymentStatus, end with ";" 
			if(*p1 != ';')
			{
				if(pIndex+1 < sizeof(order->PaymentStatus))
				{
					order->PaymentStatus[pIndex] = *p1;
					pIndex++;
				}
				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;

		case 16:// get CustomerPhone, end with "*"
			if(*p1 != '*')
			{

				if(pIndex+1 < sizeof(order->CustomerPhone))
				{
				    order->CustomerPhone[pIndex] = *p1;	
				    pIndex++;			
				}

				p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;
     
		case 17:/* get CustomerComments, end with "#"*/ 
			if(*p1 == '*')
			{
				p1++;i++;
			}
			if(*p1 != '#')
			{
				if(pIndex+1 < 200)
				{				
				    CustomerComments[pIndex] = *p1;
					pIndex++;
				}
					p1++;i++;
			}
			else
			{
				p1++;i++;
				pIndex = 0;
				State++;
			}
			break;

		default:
			break;
			
		}

	}

	printf("%s\r\n", order->RestaurantID);
	printf("%s\r\n", order->OrderType);
	printf("%s\r\n", order->OrderNumber);
	printf("%s\r\n", gOrderFood);
	//printf("%s\r\n", order->DeliveryCharge);
	//printf("%s\r\n", order->CC_HandelingFees);
	printf("%s\r\n", order->Discount);
	printf("%s\r\n", order->Total);
	printf("%s\r\n", order->CustomerType);
	printf("%s\r\n", order->CustomerName);
	printf("%s\r\n", order->CustomerAddress);
	printf("%s\r\n", order->RequestTime);
	printf("%s\r\n", order->PreviousNumberOfOrders);
	printf("%s\r\n", order->PaymentStatus);
	printf("%s\r\n", order->CustomerPhone);
	printf("%s\r\n", CustomerComments);

	return p2;	
}


char AcceptRejectOrders(void)
{


	
	unsigned char retval;
	clear_lcd();
	DrawTitle_to_row_Language2 (2, "Accept this order?", "Accept this order?");
	text_out_to_row_x_Language2 (4, 6,"Cancel", "Cancel");
	text_out_to_row_x_Language2 (4, 100,"OK", "OK");
	
	while(1)
	{
		retval = kmy_GetKeyValue();
		
		if (retval == KEY_NONE) continue;
		if (beepKG[0] == '1')
		{
			BeepNormal();
		}

		switch (retval)
		{
			case KEY_Enter:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				
				return retval;
			default: break;
		}

	}
}

void GetTcpConnectATString (unsigned char*dest,unsigned char*IP,unsigned char*Port)
{		
	printf("IP=%s;Port=%s\r\n",IP,Port);
	
	strcpy ( (char *) dest , AT_TcpConnect );
	strcat ( (char *) dest , (char *)IP );
	strcat ( (char *) dest , "\",\"" );
	strcat ( (char *) dest , (char *)Port );
	strcat ( (char *) dest , "\"\r" );
}


char GetSelectItemOfMenus (char (*Menus)[MENU_COL], char MaxItems,char *SelectItemIndex) //0 <= MenuVar.CurMenuIndex[0] <MaxItems
{
    #define _4RowOneScr 5
    #define KEY_FLASHINTERFACE 0x55
	char tmp[4]={0};
	char tmpbuff[20]={0};
	char TempKey = KEY_NONE;

	MENUVAR MenuVar = {0, 0, 0, 0, 0, 0x01};
	
	MenuVar.FlashFlag = 0x01;

	printf("-------------------\r\n");
	
	if (MenuVar.CurMenuIndex >= _4RowOneScr)
	{
		MenuVar.MenuScrollCount = (MenuVar.CurMenuIndex + 1) - _4RowOneScr;
		MenuVar.CurHighLightRow = _4RowOneScr - 1;
		MenuVar.OldHighLightRow = _4RowOneScr - 1;
	}
	else
	{
		MenuVar.CurHighLightRow = MenuVar.CurMenuIndex;
		MenuVar.OldHighLightRow = MenuVar.CurMenuIndex;
	}

	PostKey (KEY_FLASHINTERFACE);

	while (1)
	{
		TempKey = kmy_GetKeyValue();

		if (TempKey == KEY_NONE) continue;

		switch (TempKey)
		{
		    case KEY1:
			    MenuVar.MenuLayerCnt=1;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY2:
				MenuVar.MenuLayerCnt=2;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY3:
				MenuVar.MenuLayerCnt=3;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY4:
				MenuVar.MenuLayerCnt=4;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY5:
				MenuVar.MenuLayerCnt=5;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY6:
				MenuVar.MenuLayerCnt=6;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY7:
				MenuVar.MenuLayerCnt=7;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY8:
				MenuVar.MenuLayerCnt=8;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY9:
				MenuVar.MenuLayerCnt=9;
				PostKey (KEY_FLASHINTERFACE);
			    break;
					
			case KEY_UP:
				if (beepKG[0] == '1')
				{
					BeepNormal();
				}
				
				if ( (MenuVar.CurHighLightRow == 0) && (MenuVar.MenuScrollCount - 1 >= 0) ) //
				{
					MenuVar.MenuScrollCount --;
					MenuVar.FlashFlag |= 0x01;//0x01---需要刷新整个界面
				}

				if (MenuVar.CurMenuIndex - 1 >= 0)
				{
					if (MenuVar.CurHighLightRow - 1 >= 0) MenuVar.CurHighLightRow --;

					MenuVar.CurMenuIndex --;
					MenuVar.FlashFlag |= 0x02;//0x02---需要刷新高亮行
				}
				else
				{
				    MenuVar.CurMenuIndex=MaxItems-1;
					if (MenuVar.CurMenuIndex >= _4RowOneScr)
					{
						MenuVar.MenuScrollCount =(MenuVar.CurMenuIndex + 1) - _4RowOneScr;					    
		                MenuVar.OldHighLightRow = MenuVar.CurHighLightRow=_4RowOneScr - 1;
						MenuVar.FlashFlag |= 0x01;//0x01---需要刷新整个界面
					}
					else
					{
					    MenuVar.MenuScrollCount=0;
						MenuVar.CurHighLightRow = MenuVar.CurMenuIndex;
		                MenuVar.OldHighLightRow = MenuVar.CurMenuIndex;	
						MenuVar.FlashFlag |= 0x01;//0x01---需要刷新整个界面
					}					
				}
				PostKey (KEY_FLASHINTERFACE);
				break;

			case KEY_DOWN:
				if (beepKG[0] == '1')
				{
					BeepNormal();
				}
				
				if ( (MenuVar.CurHighLightRow >= (_4RowOneScr - 1) ) && (MenuVar.MenuScrollCount + 1 <= (MaxItems - _4RowOneScr) ) )
				{
					MenuVar.MenuScrollCount ++;
					MenuVar.FlashFlag |= 0x01;//0x01---需要刷新整个界面
				}

				if (MenuVar.CurMenuIndex + 1 < MaxItems)
				{
					if (MenuVar.CurHighLightRow + 1 < _4RowOneScr) MenuVar.CurHighLightRow ++;

					MenuVar.CurMenuIndex ++;
					MenuVar.FlashFlag |= 0x02;//0x02---需要刷新高亮行
				}
				else
				{
					MenuVar.MenuScrollCount=0;
					MenuVar.FlashFlag |= 0x01;//0x01---需要刷新整个界面
					MenuVar.CurMenuIndex=0;
		            MenuVar.OldHighLightRow = MenuVar.CurHighLightRow=0;
				}
				PostKey (KEY_FLASHINTERFACE);
				break;

			case KEY_R1://enter
				if (beepKG[0] == '1')
				{
					BeepNormal();
				}
				
				SelectItemIndex[0] = MenuVar.CurMenuIndex;
				return KEY_Enter;
				
			case KEY_R2://exit
				if (beepKG[0] == '1')
				{
					BeepNormal();
				}
				
				return KEY_ReturnInitInterface;

			case KEY_L1://return
				if (beepKG[0] == '1')
				{
					BeepNormal();
				}
				
				return KEY_ReturnPrevious;

			case KEY_FLASHINTERFACE:
				if(MenuVar.MenuLayerCnt!=0)
				{
				    if(MenuVar.MenuLayerCnt>MaxItems)break;
					MenuVar.CurMenuIndex=MenuVar.MenuLayerCnt-1;
					MenuVar.MenuLayerCnt=0;
					if (MenuVar.CurMenuIndex >= _4RowOneScr)
                	{
                		MenuVar.MenuScrollCount = (MenuVar.CurMenuIndex + 1) - _4RowOneScr;
                		MenuVar.CurHighLightRow = _4RowOneScr - 1;
                		MenuVar.OldHighLightRow = _4RowOneScr - 1;
                	}
                	else
                	{
                	    MenuVar.MenuScrollCount=0;
                		MenuVar.CurHighLightRow = MenuVar.CurMenuIndex;
                		MenuVar.OldHighLightRow = MenuVar.CurMenuIndex;
                	}
					MenuVar.FlashFlag |= 0x01;//0x01---需要刷新整个界面
					if (beepKG[0] == '1')
				    {
					    BeepNormal();
				    }				
				    SelectItemIndex[0] = MenuVar.CurMenuIndex;
					return KEY_Enter;
				}
				
				if ( (MenuVar.FlashFlag & 0x01) == 0x01) //0x01---需要刷新整个界面
				{
					kmy_LcdClear();

					if (MaxItems >= 1)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar.MenuScrollCount+1);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[0 + MenuVar.MenuScrollCount]);
						text_out_to_row(0,(unsigned char *)tmpbuff);
					}

					if (MaxItems >= 2)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar.MenuScrollCount+2);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[1 + MenuVar.MenuScrollCount]);
						text_out_to_row(1,(unsigned char *)tmpbuff);
					}
					if (MaxItems >= 3)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar.MenuScrollCount+3);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[2 + MenuVar.MenuScrollCount]);
						text_out_to_row(2,(unsigned char *)tmpbuff);
					}

					if (MaxItems >= 4)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar.MenuScrollCount+4);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[3 + MenuVar.MenuScrollCount]);
						text_out_to_row(3,(unsigned char *)tmpbuff);
					}

					if (MaxItems >= 5)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar.MenuScrollCount+5);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[4 + MenuVar.MenuScrollCount]);
						text_out_to_row(4,(unsigned char *)tmpbuff);
					}

					DispHighLightRow (MenuVar.CurHighLightRow + 0); //高亮
					MenuVar.FlashFlag &= ~0x01;
				}

				if ( (MenuVar.OldHighLightRow != MenuVar.CurHighLightRow) && ( (MenuVar.FlashFlag & 0x02) == 0x02) ) //0x02---需要刷新高亮行
				{
					DispHighLightRow (MenuVar.OldHighLightRow + 0); //取消高亮
					DispHighLightRow (MenuVar.CurHighLightRow + 0); //高亮
					MenuVar.OldHighLightRow = MenuVar.CurHighLightRow;
					MenuVar.FlashFlag &= ~0x02;
				}

				break;

			default:
				
				break;
		}

	}
}

char Centered(char *des,char *src,int Total_Num,int Center_Num)
{
	char *p1 = NULL;
	char *p2 = NULL;
	unsigned char len=0;
	
	p1 = src;
	p2 = p1;
	
	while(1)
	{	
	    if(*p1 == '\n')
		{
			len = 0;
			p2 = p1;
		}
		else if(*p1 == ' ')
		{
			p2 = p1;
			len++;
		}
		else 
		{
			len++;
		}
       
		if(len==Center_Num+1)
		{
			*p2='\n';
			len=p1-p2;			
		}
		p1++;
		if(*p1 == '\0')break;
		if(p1 == NULL)return 0;
	};

    p1 = src;	
	p2 = strstr(p1,"\n");
	len = (Total_Num-(p2-p1))/2;
	for(;len>0;len--)
	{
		*des=' ';
		des++;
	}
	
	while(1)
	{		    
		if(*p1!='\n')
		{
			*des=*p1;
			if(*p1=='\0')return 1;
			des++;
			p1++;
		}
		else
		{
			*des='\n';
			des++;
			p1++;
	    	p2 = strstr(p1,"\n");
			if(p2==NULL)
			{
				p2=src+strlen(src);
			}
	        len = (Total_Num-(p2-p1))/2;
	        for(;len>0;len--)
			{
		        *des=' ';
		        des++;
			}
	    }		
		if(p1==NULL)break;
	}
	return 0;	
}

void DistributedAlign(char *left_buff, char *right_buff,unsigned char NumPerLine)
{
    unsigned char i,len;
	char rightbuff[30]={0};

    //strcpy(rightbuff,"\x9c");
	strcat(rightbuff,right_buff);
	len = NumPerLine - (strlen(left_buff) + strlen(rightbuff));
    if(len>=NumPerLine)
	{
		strcat(left_buff,rightbuff);
		strcat(left_buff,"\n");
		return;
	}
	for(i=0;i<len;i++)
	{
		strcat(left_buff," ");
	}	
	strcat(left_buff,rightbuff);
	strcat(left_buff,"\n");
}




char PrintOrder(ORDER_STRUCT *order, char *gOrderFood, char *CustomerComments)
{	
	char ret=0;
	char buff[2048]={0};
	char temp[400]={0};
/*
    unsigned int year = 0;
    unsigned char month = 0;
    unsigned char day = 0;
    unsigned char hour = 0;
    unsigned char minute = 0;
    unsigned char second = 0;
*/
    //kmy_PrinterSetcommand ("\x10\x05\x05\x10",4);
	
	clear_lcd();
	DrawTitle_to_row_Language2 (2, "Printing...", "Printing...");

/*
	kmy_PrinterSetcommand (WidthZoom1, sizeof (WidthZoom1) );
	kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );

    kmy_TimeGetTime (&year, &month, &day, &hour, &minute, &second, NULL);
	memset(buff,0,sizeof(buff));
	sprintf(buff,"      %04d-%02d-%02d %02d:%02d:%02d\n",year, month, day, hour, minute, second);
	ret=kmy_PrinterString((unsigned char*)buff);
	if (ret != Printer_OK) goto PrinterErrorExit;
*/	
	kmy_PrinterSetcommand (WidthZoom2, sizeof (WidthZoom2) );
	kmy_PrinterSetcommand (HighZoom2, sizeof (HighZoom2) );
	
	memset(buff,0,sizeof(buff));
	strcat(buff,"\n");
	if(strstr(order->OrderType,"1"))
	{
		strcat(buff,"Delivery");
	}
	else if(strstr(order->OrderType,"2"))
	{
		strcat(buff,"Collection");
	}
	strcat(buff,"\n\n");
	ret=kmy_PrinterString((unsigned char*)buff);
	if (ret != Printer_OK) goto PrinterErrorExit;
	

	kmy_PrinterSetcommand (WidthZoom2, sizeof (WidthZoom2) );
	kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );
	
	memset(buff,0,sizeof(buff));
	strcpy(buff,"Order No:");
	strcat(buff,order->OrderNumber);
	strcat(buff,"\n");
	
	ret=kmy_PrinterString((unsigned char*)buff);
	if (ret != Printer_OK) goto PrinterErrorExit;

	kmy_PrinterSetcommand (WidthZoom1, sizeof (WidthZoom1) );
	kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );

	memset(buff,0,sizeof(buff));
	//ResNameCheck(buff);
	strcpy(buff,"Restaurant ID:");
	strcat(buff,order->RestaurantID);
	AddCarriage(buff,32);
	strcat(buff,"\n--------------------------------\n");	
	ret=kmy_PrinterString((unsigned char*)buff);
	if (ret != Printer_OK) goto PrinterErrorExit;

	ret=kmy_PrinterString((unsigned char*)gOrderFood);
    if (ret != Printer_OK) goto PrinterErrorExit;
    
	memset(buff,0,sizeof(buff));
	memset(temp,0,sizeof(temp));
/*
	strcpy(temp,"Delivery Chg:");
    DistributedAlign(temp,order->DeliveryCharge,31);
	strcat(buff,temp);

	strcpy(temp,"CC+Han fees:");
    DistributedAlign(temp,order->CC_HandelingFees,31);
	strcat(buff,temp);
*/	

	strcpy(temp,"Discount:");
    DistributedAlign(temp,order->Discount,31);
	strcat(buff,temp);
	
    strcat(buff,"--------------------------------\n");

	strcpy(temp,"Total:");
    DistributedAlign(temp,order->Total,31);
	strcat(buff,temp);
    ret=kmy_PrinterString((unsigned char*)buff);		
    if (ret != Printer_OK) goto PrinterErrorExit;

    kmy_PrinterSetcommand (WidthZoom2, sizeof (WidthZoom2) );
	kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );
		
	if(strlen(CustomerComments)>0)
	{
		memset(buff,0,sizeof(buff));
		strcat(buff,"Comments:");
		strcat(buff,"\n");
		ret=kmy_PrinterString((unsigned char*)buff);		
        if (ret != Printer_OK) goto PrinterErrorExit;

		kmy_PrinterSetcommand (WidthZoom1, sizeof (WidthZoom1) );
	    kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );

		memset(buff,0,sizeof(buff));
		AddCarriage(CustomerComments,32);
		strcat(buff,CustomerComments);
		strcat(buff,"\n");
		ret=kmy_PrinterString((unsigned char*)buff);		
        if (ret != Printer_OK) goto PrinterErrorExit;
	}

	kmy_PrinterSetcommand (WidthZoom2, sizeof (WidthZoom2) );
	kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );
	
	memset(buff,0,sizeof(buff));
	strcat(buff,"\n");
	strcat(buff,"Requested for:");
	strcat(buff,"\n");
	ret=kmy_PrinterString((unsigned char*)buff);		
    if (ret != Printer_OK) goto PrinterErrorExit;
	
	kmy_PrinterSetcommand (WidthZoom2, sizeof (WidthZoom2) );
	kmy_PrinterSetcommand (HighZoom2, sizeof (HighZoom2) );

	memset(buff,0,sizeof(buff));
	strcat(buff,order->RequestTime);
	strcat(buff,"\n");
	ret=kmy_PrinterString((unsigned char*)buff);		
    if (ret != Printer_OK) goto PrinterErrorExit;

	kmy_PrinterSetcommand (WidthZoom1, sizeof (WidthZoom1) );
	kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );
	
	memset(buff,0,sizeof(buff));
	strcpy(buff,"--------------------------------\n");
	ret=kmy_PrinterString((unsigned char*)buff);
	if (ret != Printer_OK) goto PrinterErrorExit;

	kmy_PrinterSetcommand (WidthZoom2, sizeof (WidthZoom2) );
	kmy_PrinterSetcommand (HighZoom2, sizeof (HighZoom2) );

	memset(buff,0,sizeof(buff));
    if(strstr(order->PaymentStatus,"6"))
    {
    	strcpy(buff,"Order Paid");
    }
    else
    {
    	strcpy(buff,"Order Not Paid");
    }
    strcat(buff,"\n");
    ret=kmy_PrinterString((unsigned char*)buff);
    if (ret != Printer_OK) goto PrinterErrorExit;
		
	kmy_PrinterSetcommand (WidthZoom1, sizeof (WidthZoom1) );
	kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );
	
	memset(buff,0,sizeof(buff));
	memset(temp,0,sizeof(temp));	
	strcat(buff,"Cus Info:");
	if(strstr(order->CustomerType,"4"))
	{
		strcat(buff,"(Verified)");
	}
	else
	{
		strcat(buff,"(Not Verified)");
	}
	strcat(buff,"\n");
	strcat((char *)temp,(char *)PrintPageFooter);
	AddCarriage((char *)temp,32);
	strcat(buff,(char *)temp);
	
	memset(temp,0,sizeof(temp));
	strcat((char*)temp,"\nName: ");
	strcat((char*)temp,order->CustomerName);
	strcat((char*)temp,"\nAddress: ");
	strcat((char*)temp,order->CustomerAddress);
	strcat((char*)temp,"\nCustomer Phone:");
	strcat((char*)temp,order->CustomerPhone);	
	AddCarriage((char*)temp,32);
	strcat(buff,(char *)temp);
	strcat(buff,"\n\n\n\n\n\n");

	ret=kmy_PrinterString((unsigned char*)buff);
	if (ret != Printer_OK) goto PrinterErrorExit;

	return Printer_OK;	
	PrinterErrorExit:
		if (ret == Printer_NoPaper)
		{
			clear_lcd();
			DrawTitle_to_row_Language2 (2, "No paper", "No paper");
			DrawTitle_to_row_Language2 (3, "Please insert it", "Please insert it");
		}
		else
		{
			clear_lcd();
			DrawTitle_to_row_Language2 (2, "print error", "print error");
			printf("print error=%d\r\n",ret);
		}

		return WaitkeyAndAutoExit (10);

}
char DisplayOrder(unsigned char num, ORDER_STRUCT *order, char *index, char *gOrderFood, char *CustomerComments)
{
	char i=0;
	char orderlist[ORDER_LIMIT][MENU_COL]={0};
	char retval=0;
	
	printf("========================\r\n");
	while(1)
	{
		for(i=0;i<num;i++)
		{
			strcpy(orderlist[i],order[i].OrderNumber);
			printf("OrderNumber:%s\r\n",orderlist[i]);
		}
		//Check_RequestTime(order,*index);

		if(AutoPrint==1) 
		{
			retval=KEY_Enter;
		}
		else 
		{
			//retval=GetSelectItemOfMenus(orderlist,num,index);
			retval=KEY_Enter;
		}
		if(retval==KEY_Enter)
		{		    
			
		}
		else
		{
			return retval;	
		}

	}
}
char GetSelectOrderID(char *src, char index, char *des)
{
	int i=0;
	char *p1=NULL,*p=NULL;
	int len=0;
	
	p1=src;
	p1=strstr(p1,"#");
	for(i=1;i<index;i++)
	{
		p1++;
		p1=strstr(p1,"#");
		p1++;
		p1=strstr(p1,"#");
	}
	
	if(p1!=NULL)
	{
		p1++;
		p1=strstr(p1,"*");
		if(p1==NULL)return 1;	
		p1++;
		p1=strstr(p1,"*");
		if(p1==NULL)return 1;	
		p1++;
		p=p1;
		p1=strstr(p1,"*");
		if(p1==NULL)return 1;	
		
		len=p1-p;
		strncpy(des,p,len);
		return 0;	
	}
	return 1;	
}

char GetSelectOrder(char *src, char index, char *des)
{
	char i=0;
	char *p;
	
	p=src;
	p=strstr(p,"#");
	for(i=1;i<index;i++)
	{
		p++;
		p=strstr(p,"#");
		p++;
		p=strstr(p,"#");
	}
	
	if(p!=NULL)
	{
		p++;
		*des='#';
		des++;
		for(i=0;*p!='#';p++,des++)
		{
			*des=*p;
		}
		*des=*p;
		des++;
		*des=0;

		return 0;
	}
	else
	{
		return 1;
	}
}


char DeleteOneOrder(char *buff,char index)
{
	char i;
	char *p;

	int len=0;


	p=buff;
	p=strstr(p,"#");
	for(i=1;i<index;i++)
	{
		printf("into here....\r\n");
		p++;
		p=strstr(p,"#");
		p++;
		p=strstr(p,"#");
	}
	if(p!=NULL)
	{
		buff=p;
		p++;
		p=strstr(p,"#");
		
		if(p!=NULL)
		{
			p++;
			len=p-buff;
		
			while(*p!=0)
			{
				*buff=*p;
				buff++;
				p++;
			}
			p=p-len;
			while(*p!=0)
			{
				*p=0;
				p++;
			}
		}
		else
		{
			while(*buff!=0)
			{
				*buff=0;
				buff++;
			}
		}
		return 0;
	}
	else
	{
		return 1;
	}
	
}
char ReadOutOrders(unsigned short ORDER_BASE, char (*OrdernumBuff)[MENU_COL])
{
	unsigned char total=0;
	int i=0;
	int offset=0;
	char temp[10]={0};
	int loop=0;

	kmy_EepromReadByte(ORDER_BASE-1,&total);
	if(ORDER_BASE==CONFIRMORDER_BASE)
	{
		printf("Read confirm orders total=%d\r\n",total);
	}
	else
	{
		printf("Read reject orders total=%d\r\n",total);
	}

	if(total==0)
	{
		clear_lcd();
		if(ORDER_BASE==CONFIRMORDER_BASE)
		{
			DrawTitle_to_row(1,"No Confirm Order");
		}
		else
		{
			DrawTitle_to_row(1,"No Reject Order");
		}
		WaitkeyAndAutoExit(10);
		return 1;
	}
	if (total>2*DISPLAY_MAXORDERID)// if not initial store memory
	{
		clear_lcd();
		if(ORDER_BASE==CONFIRMORDER_BASE)
		{
			DrawTitle_to_row(1,"No Confirm Order");
		}
		else
		{
			DrawTitle_to_row(1,"No Reject Order");
		}
		
		InitOrder();
		
		kmy_EepromWriteByte(ORDER_BASE-1,0);//store the total number
		WaitkeyAndAutoExit(10);
		return 1;
	}

	offset =(total-1)%DISPLAY_MAXORDERID;
	if(total<DISPLAY_MAXORDERID)
	{
		loop=total;
	}
	else
	{
		loop=DISPLAY_MAXORDERID;
	}
	for(i=0;i<loop;i++)
	{
		kmy_EepromReadSequent(ORDER_BASE+offset*10,(unsigned char*)temp,sizeof(temp));
		printf("temp[%d]=%s\r\n",i,temp);
		strcpy(OrdernumBuff[i],temp);
		if(offset==0)offset=DISPLAY_MAXORDERID;
		offset--;
	}
	return 0;
}
char StoreAnOrder(unsigned short ORDER_BASE,unsigned char *buff, int size)
{
	unsigned char total=0;
	unsigned short writeaddr=0;
	unsigned char testbuff[20]={0};
	int offset=0;
	
	
	kmy_EepromReadByte(ORDER_BASE-1,&total);
	if(ORDER_BASE==CONFIRMORDER_BASE)
	{
		printf("total_Confirm=%d\r\n",total);
	}
	else
	{
		printf("total_Reject=%d\r\n",total);
	}
	if(total>2*DISPLAY_MAXORDERID)total=0;//total loop from 0 to 2*DISPLAY_MAXORDERID
	
	offset=total%DISPLAY_MAXORDERID;
	writeaddr=ORDER_BASE+offset*size;
	kmy_EepromWrite(writeaddr,buff,size);
	
	kmy_EepromReadSequent(writeaddr,testbuff,size);
	printf("readstore=%s\r\n",testbuff);

	total++;
	if(total==2*DISPLAY_MAXORDERID)total=DISPLAY_MAXORDERID;
	
	kmy_EepromWriteByte(ORDER_BASE-1,total);//store the total number
	return 0;
}



char InputDeliveryTime(unsigned char* buff)
{
	char retval=0;

	
	clear_lcd();
	
	retval = GetString_123TimeInput ("Input Delivery Time",SRF_123,SRF_123,buff,6,0);
	return retval;
}
char CheckTimeFormat(char* buff)
{
	int time=0; 
	char *p=NULL;
	if(strlen(buff)<1)
	{
		return 1;
	}
	p=buff;
	p=strstr(p,":");
	if(p==NULL)
	{
		return 1;
	}
	time=atoi(buff);//printf("time1=%d\r\n",time);
	if(time<0||time>=24)
	{
		return 1;
	}
	p++;
	time=atoi(p);//printf("time2=%d\r\n",time);
	if(time<0||time>=60)
	{
		return 1;
	}
	return 0;
}

char AcceptOrder(ORDER_STRUCT *order)
{   
	char buff[2048]={0};
	unsigned char retval=0;
	char IP[45]={0};
	char Port[6]={0};
	char AcceptTimeNum=0;
	char AcceptTimeTable[10][MENU_COL]={0};
	char selectNum=0;

	char DeliveryTime[6]={0};
	char temp[MENU_COL]={0};

	SeperateIpAndPort((char*)Domain_IP,IP,Port);
	
	memset(buff,0,sizeof(buff));
	if(AutoAcceptOrder[0]=='1');
	else if(strcmp((char*)AcceptTimeMode,"1")==0)
	{
		GetRejectReason(AcceptTimeTable,(char*)AcceptTimeOptions,&AcceptTimeNum);
		if(AcceptTimeNum==0)
		{
			clear_lcd();
			DrawTitle_to_row(2,"no Accept Time");
			retval=WaitkeyAndAutoExit(10);
			selectNum=0;
			if(retval!=KEY_Enter)
			{
				return retval;
			}		
		}
		else
		{				
			retval=GetSelectItemOfMenus(AcceptTimeTable,AcceptTimeNum,&selectNum);
			if(retval!=KEY_Enter)
			{
				return retval;
			}
		}

	}
	else 
	{
		retval=InputDeliveryTime((unsigned char*)DeliveryTime);
	
	
		if(retval!=KEY_Enter)
		{
			return KEY_ReturnPrevious;	
		}
		if(CheckTimeFormat(DeliveryTime)==1)
		{
			clear_lcd();
			DrawTitle_to_row(2,"Time Format Error");	
			DrawTitle_to_row(3,"00:00");	
			WaitkeyAndAutoExit (8);
			printf("Time Format Error\r\n");
			return KEY_ReturnPrevious;
		}
	}
	
	clear_lcd();
	DrawTitle_to_row(2,"Connecting...");
	if(NetworkSwitch[0]==WiFi)
	{
    	retval = WiFi_Connect(Domain_IP);
    	if(retval!=TCP_ConnectOK)
    	{
    		clear_lcd();
    		DrawTitle_to_row(2,"Accept Order Failed");	
    		WaitkeyAndAutoExit (8);
    		printf("tcp connect fail\r\n");
    		return KEY_ReturnPrevious;
    	}			
	}
	else
	{
		retval=tcp_Connect(Domain_IP);
		if(retval!=TCP_OK)
		{	
			clear_lcd();
			DrawTitle_to_row(2,"Accept Order Failed");	
			WaitkeyAndAutoExit (8);
			printf("tcp connect fail\r\n");
			return KEY_ReturnPrevious;
		}
	}

	clear_lcd();
	DrawTitle_to_row(2,"Receiving...");

    memset(buff, 0, sizeof(buff));
	if(strcmp((char*)AcceptTimeMode,"1")==0)
	{
		strncpy(temp,AcceptTimeTable[selectNum],sizeof(temp)-1);
		my_string_replace((unsigned char*)temp,' ','_');//get rid of the space, use '_' instead,because http get can not accept space
		GetHttpCallbackString(buff,	(char*)CallbackURL, (char*)IP, (char*)Port,	(char *)ResID,
			                 (char*)LoginWebUsername, (char*)LoginWebPassword,	
			                  order->OrderNumber, "Accepted", "ok", temp);
		strcpy(order->Delivery,temp);
	}
	else 
	{
		GetHttpCallbackString(buff,	(char*)CallbackURL, (char*)IP, (char*)Port,	(char *)ResID,
			                 (char*)LoginWebUsername, (char*)LoginWebPassword,
			                  order->OrderNumber, "Accepted", "ok", DeliveryTime);
		strcpy(order->Delivery,DeliveryTime);
	}
	if(NetworkSwitch[0]==WiFi)
	{
   		WiFi_Send((unsigned char*)buff,strlen((char *)buff));
		memset(buff,0,sizeof(buff));
    	WiFi_Receive((unsigned char*)buff,sizeof(buff),"HTTP/1.1 200 OK",30);
		WiFi_Close();
	}
	else
	{
    	tcp_send((unsigned char*)buff,strlen(buff));
		memset(buff,0,sizeof(buff));
    	tcp_receive((unsigned char*)buff,sizeof(buff),"HTTP/1.1 200 OK",30);		
	}
	printf("Recv-->%s\r\n",buff);
	if(strstr(buff,"HTTP/1.1 200 OK"))
	{
		StoreAnOrder(CONFIRMORDER_BASE,(u8*)order->OrderNumber,10);
		printf("order->OrderNumber=%s\r\n",order->OrderNumber);
		memset(gOrderBuf,0,sizeof(gOrderBuf));
		clear_lcd();
		DrawTitle_to_row(2,"Accept Order Success");

    	memset(buff,0,sizeof(buff));
	    kmy_PrinterSetcommand (WidthZoom2, sizeof (WidthZoom2) );
	    kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );
		strcpy(buff,"Accepted for:\n");
		strcat(buff,order->Delivery);
		strcat(buff,"\n\n\n\n\n\n\n");
    	retval=kmy_PrinterString((unsigned char*)buff);
    	if (retval != Printer_OK) goto PrinterErrorExit;			
	}
	else
	{
		clear_lcd();
		DrawTitle_to_row(2,"Accept Order Failed");
		WaitkeyAndAutoExit (8);
		return KEY_ReturnPrevious;
	}
	
	WaitkeyAndAutoExit (8);
	return KEY_ReturnInitInterface;

PrinterErrorExit:
	if (retval == Printer_NoPaper)
	{
		clear_lcd();
		DrawTitle_to_row_Language2 (2, "No paper", "No paper");
		DrawTitle_to_row_Language2 (3, "Please insert it", "Please insert it");
	}
	else
	{
		clear_lcd();
		DrawTitle_to_row_Language2 (2, "print error", "print error");
		printf("print error=%d\r\n",retval);
	}

	return WaitkeyAndAutoExit (10);		
}

void GetRejectReason(char (*ReasonTable)[MENU_COL], char *Reason, char *RejectNum )
{
	int i=0;
	int len=0;
	char count=0;
	char *p;
	char *pt;

	len=strlen(Reason);
	if(len==0)return;
	
	for(i=0;i<len;i++)
	{
		if(Reason[i]==';')count++;
	}
	if(count==0)
	{
		strncpy(ReasonTable[0],Reason,20);
		*RejectNum=1;
		return;
	}
	p=Reason;
	if(count>15)count=15;
	for(i=0;i<count;i++)
	{
		pt=strstr(p,";");
		if(pt-p>20)
		{
			strncpy(ReasonTable[i],p,20);
		}
		else
		{
			strncpy(ReasonTable[i],p,pt-p);
		}
		pt++;
		p=pt;
		*RejectNum=count;
	}	
}
char RejectOrder(ORDER_STRUCT *order)
{
	char buff[2048]={0};
	unsigned char retval=0;
	char RejectReasonTable[15][MENU_COL]={0};
	char rejectNum=0;
	char selectNum=0;
	char IP[45]={0};
	char Port[6]={0};
	char Time[8]={0};
	unsigned char hour=0;
	unsigned char min=0;
	char temp[MENU_COL]={0};
	
	SeperateIpAndPort((char*)Domain_IP,IP,Port);
	kmy_TimeGetTime(NULL,NULL,NULL,&hour,&min,NULL,NULL);
	sprintf(Time,"%02d:%02d",hour,min);
	GetRejectReason(RejectReasonTable,(char*)RejectReason,&rejectNum);
	printf("RejectReason=%s,rejectNum=%d\r\n",RejectReason,rejectNum);

	if(rejectNum==0)
	{
		clear_lcd();
		DrawTitle_to_row(2,"no reject reason");
		retval=WaitkeyAndAutoExit(10);
		selectNum=0;
		if(retval!=KEY_Enter)
		{
			return retval;
		}		
	}
	else
	{
		retval=GetSelectItemOfMenus(RejectReasonTable,rejectNum,&selectNum);
		if(retval!=KEY_Enter)
		{
			return retval;
		}
	}
	


    strncpy(temp,RejectReasonTable[selectNum],sizeof(temp)-1);
	my_string_replace((unsigned char*)temp,' ','_');//get rid of the space, use '_' instead, because http get can not accept space
	clear_lcd();
	DrawTitle_to_row(2,"Connecting...");
	if(NetworkSwitch[0]==WiFi)
	{
    	retval = WiFi_Connect(Domain_IP);
    	if(retval!=TCP_ConnectOK)
    	{
    		clear_lcd();
    		DrawTitle_to_row(2,"Accept Order Failed");	
    		WaitkeyAndAutoExit (8);
    		printf("tcp connect fail\r\n");
    		return KEY_ReturnPrevious;
    	}
	}
	else
	{
		retval=tcp_Connect(Domain_IP);
		if(retval!=TCP_OK)
		{
			clear_lcd();
			DrawTitle_to_row(2,"Reject Order Failed");		
			WaitkeyAndAutoExit (10);	
			printf("tcp connect fail\r\n");
			return KEY_ReturnPrevious;
		}
	}
	clear_lcd();
	DrawTitle_to_row(2,"Receiving...");

	memset(buff,0,sizeof(buff));
	GetHttpCallbackString(buff, (char*)CallbackURL, (char*)IP, (char*)Port,	(char *)ResID,
		                 (char*)LoginWebUsername, (char*)LoginWebPassword,	
		                  order->OrderNumber, "Rejected", temp, Time);
	if(NetworkSwitch[0]==WiFi)
	{
    	WiFi_Send((unsigned char*)buff,strlen((char *)buff));
		memset(buff,0,sizeof(buff));
    	WiFi_Receive((unsigned char*)buff,sizeof(buff),"HTTP/1.1 200 OK",30);
		WiFi_Close();
	}
	else
	{
		tcp_send((unsigned char*)buff,strlen(buff));
		memset(buff,0,sizeof(buff));
    	tcp_receive((unsigned char*)buff,sizeof(buff),"HTTP/1.1 200 OK",30);		
	}
	printf("Recv-->%s\r\n",buff);
	if(strstr(buff,"HTTP/1.1 200 OK"))
	{		
		StoreAnOrder(REJECTORDER_BASE,(u8*)order->OrderNumber,10);
		printf("Reject_orderID=%s\r\n",order->OrderNumber);
		memset(gOrderBuf,0,sizeof(gOrderBuf));
		clear_lcd();
		DrawTitle_to_row(2,"Reject Order Success");
		
		memset(buff,0,sizeof(buff));
	    kmy_PrinterSetcommand (WidthZoom2, sizeof (WidthZoom2) );
	    kmy_PrinterSetcommand (HighZoom1, sizeof (HighZoom1) );
    	strcat(buff,"Rejected\n\n\n\n\n\n");
    	retval=kmy_PrinterString((unsigned char*)buff);
    	if (retval != Printer_OK) goto PrinterErrorExit;		
	}
	else
	{
		clear_lcd();
		DrawTitle_to_row(2,"Reject Order Failed");
		WaitkeyAndAutoExit (10);
		return KEY_ReturnPrevious;
	}
	
	
	WaitkeyAndAutoExit (10);
	return KEY_ReturnInitInterface;

PrinterErrorExit:
	if (retval == Printer_NoPaper)
	{
		clear_lcd();
		DrawTitle_to_row_Language2 (2, "No paper", "No paper");
		DrawTitle_to_row_Language2 (3, "Please insert it", "Please insert it");
	}
	else
	{
		clear_lcd();
		DrawTitle_to_row_Language2 (2, "print error", "print error");
		printf("print error=%d\r\n",retval);
	}

	return WaitkeyAndAutoExit (10);			
}



char AcceptRejectMenu(ORDER_STRUCT *order)
{

	const static struct MenuFuc_Orders Menu[2] =
	{
		{"1.AcceptOrder", "1.AcceptOrder", AcceptOrder},
		{"2.RejectOrder", "2.RejectOrder", RejectOrder}
	} ;
	struct MenuOperate_Orders MenuOp =
	{
		(struct MenuFuc_Orders*) Menu,
		2,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0,
	};

	while (1)
	{
		MenuHandler_Orders(&MenuOp);
		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (order);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface )
			{
				CurProc = CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else
			{
				kmy_LcdClear();
				MenuOp.flashflag = 1;
			}
		}
		else
		{
			InitFlag = 1;
			CurProc = CurProc_Return;
			return MenuOp.RetVal;
		}
	}
	
}

void InitOrder(void)
{
	kmy_EepromWriteByte(TOTAL_CONFIRMADDR,0);
	kmy_EepromWriteByte(TOTAL_REJECTADDR,0);
	printf("Inint eeprom order base...\r\n");
}

char Latest_Orders (char *msg)
{
	char *p1 = NULL;
	unsigned char num=0;
	ORDER_STRUCT order={0};//one ORDER_STRUCT size 
	char retval=0;
	char gOrderFood[2048]={0};
	char CustomerComments[200]={0};

	WaitKeyRelease();
	ReceiveOrderFlag=0;
	printf("ORDER_STRUCT size=%d\r\n",sizeof(ORDER_STRUCT));
	if(Net_ONOFF==1)
	{
		clear_lcd();
		DrawTitle_to_row(1,"Network is OFF");
		DrawTitle_to_row(2,"check Network ON/OFF");
		
		WaitkeyAndAutoExit(10);
		return KEY_ReturnPrevious;
	}
	if(Net_State==0)
	{
		clear_lcd();
		DrawTitle_to_row(1,"Network not open yet");
		DrawTitle_to_row(2,"please check the open ");		
		DrawTitle_to_row(3,"and close time");
		WaitkeyAndAutoExit(10);
		return KEY_ReturnPrevious;
	}

	if(writeto_gOrderBuf==1)
	{
		clear_lcd();
		DrawTitle_to_row(2,"Please wait...");
		while(1)
		{
			if(Writting_gOrderBuf==0)break;
		}
	}
	p1=strstr((char*)gOrderBuf,"Content-Length: 0");
	if(p1!=NULL)
	{
		clear_lcd();
		DrawTitle_to_row(1,"No order");
		
		WaitkeyAndAutoExit(10);
		return KEY_ReturnPrevious;

	}
	num=GetCountOfOrders((char*)gOrderBuf);
	printf("num=%d\r\n",num);

	if(num==0)
	{
		clear_lcd();
		DrawTitle_to_row(1,"No order");
		
		WaitkeyAndAutoExit(10);
		return KEY_ReturnPrevious;

	}

	p1 = gOrderBuf;
	memset(gOrderFood,0,sizeof(gOrderFood));
	memset(CustomerComments,0,sizeof(CustomerComments));
	GetOneOrder(p1,&order,gOrderFood,CustomerComments);

    kmy_EnableDisableTime3(0);
	retval=PrintOrder(&order,gOrderFood,CustomerComments);

	if(retval==Printer_OK)
	{
	    AcceptRejectMenu(&order);
	}
	kmy_EnableDisableTime3(1);
	return KEY_ReturnPrevious;
}

char Reject_Orders (char *msg)
{	
	unsigned char total=0;
	char index=0;
	char OrdernumBuff[DISPLAY_MAXORDERID][MENU_COL]={0};
	char retval=0;
	char ret=0;
	
	
	WaitKeyRelease();
	
	if(ReadOutOrders(REJECTORDER_BASE,OrdernumBuff)==1)
	{
		return KEY_ReturnPrevious;
	}
	
	kmy_EepromReadByte(TOTAL_REJECTADDR,&total);
	retval=GetSelectItemOfMenus(OrdernumBuff,total,&index); 
	if(retval==KEY_Enter)
	{
		WaitKeyRelease();
		clear_lcd();
		text_out_to_row(0,"1.Delete orders");
		DispHighLightRow(0);
		ret=WaitkeyAndAutoExit(10);
		if(ret==KEY_Enter)
		{
			WaitKeyRelease();
			
			kmy_EepromWriteByte(TOTAL_REJECTADDR,0);
			
			clear_lcd();
			DrawTitle_to_row(1,"Delete success");			
			return WaitkeyAndAutoExit(3);
		}	
	}
	return retval;
}

char Confirm_Orders (char *msg)
{	
	unsigned char total=0;
	char index=0;
	char OrdernumBuff[DISPLAY_MAXORDERID][MENU_COL]={0};
	char retval=0;
	char ret=0;

	
	WaitKeyRelease();
	
	if(ReadOutOrders(CONFIRMORDER_BASE,OrdernumBuff)==1)
	{
		return KEY_ReturnPrevious;
	}

	kmy_EepromReadByte(TOTAL_CONFIRMADDR,&total);
	retval=GetSelectItemOfMenus(OrdernumBuff,total,&index);
	if(retval==KEY_Enter)
	{	
		WaitKeyRelease();
		clear_lcd();
		text_out_to_row(0,"1.Delete orders");
		DispHighLightRow(0);
		ret=WaitkeyAndAutoExit(10);
		if(ret==KEY_Enter)
		{	
			WaitKeyRelease();
			
			kmy_EepromWriteByte(TOTAL_CONFIRMADDR,0);
			clear_lcd();
			DrawTitle_to_row(1,"Delete success");			
			return WaitkeyAndAutoExit(3);
		}
		
	}
 	return retval;
}

void gprs_SendATCmd (unsigned char *SendString)
{
	int i = 0xFFF;
	
	kmy_USART2SendString (SendString);

	printf ("\r\n%s : %s\r\n", __func__, SendString);
	while(i--);
}

void GetUPgradeGETString(char* destbuff, char* pageurl,char* IP,
	                             char* RestaurantID)
{
    strcpy(destbuff, "GET ");
	strcat(destbuff, pageurl);
	/*
	if(strlen(RestaurantID)!=0)
	{	
		strcat(destbuff, "?");
		strcat(destbuff, "a=");
		strcat(destbuff, RestaurantID);
		strcat(destbuff, "&ver=main_ver0.1.bin");
	}
	*/
	
	strcat(destbuff, " HTTP/1.1");
	strcat(destbuff, "\r\n");

	//Range: bytes=0-  
//	strcat(destbuff, "Range: bytes=0-");
//	strcat(destbuff, "\r\n");	
	
	//Host: 192.168.5.1
	strcat(destbuff, "Host: ");
	strcat(destbuff, IP);
	strcat(destbuff, "\r\n");
	
	//Pragma: no-cache
	strcat(destbuff, "Pragma: no-cache");
	strcat(destbuff, "\r\n");
	
	//Accept: */*
	strcat(destbuff, "Accept: */*");
	strcat(destbuff, "\r\n");
	
	//Proxy-Connection: Keep-Alive
	strcat(destbuff, "Proxy-Connection: Keep-Alive");
	strcat(destbuff, "\r\n");
	strcat(destbuff, "\r\n");
}

void GPRS_Thread(void)//10ms
{
	#define AUTOCHECK_WAIT 0
	#define AUTOCHECK_ATE 1
	#define AUTOCHECK_GET_STATE 2
	#define AUTOCHECK_DNS 3
	#define AUTOCHECK_CONNECT 4
	#define AUTOCHECK_CONNECT_OK 5
	#define AUTOCHECK_SEND 6
	#define AUTOCHECK_CHECK_SEND_CHAR 7
	#define AUTOCHECK_SENDDATAS 8
	#define AUTOCHECK_SEND_OK 9
	#define AUTOCHECK_RECV 10	
	#define AUTOCHECK_PROCESSING 11
	#define AUTOCHECK_EXIT 12

	unsigned int SendLen = 0;
	static unsigned int RecvLen = 0;
	char LengthBuff[6] = {0};
	rak_close_t uClose;
	static unsigned char AutoCheckState = 0;
	static uint16_t AutoCheckInterval = 0;
	static uint16_t AutoCheckTimeout = 0;
	static char Upgradecnt=3;
    static unsigned char lcdcnt=0;
    static unsigned int count10ms=0;
	
	static char IP[100]={0};
	static char Port[6]={0};
	
	if(count10ms%18000==0)
	{		
		kmy_NetStateSet();
	}
	
	count10ms++;
	
	if(Net_ONOFF==1||Net_State==0)return;//GRPS is OFF
	if(ReceiveOrderFlag==1)return;
	
	if(LcdBlackLightFlash==1)
	{
		if(count10ms%100==0)
		{
			lcdcnt++;
			if(lcdcnt%2==0)
			{
				kmy_LcdBlackLightOn();
			}
			else
			{
				kmy_LcdBlackLightOff();
			}
		}
		
	}

    if(NetworkSwitch[0]==WiFi)       // WiFi
    {
	    if(strlen((char*)SSID)==0)return;
    	switch(AutoCheckState)
    	{
    		case AUTOCHECK_WAIT:
			    	
    			check_Interval=atoi((char*)CheckOrderInterval);
    			if(check_Interval<=0)check_Interval=10;//10ms*1000=10s
				if((AutoCheckInterval>=100)&&(AutoCheckInterval%100==0))
				{
					if(rak_checkPktIrq() == RAK_TRUE)
					{
						rak_read_packet(&uCmdRspFrame); 	
						rak_clearPktIrq(); 
					    printf("Debug\r\n");
					    for(lcdcnt=0;lcdcnt<30;lcdcnt++)
					    {
					        printf("%02x ",uCmdRspFrame.CmdRspBuf[lcdcnt]);
					    }
						printf("\r\n");					
					}
				}
    			if(AutoCheckInterval++ > (100*check_Interval))
    			{
					if(SyncFlag==0&&kmy_TimeGetRtcSeconds()>=SyncSec)
					{
						printf("begin to sync...\n");
						if(SYNC_Time()<0)
    					printf("sync time Failed!\n");
						else
   						printf("sync time Succeed!\n");
						
					}
					
    				AutoCheckInterval = 0;
    				ATsendreceive_Lock=1;
					memset((unsigned char *)IP, 0, sizeof(IP));
        		    memset((unsigned char *)Port, 0, sizeof(Port));	
					SeperateIpAndPort((char*)Domain_IP,IP,Port);
					printf("--------WiFi---------\r\n");
    				printf("AUTOCHECK_WAIT=%d\r\n",check_Interval);
					memset(uCmdRspFrame.CmdRspBuf,0,sizeof(uCmdRspFrame.CmdRspBuf));
/*					if(spi_send_head(SEND_DATA_CMD,0)!=RUN_OK)
					{
						AutoCheckState=AUTOCHECK_EXIT;
						break;
					}
*/					AutoCheckState=AUTOCHECK_GET_STATE;
    			}
    			else
    			{
    				ATsendreceive_Lock=0;
    			}
    			break;
    		case AUTOCHECK_GET_STATE:
/*    			
				if(rak_checkPktIrq() == RAK_TRUE)
				{
				    AutoCheckTimeout = 0;
					rak_read_packet(&uCmdRspFrame);     
		            rak_clearPktIrq(); 
					if(uCmdRspFrame.CmdRspBuf[0]==RSPCODE_SOCKET_SVR)
					{
						AutoCheckState=AUTOCHECK_SENDDATAS;
						printf("AUTOCHECK_GET_STATE: Already connect\r\n");
						break;
					}
					else
					{						
*/	                    
        				if(rak_checkPktIrq() == RAK_TRUE)
        				{
        				    AutoCheckTimeout = 0;
        					rak_read_packet(&uCmdRspFrame);     
        		            rak_clearPktIrq();  
        				}
						if(DEST_PORT==0)DEST_PORT=atoi(Port);
                        if(DEST_IP!=0)
                        {
    						AutoCheckState=AUTOCHECK_CONNECT;
    						printf("AUTOCHECK_GET_STATE: Goto connect\r\n");                         	
                        }
                        else if(IP[0]>='0'&&IP[0]<='9')// IP connect
    					{
    					    rak_asciiDotAddressTouint32((uint8 *)&DEST_IP,(int8_t*)IP);
    						AutoCheckState=AUTOCHECK_CONNECT;
    						printf("AUTOCHECK_GET_STATE: Goto connect\r\n"); 						
    					}
    					else                      //Domain connect
    					{
    						rak_strapi.uDnsFrame.cmd = DNS_QUERY_CMD;
							strcpy((char*)rak_strapi.uDnsFrame.name,IP);
							if(RAK_SendCMD(DNS_QUERY_CMD,&rak_strapi.uDnsFrame, sizeof(rak_dns_t))!=RUN_OK)
							{
							    printf("AUTOCHECK_GET_STATE: Send DNS CMD Fail\r\n");
								AutoCheckState=AUTOCHECK_EXIT;
								break;
							}
							AutoCheckState=AUTOCHECK_DNS;
							printf("AUTOCHECK_GET_STATE: Goto DNS\r\n");

    					}
/*					}
				}
    			else if(AutoCheckTimeout++ > 1000)
    			{
    				AutoCheckTimeout = 0;
					rak_read_packet(&uCmdRspFrame);     
		            rak_clearPktIrq(); 					
					WiFiState=0;
    				AutoCheckState = AUTOCHECK_EXIT;	
					printf("AUTOCHECK_GET_STATE: Time out\r\n");
    			}				
*/    			break;

			case AUTOCHECK_DNS:
				if((AutoCheckTimeout>=100)&&(AutoCheckTimeout%100==0))
				{				
    				if(rak_checkPktIrq() == RAK_TRUE)
    				{
            			AutoCheckTimeout = 0;
            			rak_read_packet(&uCmdRspFrame); 	
            			rak_clearPktIrq(); 
    					if(uCmdRspFrame.DnsFrame.status!=RUN_OK)
    					{
    					    printf("AUTOCHECK_DNS: DNS Fail\r\n");
    						AutoCheckState=AUTOCHECK_EXIT;
    						break;
    					}
    					memcpy((uint8*)&DEST_IP,uCmdRspFrame.DnsFrame.addr,4);
    					printf("Dest IP: %2x %2x %2x %2x\r\n",uCmdRspFrame.DnsFrame.addr[0],uCmdRspFrame.DnsFrame.addr[1],uCmdRspFrame.DnsFrame.addr[2],uCmdRspFrame.DnsFrame.addr[3]);
    					AutoCheckState=AUTOCHECK_CONNECT;
    				}
				}
    			else if(AutoCheckTimeout++ > 1000)
    			{
    				AutoCheckTimeout = 0;
					rak_read_packet(&uCmdRspFrame);     
		            rak_clearPktIrq(); 					
    				AutoCheckState = AUTOCHECK_EXIT;	
					printf("AUTOCHECK_DNS: Time out\r\n");
    			}				
    			break;				
    		case AUTOCHECK_CONNECT:	
				if(rak_socket(LOCAL_PORT,DEST_PORT,RAK_MODULE_SOCKET_MODE,(uint32_t)DEST_IP)!=RUN_OK)
				{
				    printf("AUTOCHECK_CONNECT: Fail to open socket\r\n");
					AutoCheckState=AUTOCHECK_EXIT;
					break;
				}
				AutoCheckState=AUTOCHECK_CONNECT_OK;
				printf("AUTOCHECK_CONNECT: Goto Connect ok\r\n");

    			break;
    		case AUTOCHECK_CONNECT_OK:
				if((AutoCheckTimeout>=100)&&(AutoCheckTimeout%100==0))
				{
					if(rak_checkPktIrq() == RAK_TRUE)
					{
						AutoCheckTimeout = 0;
						rak_read_packet(&uCmdRspFrame); 	
						rak_clearPktIrq(); 
						if(uCmdRspFrame.socketFrameRcv.status!=RUN_OK)
						{
							printf("Rsp:\r\n");
							for(lcdcnt=0;lcdcnt<30;lcdcnt++)
							{
								printf(" %2x",uCmdRspFrame.CmdRspBuf[lcdcnt]);
							}
							printf("\r\nEnd\r\n");

							printf("AUTOCHECK_CONNECT_OK: Fail to open socket\r\n");
							WiFiState=0;
							AutoCheckState=AUTOCHECK_EXIT;
							break;						
						}
						printf("Rsp:\r\n");
						for(lcdcnt=0;lcdcnt<30;lcdcnt++)
						{
							printf(" %2x",uCmdRspFrame.CmdRspBuf[lcdcnt]);
						}
						printf("\r\nEnd\r\n");
						
						socket_flag=uCmdRspFrame.recvFrame.socket_flag; 
						printf("AUTOCHECK_CONNECT_OK: socket_flag->%d\r\n",socket_flag);
						AutoCheckState=AUTOCHECK_SENDDATAS;
						WiFiState=1;
					}
				}
    			if(AutoCheckTimeout++ > 1000)
    			{
    				AutoCheckTimeout = 0;
					rak_read_packet(&uCmdRspFrame);     
		            rak_clearPktIrq(); 					
					WiFiState=0;
    				AutoCheckState = AUTOCHECK_EXIT;	
					printf("AUTOCHECK_CONNECT_OK: Time out\r\n");
    			}
    			break;

    		case AUTOCHECK_SENDDATAS:
    			memset (pBufNonBlock, 0, sizeof (pBufNonBlock) );
				
				if(strlen((char*)VersionURL)==0)Upgradecnt=0;
				if(Upgradecnt>0)Upgradecnt--;
    			if(Upgradecnt<=0)
    			{
					GetHttpGETString(pBufNonBlock,(char*)RequestWebURL, (char*)ResID, 
									(char*)LoginWebUsername, (char*)LoginWebPassword, 
									(char*)IP);
    			}
    			else
    			{
    				GetUPgradeGETString(pBufNonBlock, (char *)VersionURL,
    					               (char *)IP,(char *)ResID);					
    			}
				
				if(send_data(0,0,socket_flag,strlen(pBufNonBlock),(unsigned char*)pBufNonBlock)!=RUN_OK)
				{
					AutoCheckState=AUTOCHECK_EXIT;
					break;                  
				}
				memset(gOrderBuf,0,sizeof(gOrderBuf));
    			AutoCheckState=AUTOCHECK_RECV;
    			AutoCheckTimeout = 0;
    			printf("AUTOCHECK_SENDDATAS: send->\r\n%s\r\n",pBufNonBlock);
    			break;

    		case AUTOCHECK_RECV:
				if((AutoCheckTimeout>=100)&&(AutoCheckTimeout%100==0))
				{
    				if(rak_checkPktIrq() == RAK_TRUE)
    				{
    					rak_read_packet(&uCmdRspFrame);
    					rak_clearPktIrq();
    					if(uCmdRspFrame.rspCode[0]==RSPCODE_RECV_DATA)
    					{				
    						//printf("%s",uCmdRspFrame.recvFrame.recvDataBuf);
    						RecvLen+=uCmdRspFrame.recvFrame.data_len;
    						if(RecvLen<sizeof(gOrderBuf))strcat((char*)gOrderBuf,(char*)uCmdRspFrame.recvFrame.recvDataBuf);
    						if(strstr((char*)uCmdRspFrame.recvFrame.recvDataBuf,"HTTP"))
    						{
    							AutoCheckState = AUTOCHECK_PROCESSING;
    							AutoCheckTimeout = 0;
    							printf("AUTOCHECK_RECV: Get HTTP\r\n");
    						}				
    						memset(uCmdRspFrame.CmdRspBuf,0,sizeof(uCmdRspFrame.CmdRspBuf));
    					}
    				}
				}
    			if(AutoCheckTimeout++ > 1000)
    			{
    				AutoCheckTimeout = 0;
					rak_read_packet(&uCmdRspFrame);     
		            rak_clearPktIrq(); 	
    				AutoCheckState = AUTOCHECK_EXIT;
    				printf("AUTOCHECK_RECV time out\r\n");
    			}

    			break;
    		case AUTOCHECK_PROCESSING:
				if((AutoCheckTimeout>=100)&&(AutoCheckTimeout%100==0))
				{				
    				if(rak_checkPktIrq() == RAK_TRUE)
    				{
    					rak_read_packet(&uCmdRspFrame);
    					rak_clearPktIrq();
    					if(uCmdRspFrame.rspCode[0]==RSPCODE_RECV_DATA)
    					{				
    						RecvLen+=uCmdRspFrame.recvFrame.data_len;
    						if(RecvLen<sizeof(gOrderBuf))strcat((char*)gOrderBuf,(char*)uCmdRspFrame.recvFrame.recvDataBuf);				
    						memset(uCmdRspFrame.CmdRspBuf,0,sizeof(uCmdRspFrame.CmdRspBuf));
    					}
    				} 
				}
    			if(AutoCheckTimeout++>600)// 6 seconds for receive datas
    			{
					printf("Reclen=%d\r\n",RecvLen);
					RecvLen=0;
					printf("Receive-->%s\r\n",gOrderBuf);
/*					if(SyncTimeFlag)
					{
						if(strstr(gOrderBuf,"Date:"))
						{
							if(SyncTime(gOrderBuf))SyncTimeFlag=0;
						}
					}											
*/					if(strstr(gOrderBuf,"HTTP/1.1 200 OK"))
					{
        				if(Upgradecnt<=0)
        				{
							if(strstr(gOrderBuf,"#")!=NULL)
							{
								ReceiveOrderFlag=1;
								printf("have order\r\n");
							}
							else
							{
								ReceiveOrderFlag=0;
							}
							connectfail_conunt=0;
        				}
        				else
        				{
        				    Upgradecnt=0;	
							if(strstr(gOrderBuf,(char *)VersionString)) 
							{	
								Upgrade_Flag[0]='0';					
							}
							else if(strstr(gOrderBuf,"main_ver")) 
							{
								Upgrade_Flag[0]='1';								
								printf("Have New Version\r\n");
							}	
        				}
        				AutoCheckTimeout = 0;
        				AutoCheckState=AUTOCHECK_EXIT+1;//To next state default						
					}
					else
					{
						AutoCheckState=AUTOCHECK_EXIT;
					}    			
    			}    			
    			break;
    			
    		case AUTOCHECK_EXIT: //Failed exit
       			ATsendreceive_Lock=0;
				connectfail_conunt++;
    			AutoCheckState = AUTOCHECK_WAIT;
    			AutoCheckInterval = 0;
    			AutoCheckTimeout = 0;
            	printf("socket_close Start \r\n");
            	uClose.cmd = CLOSEPORT_CMD;
            	uClose.flag= socket_flag;
            	RAK_SendCMD(CLOSEPORT_CMD,&uClose, sizeof(rak_close_t));					
				printf("END:AUTOCHECK_EXIT\r\n");
//				printf("DEST_IP:%d,DEST_PORT:%d,socket_flag:%d\r\n",DEST_IP,DEST_PORT,socket_flag);
    			break;
    			
    		default:	//Normal exit
    			ATsendreceive_Lock=0;
    			AutoCheckState = AUTOCHECK_WAIT;
    			AutoCheckInterval = 0;
    			AutoCheckTimeout = 0;
            	printf("socket_close Start \r\n");
            	uClose.cmd = CLOSEPORT_CMD;
            	uClose.flag= socket_flag;
            	RAK_SendCMD(CLOSEPORT_CMD,&uClose, sizeof(rak_close_t));	    			
    			printf("END:Normal\r\n");
//				printf("DEST_IP:%d,DEST_PORT:%d,socket_flag:%d\r\n",DEST_IP,DEST_PORT,socket_flag);				
    			break;
    	}        	
    }
    else                 //GPRS
    {
    	switch(AutoCheckState)
    	{
    		case AUTOCHECK_WAIT:	
    			check_Interval=atoi((char*)CheckOrderInterval);
    			if(check_Interval<=0)check_Interval=10;//10ms*1000=10s
    			if(AutoCheckInterval++ > (100*check_Interval))
    			{
    				AutoCheckInterval = 0;
    				AutoCheckState=AUTOCHECK_ATE;//To next state
    				ATsendreceive_Lock=1;//GPRS_Thread occupy the USART2
    				
					printf("--------GPRS---------\r\n");
    				printf("AUTOCHECK_WAIT=%d\r\n",check_Interval);
					memset((unsigned char *)IP, 0, sizeof(IP));
        		    memset((unsigned char *)Port, 0, sizeof(Port));	
    				SeperateIpAndPort((char*)Domain_IP,IP,Port);
    				kmy_USART2ReceiveReset();				
    				kmy_USART2SendString(AT_TcpStatus);
    			}
    			else
    			{
    				ATsendreceive_Lock=0;
    			}
    			break;
    		case AUTOCHECK_ATE:
    			
    			if(AutoCheckTimeout++ > 5)
    			{
    				kmy_USART2ShowBuff();
    				if(kmy_USART2CheckBuff("CONNECT OK"))
    				{
    					AutoCheckState=AUTOCHECK_SEND;
    				}
    				else if(kmy_USART2CheckBuff("IP INITIAL")||kmy_USART2CheckBuff("IP STATUS")||kmy_USART2CheckBuff("IP CLOSE"))
    				{
    					kmy_USART2SendString(AT_ATE0);
    					kmy_USART2SendString(AT_TcpHeadOff);
    					if(IP[0]>='0'&&IP[0]<='9')// IP connect
    					{
    					    kmy_USART2SendString(AT_IPconnect);    						
    					}
    					else                      //Domain connect
    					{
    						kmy_USART2SendString(AT_DNSconnect);
    					}
    					AutoCheckState=AUTOCHECK_CONNECT;//To next state
    				}
    				else
    				{
    					kmy_USART2SendString(AT_TcpClose2);
    					
    					AutoCheckState=AUTOCHECK_EXIT+1;
    				}
    				
    				AutoCheckTimeout = 0;
    				kmy_USART2ReceiveReset();
    				printf("AUTOCHECK_ATE\r\n");
    			}
    			
    			break;
    		case AUTOCHECK_CONNECT:
    			if(AutoCheckTimeout++ > 5)
    			{
    				kmy_USART2ShowBuff();
					GetTcpConnectATString((unsigned char*)pBufNonBlock,(unsigned char *)IP,(unsigned char *)Port);
    				kmy_USART2ReceiveReset();
    				kmy_USART2SendString((unsigned char*)pBufNonBlock);
    				
    				AutoCheckState++;//To next state
    				
    				printf("send->%s\r\n",pBufNonBlock);
    				AutoCheckTimeout = 0;
    			}
    			break;
    		case AUTOCHECK_CONNECT_OK:
    			if(kmy_USART2CheckBuff("CONNECT OK"))
    			{
    				AutoCheckState++;//To next state
    				printf("CONNECT OK\r\n");
    				AutoCheckTimeout = 0;
    			}
    			else if(kmy_USART2CheckBuff("CONNECT FAIL"))
    			{
    				
    				kmy_USART2ShowBuff();
    				kmy_USART2SendString(AT_TcpClose2);				
    				printf("Connect Status: CONNECT FAIL\r\n");			
    				AutoCheckState = AUTOCHECK_EXIT;				
    						
    			}
    			else if(kmy_USART2CheckBuff("ALREADY CONNECT"))
    			{			
    				kmy_USART2SendString(AT_TcpClose2);
    				printf("Connect Status: ALREADY CONNECT\r\n");
    				
    				AutoCheckState = AUTOCHECK_EXIT+1;				
    				
    			}
    			else if(kmy_USART2CheckBuff("PDP DEACT"))
    			{			
    				kmy_USART2SendString(AT_TcpClose2);
    				printf("Connect Status: PDP DEACT\r\n");
    				AutoCheckState = AUTOCHECK_EXIT;			
    			}
    			else if(kmy_USART2CheckBuff("ERROR"))
    			{
    				kmy_USART2SendString(AT_TcpClose2);
    				kmy_USART2ShowBuff();
    				
    				printf("Connect Status: ERROR\r\n");				
    				AutoCheckState = AUTOCHECK_EXIT;				
    					
    			}
    			if(AutoCheckTimeout++ > 1000)
    			{
    				AutoCheckTimeout = 0;
    				AutoCheckState = AUTOCHECK_EXIT;				
    			}
    			break;
    		case AUTOCHECK_SEND:// send length
    			memset (pBufNonBlock, 0, sizeof (pBufNonBlock) );  
				
				if(strlen((char*)VersionURL)==0)Upgradecnt=0;				
				if(Upgradecnt>0)Upgradecnt--;
            	if(Upgradecnt<=0)
            	{
            		GetHttpGETString(pBufNonBlock,(char*)RequestWebURL, (char*)ResID, 
            						(char*)LoginWebUsername, (char*)LoginWebPassword, 
            						(char*)IP);
            	}
            	else
            	{
            		GetUPgradeGETString(pBufNonBlock, (char *)VersionURL,
            						   (char *)IP,(char *)ResID);
            	}

    						
    			SendLen = strlen((const char *)pBufNonBlock);
    			sprintf(LengthBuff,"%d",SendLen);
    			
    			memset (pBufNonBlock, 0, sizeof (pBufNonBlock) );			
    			strcpy ( pBufNonBlock, AT_TcpSend); 		
    			strcat ( pBufNonBlock, (char const *) LengthBuff);
    			strcat ( pBufNonBlock, "\r");
    			
    			kmy_USART2ReceiveReset();
    			kmy_USART2SendString((unsigned char*)pBufNonBlock);
    			printf("send->%s\r\n",pBufNonBlock);
    			AutoCheckState++;//To next state
    			
    			AutoCheckTimeout = 0;
    
    		
    			break;
    		case AUTOCHECK_CHECK_SEND_CHAR://wait ">"
    			if(kmy_USART2CheckBuff(">"))
    			{
    				printf(">\r\n");
    				AutoCheckState++;//To next state
    				AutoCheckTimeout = 0;
    				kmy_USART2ReceiveReset();					
    			}
    			else if(kmy_USART2CheckBuff("ERROR"))
    			{
    				printf("Send Status, Check '>': ERROR\r\n");
    				AutoCheckState = AUTOCHECK_EXIT;
    			}
    			/* Timeout Handling */
    			if(AutoCheckTimeout++ >  500)//5s
    			{
    				AutoCheckTimeout = 0;
    				AutoCheckState = AUTOCHECK_EXIT;
    				printf("Check '>' timeout\r\n");
    			}
    			break;
    		case AUTOCHECK_SENDDATAS:
    			memset (pBufNonBlock, 0, sizeof (pBufNonBlock) );    
            	if(Upgradecnt<=0)
            	{
            		GetHttpGETString(pBufNonBlock,(char*)RequestWebURL, (char*)ResID, 
            						(char*)LoginWebUsername, (char*)LoginWebPassword, 
            						(char*)IP);
            	}
            	else
            	{
            		GetUPgradeGETString(pBufNonBlock, (char *)VersionURL,
            						   (char *)IP,(char *)ResID);
            	}
    
    			kmy_USART2ReceiveReset();					
    			kmy_USART2SendString((unsigned char*)pBufNonBlock);
    			
    			AutoCheckState++;//To next state
    			
    			AutoCheckTimeout = 0;
    			printf("send->%s\r\n",pBufNonBlock);
    			break;
    		case AUTOCHECK_SEND_OK:
    			if(kmy_USART2CheckBuff("SEND OK"))
    			{
    				printf("get->SEND OK\r\n");
    				AutoCheckState++;//To next state
    				
    				AutoCheckTimeout = 0;
    			}
    			else if(kmy_USART2CheckBuff("SEND FAIL"))
    			{
    				printf("Send Status: SEND FAIL\r\n");
    				AutoCheckState = AUTOCHECK_EXIT;
    			}
    			else if(kmy_USART2CheckBuff("ERROR"))
    			{
    				printf("Send Status: ERROR\r\n");
    				AutoCheckState = AUTOCHECK_EXIT;
    			}
    			/* Timeout Handling */
    			if(AutoCheckTimeout++ > 1000)//10s timeout
    			{
    				AutoCheckTimeout = 0;
    				AutoCheckState = AUTOCHECK_EXIT;
    			}
    			break;
    		case AUTOCHECK_RECV:
    			
    			if(AutoCheckTimeout++ > 1000)//10 seconds time out, wait for server response
    			{
    				AutoCheckTimeout = 0;
    				AutoCheckState = AUTOCHECK_EXIT;
    				kmy_USART2ShowBuff();
    				printf("AUTOCHECK_RECV time out\r\n");
    			}
    			
    			if(kmy_USART2CheckBuff("HTTP/1.1 200 OK"))
    			{
    				AutoCheckState=AUTOCHECK_PROCESSING;//To next state
    				AutoCheckTimeout = 0;
    			}
    			else if(kmy_USART2CheckBuff("HTTP/1.1 404 Not Found"))
    			{
    				kmy_USART2ReceiveReset();					
    				AutoCheckTimeout = 0;
    				AutoCheckState = AUTOCHECK_EXIT;
    				
    				printf("HTTP/1.1 404 Not Found\r\n");
    			}
    			else if (kmy_USART2CheckBuff("HTTP/1.1 204 No Content"))//specially for Australia project, when no order respond No Content
    			{
    				kmy_USART2ShowBuff();
    				kmy_USART2ReceiveReset();					
    				AutoCheckTimeout = 0;
    				AutoCheckState=AUTOCHECK_EXIT+1;//To next state default
    			}
    			break;
    		case AUTOCHECK_PROCESSING:
    			
    			if(AutoCheckTimeout++>500)// 5 seconds for receive datas
    			{
    				kmy_USART2ShowBuff();
    				memset(gOrderBuf,0,sizeof(gOrderBuf));
    				SendLen=kmy_USART2ReadToEnd(gOrderBuf,sizeof(gOrderBuf));
/*					if(SyncTimeFlag)
					{
						if(strstr(gOrderBuf,"Date:"))
						{
							if(SyncTime(gOrderBuf))SyncTimeFlag=0;
						}
					}
*/					if(Upgradecnt<=0)
					{
						if((SendLen>260)&&(strstr(gOrderBuf,"#")!=NULL))
						{
							ReceiveOrderFlag=1;
							printf("have order\r\n");
						}
						else
						{
							ReceiveOrderFlag=0;
						}
						connectfail_conunt=0;
					}
					else
					{
    				    Upgradecnt=0;	
						if(strstr(gOrderBuf,(char *)VersionString)) 
						{	
							Upgrade_Flag[0]='0';					
						}
						else if(strstr(gOrderBuf,"main_ver")) 
						{
							Upgrade_Flag[0]='1';								
							printf("Have New Version\r\n");
						}
					}

    				AutoCheckTimeout = 0;
    				AutoCheckState=AUTOCHECK_EXIT+1;//To next state default
    			
    			}
    			break;
    			
    		case AUTOCHECK_EXIT: //Failed exit
    					
    			kmy_USART2ReceiveReset();	
				connectfail_conunt++;
    			ATsendreceive_Lock=0;
    			AutoCheckState = AUTOCHECK_WAIT;
    			AutoCheckInterval = 0;
    			AutoCheckTimeout = 0;
    			printf("END:AUTOCHECK_EXIT\r\n");
    			kmy_USART2SendString(AT_TcpClose1);
    			kmy_USART2SendString(AT_TcpClose2);
    			break;
    			
    		default:	//Normal exit
    			kmy_USART2ReceiveReset();					
    			ATsendreceive_Lock=0;
    			AutoCheckState = AUTOCHECK_WAIT;
    			AutoCheckInterval = 0;
    			AutoCheckTimeout = 0;
    			
    			printf("END:Normal\r\n");
    			break;
    	}
    }
}

void BeepLound (void)
{
	kmy_BeepOn();
	msleep (100);
	kmy_BeepOff();
}

char HaveReceiveOrder(void)
{
	int t=0;
	char keyval=0;
	char i=5;
	
	if(ReceiveOrderFlag==1)
	{	    
		kmy_EnableDisableTime3(0);
		t=atoi((char*)BeepDuration);
		LcdBlackLightFlash=1;
		clear_lcd();
		DrawTitle_to_row(1,"You have orders");
		DrawTitle_to_row(2,"please check");
		kmy_AutoPostKey(120);
		while(1)
		{
			if(AutoAcceptOrder[0]=='1')
			{			
				AutoPrint=1;// Auto print flag 
				while(i>0)
				{
					BeepLound();
					msleep(50);
					i--;
				}
				LcdBlackLightFlash=0;
				kmy_EnableDisableTime3(1);
				return KEY_Enter;
			}
			else
			{
				AutoPrint=0;
			}
			switch(t)
			{
				case 0:
					break;
				case 1:
					BeepLound();
					msleep(50);
					break;
				default:
					if(i>0)
					{
						BeepLound();
						msleep(50);
						i--;
						keyval = GetKeyValue();
					}
					break;
			}
			
			keyval = GetKeyValue();
			
			if (keyval == KEY_NONE) continue;
	
			if (keyval == KEY_Enter || keyval == KEY_ReturnInitInterface || keyval == KEY_ReturnPrevious || keyval == KEY_WaitTimeout) break;
		}
		LcdBlackLightFlash=0;
		kmy_EnableDisableTime3(1);
		return keyval;
	}
	return 0;
}

char HaveUpgrade(void)
{
	char i=10;
	
	if(StorageVariable()==0)return 0;	
	LcdBlackLightFlash=1;
	clear_lcd();
	DrawTitle_to_row(1,"Have upgrade");
	DrawTitle_to_row(2,"The POS will restart");
    while(i>0)
    {
    	BeepLound();
    	msleep(50);
    	i--;
    }
	Upgrade_Flag[0]='0';
	LcdBlackLightFlash=0;
	NVIC_SystemReset();
	return 0;
}

char CheckOrderFailWarn(void)
{
    char i=5;
	if(connectfail_conunt>=4)
	{
		connectfail_conunt=0;
		if((NetworkSwitch[0]==WiFi)&&(WiFiState==0))
		{
			kmy_EnableDisableTime3(0);		
			clear_lcd();
			DrawTitle_to_row(2,"Lose network");
			DrawTitle_to_row(3,"Try to reconnect");		
			while(i>0)
			{
				BeepLound();
				msleep(50);
				i--;
			}			
			rak_reset();
            kmy_WifiInit();
			kmy_EnableDisableTime3(1);			
			return KEY_ReturnInitInterface; 
		}
		else
		{
			kmy_EnableDisableTime3(0);				
			clear_lcd();
			DrawTitle_to_row(1,"TCP connect fail");
			text_out_to_row_x(2,6,"please check your IP");		
			text_out_to_row_x(3,6,"and port or signal");		
			text_out_to_row_x(4,6,"problem");	
			kmy_EnableDisableTime3(1);										
			return WaitkeyAndAutoExit(60); 
		}
	}
	return 0;
}

