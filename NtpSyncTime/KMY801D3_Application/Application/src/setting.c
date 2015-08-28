#include "setting.h"
#include "kmy_Time.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "key.h"
#include "lcd.h"
#include "main.h"
#include "global_extern_variable.h"
#include "gprs.h"
#include "MenuOperate.h"
#include "SelfTest.h"
#include "mypure_library.h"
#include "my_library.h"
#include "ShuRuFa.h"
#include "kmy_EEPROMDrv.h"
#include "EepromFileSystem.h"
#include "kmy_Misc.h"
#include "Version.h"
#include "kmy_USART1Drv.h"
#include "FoodOrder.h"
#include "kmy_wifi.h"
#include "kmy_GprsDrv.h"
#include "message.h"
#include "Wifi.h"
#include "kmy_PrinterDrv.h"


const unsigned char efile_Config[] = {"Config"};
const unsigned char Configfile_des3key[25] = {"106QDabcdefg106QD1234567"};
unsigned char dataledflag;

unsigned char LightPercent[4];				//light percent
unsigned char Domain_IP[51];			//ip 地址219.134.25.231,81
unsigned char ApnApn[50 + 1];
unsigned char ApnUsername[21 + 1];
unsigned char ApnPassword[21 + 1];
unsigned char EnterAdvancedSetPassWord[22];	//高级设置密码
unsigned char StorePIN[5];					//PIN name
unsigned char beepKG[2];
unsigned char ScreenOffTimeout[3];			

unsigned char TerminalID[17];				//终端id
unsigned char TerminalPassword[17];			//终端密码
unsigned char VersionURL[100+1];
unsigned char UpgradeURL[100+1];
unsigned char NetworkSwitch[2];
unsigned char Upgrade_Flag[2];

unsigned char RequestWebURL[150+1];
unsigned char CallbackURL[150+1];
unsigned char RejectReason[200+1];
unsigned char ResID[10+1];
unsigned char LoginWebUsername[20+1];
unsigned char LoginWebPassword[20+1];

unsigned char BeepDuration[2+1];
unsigned char AcceptTimeMode[1+1];
unsigned char CheckOrderInterval[3+1];
unsigned char NetOpenTime[10];
unsigned char NetCloseTime[10];
unsigned char PrintDensity[3+1];           //打印黑度
unsigned char PrintDateFormat[1+1];
unsigned char PrintTimeFormat[1+1];
unsigned char PrintPageHeader[30+1];
unsigned char PrintPageFooter[100+1];

unsigned char PrintReceiptCount[1+1];
unsigned char AutoAcceptOrder[1+1];
unsigned char AcceptTimeOptions[150+1];
unsigned char TimeZone[50+1];

//WiFi
unsigned char LocalIpPortBuf[28];			//本机IP及端口
unsigned char GatewayBuff[28];				//网关
unsigned char DNSBuff[28];					//DNS服务器
unsigned char MaskBuff[28];					//子网掩码

unsigned char WLANWP[30];			//wifi wp
unsigned char dhcpclient[2];
unsigned char SSID[30];			//wifi ssid


const unsigned char locallanguage[sizeof (Language)] = {OTHER_LANGUAGE, 0};

struct storeFile XStor[STORESIZE+1] =  	//the parameters are store in the eeprom address 0x540
{
	{beepKG, "1"},
	{LightPercent, "44"},
	{Language, locallanguage},
	{Domain_IP, "weslen.yupage.com,80"},	
	{NetworkSwitch, "G"},
	{Upgrade_Flag, "0"},
	{VersionURL, ""},
	{UpgradeURL, ""},
	{ApnApn, "internet"},
	{ApnUsername, ""},

	{ApnPassword, ""},
	{ScreenOffTimeout, "60"},
	{EnterAdvancedSetPassWord, "000000"},
	{StorePIN, DEFAULT_PIN},
	{TerminalID, ""},
	{TerminalPassword, ""},
	{RequestWebURL, "http://weslen.yupage.com/OrderRequest.php"},
	{CallbackURL, "http://weslen.yupage.com/Callback.php"},
	{RejectReason, "TOO BUSY;FOOD UNAVAILABLE;UNABLE TO DELIVER;"},
	{ResID, "25"},

	{LoginWebUsername, "demo"},	
	{LoginWebPassword, "123456789"},
	{BeepDuration, "1"},
	{AcceptTimeMode, ""},
	{CheckOrderInterval, "10"},
	{NetOpenTime, "00:00"},
	{NetCloseTime, "00:00"},
	{PrintDensity, "2"},
	{PrintDateFormat, ""},
	{PrintTimeFormat, ""},

	{PrintPageHeader, "Business Name"},	
	{PrintPageFooter, "New customer call to confirm order"},
	{PrintReceiptCount, ""},
	{AutoAcceptOrder, ""},
	{AcceptTimeOptions, "10 Minutes;15 Minutes;20 Minutes;25 Minutes;30 Minutes;35 Minutes;40 Minutes;"},
    {LocalIpPortBuf,"192.168.0.98"},
	{GatewayBuff,"192.168.0.100"},
	{MaskBuff,"255.255.255.0"},		
    {DNSBuff,"202.96.134.33"},
	{WLANWP,"123456789"},
	
    {dhcpclient,"1"},
	{TimeZone, "UTC+08:00"},
	{NULL, NULL} 	//后面不用的务必赋成 NULL，否则会引起segmentationfault
};


unsigned char StorageVariable (void)
{
	static unsigned char buff[2000];
	unsigned char i;
	unsigned char retval;
	struct EepromFile efp;

	buff[0] = 0;

	for (i = 0; i < STORESIZE; i++)
	{
		printf("%d==%s\r\n",i,(XStor[i].Viarable));
		strcat ( (s8*) buff, (sc8*) (XStor[i].Viarable) );
		strcat ( (s8*) buff, "\t");
	}

	retval = EepromFile_Open ((unsigned char*)efile_Config, &efp);

	if (retval != EepromFile_OpenOk)
	{
		{
			printf ("[StorageVariable error]=EepromFile_Open\r\n");
		}
		return 0;
	}

	retval = EepromFile_Write (buff, strlen ( (sc8*) buff) + 1, &efp);

	if (retval != EepromFile_WriteOk)
	{
		{
			printf ("[StorageVariable error]=EepromFile_Write\r\n");
		}
		return 0;
	}

	return 1;
}


unsigned char RestoreConfigFile (void)
{
	unsigned char buff[2000]={0};
	unsigned char i;
	unsigned char retval;

	buff[0] = 0;

	for (i = 0; i < STORESIZE; i++)
	{
	    
		strcpy ( (s8*) (XStor[i].Viarable), (sc8*) (XStor[i].initVal) );
		printf("%d=>%s\r\n",i,(XStor[i].initVal));
		strcat ( (s8*) buff, (sc8*) (XStor[i].initVal) );
		strcat ( (s8*) buff, "\t");
	}

	retval = EepromFile_Delete ((unsigned char*)efile_Config);

	if (retval == EepromFile_HaveNotThisFile)
	{
		{
			printf ("EepromFile_HaveNot Config file\r\n");
		}
	}
	EepromFile_FileSystemFormat();

	retval = EepromFile_WriteData (buff, strlen ( (sc8*) buff), efile_Config);

	if (retval != EepromFile_WriteOk)
	{
		{
			printf ("[RestoreConfigFile error]=EepromFile_Write\r\n");
		}
		return 0;
	}

	return 1;
}

unsigned char RestoreGlobalVariable (void)
{
	unsigned char buff[2000]={0};
	unsigned char i;
	unsigned char retval;
	unsigned char *p;
	char *pt;
	struct EepromFile efp;

	retval = EepromFile_Open ((unsigned char*)efile_Config, &efp);

	if (retval != EepromFile_OpenOk)
	{
		{
			printf ("[RestoreGlobalVariable error]=EepromFile_Open\r\n");
		}
		return 0;
	}

	retval = EepromFile_read (buff, sizeof (buff) - 1, &efp);

	if (retval != EepromFile_ReadOk)
	{
		{
			printf ("[RestoreGlobalVariable error]=EepromFile_read\r\n");
		}
		return 0;
	}
	

	pt = (char*)buff;
	for (i = 0; i < STORESIZE; i++)
	{
		pt=strstr(pt,"\t");
		if(pt==NULL)
		{
			printf("Add new global varible, format Eeprom and restart\r\n");
			return 2;
		}
		pt++;
	}
	pt=strstr(pt,"\t");
	if(pt!=NULL)
	{
		printf("Global varible have been changed, format Eeprom and restart\r\n");
		return 2;
	}
	
	p = buff;
		
	for (i = 0; i < STORESIZE; i++)
	{
		my_strcpy_EncounterHexEnd ( (XStor[i].Viarable), p, 300, '\t');
		my_MovePointToHex (&p, &buff[sizeof (buff) - 1], '\t');
		p++;
		printf ("%d=%s\r\n", i, XStor[i].Viarable);
	}

	return 1;
}


static char ContrastSet (char *msg)	//2.screen constract setting
{
#if 0
	return (abeyant() );
#else
	signed int NewPercent	= atoi ( (sc8*) LightPercent);
	signed int 	CurCursor	= 39 + ( (NewPercent - 56) / 4 * 3);
	signed int  OldCursor   = CurCursor;
	unsigned char Buf[50] = {'\0'};
	unsigned char keyval;

	clear_lcd();
	text_out_to_row_x_Language2 (0, 28, "【亮度设定】", "Contrast Set");

	text_out (17, 25, "-");
	text_out (105, 25, "+");
	draw_rect (26, 25, 77, 10, mode_OnlyEdge);
	draw_line (26, 30, 26 + 75 + 1, 30, 1);

	fill_rect (26 + 1 + CurCursor, 26, 3, 8, mode_Reverse);	//3*8

	memset (Buf, '\0', sizeof (Buf) );
	sprintf ( (s8*) Buf, "%d", NewPercent);
	strcat ( (s8*) Buf, "%");
	text_out (58, 48, Buf);

	while (1)
	{
		keyval = GetKeyValue();

		if (keyval == KEY_NONE) continue ;

		KeyEventPost();

		switch (keyval)
		{
			case KEY_UP:
				if (NewPercent + 4 <= 100)
				{
					NewPercent += 4;
					CurCursor += 3;
				}

				break;

			case KEY_DOWN:
				if (NewPercent - 4 >= 4)
				{
					NewPercent -= 4;
					CurCursor -= 3;
				}

				break;

			case KEY_Enter:
				sprintf ( (s8*) Buf, "%d", NewPercent);
				Buf[3] = 0;
				strcpy ( (s8*) LightPercent, (sc8*) Buf);

				if (!StorageVariable() )
				{
					DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
					WaitkeyAndAutoExit (10);
				}

				return KEY_Enter;

			case KEY_ReturnPrevious:
				NewPercent = atoi ( (sc8*) LightPercent);
				set_contrast (NewPercent);
				return KEY_ReturnPrevious;

			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
				NewPercent = atoi ( (sc8*) LightPercent);
				set_contrast (NewPercent);
				return KEY_ReturnInitInterface;

			default:
				break;
		}

		memset (Buf, '\0', sizeof (Buf) );
		text_out (58, 48, "    ");
		sprintf ( (s8*) Buf, "%d", NewPercent);
		strcat ( (s8*) Buf, "%");
		text_out (58, 48, Buf);
		KeyIsResponse();

		if (OldCursor != CurCursor)
		{
			fill_rect (26 + 1 + OldCursor, 26, 3, 8, mode_Reverse);	//3*8
			fill_rect (26 + 1 + CurCursor, 26, 3, 8, mode_Reverse);	//3*8
			OldCursor = CurCursor;
		}

#ifdef DEBUG_PRINTF
		printf ("NewPercent=%d\n", NewPercent);
#endif

		set_contrast (NewPercent);		// 24---52
	}

#endif
}

static char PrintDensitySet (char *msg)
{
	signed int NewPercent;
	signed int 	CurCursor;
	signed int  OldCursor;
	unsigned char Buf[50] = {'\0'};
	unsigned char keyval;

	NewPercent = atoi((char*)PrintDensity);
	CurCursor = NewPercent*4;
	OldCursor = CurCursor;
	
	clear_lcd();
	DrawTitle_to_row_Language2(0,"【打印黑度设置】", "Print Density Set");

	text_out (15, 25, "-");
	text_out (107, 25, "+");
	draw_rect (23, 25, 82, 10, mode_OnlyEdge);
	draw_line (23, 30, 23 + 80 + 1, 30, 1);

	fill_rect (23 + 1 + CurCursor, 26, 4, 8, mode_Reverse);	//4*8

	memset (Buf, '\0', sizeof (Buf) );
	sprintf ( (s8*) Buf, "%d", NewPercent);
	//strcat ( (s8*) Buf, "%");
	DrawTitle_to_row(4, Buf);

	while (1)
	{
		keyval = GetKeyValue();

		if (keyval == KEY_NONE) continue ;

		KeyEventPost();

		switch (keyval)
		{
			case KEY_UP:
				if (NewPercent + 1 <= 19)
				{
					NewPercent += 1;
					CurCursor += 4;
				}

				break;

			case KEY_DOWN:
				if (NewPercent - 1 >= 0)
				{
					NewPercent -= 1;
					CurCursor -= 4;
				}

				break;

			case KEY_Enter:
				memset (Buf, '\0', sizeof (Buf) );
				Buf[0]=0x10;Buf[1]=0x05;Buf[2]=0x05;Buf[3]=NewPercent;
				kmy_PrinterSetcommand((char*)Buf,4);
				
				sprintf ( (s8*) PrintDensity, "%d", NewPercent);
				if (!StorageVariable() )
				{
					DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
					WaitkeyAndAutoExit (10);
				}


				return KEY_Enter;

			case KEY_ReturnPrevious:
				return KEY_ReturnPrevious;

			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
				return KEY_ReturnInitInterface;

			default:
				break;
		}

		memset (Buf, '\0', sizeof (Buf) );
		sprintf ( (s8*) Buf, "%d", NewPercent);
		//strcat ( (s8*) Buf, "%");
		clear_area_to_row(4,4);
 		DrawTitle_to_row(4, Buf);

		KeyIsResponse();

		if (OldCursor != CurCursor)
		{		
			fill_rect (23 + 1 + OldCursor, 26, 4, 8, mode_Reverse);	//4*8
			fill_rect (23 + 1 + CurCursor, 26, 4, 8, mode_Reverse);	//4*8
			OldCursor = CurCursor;
		}
	}
}



static char Polish_Display_Keytone_Close (char *msg)
{
	memset (beepKG, 0, sizeof (beepKG) );
	beepKG[0] = '0';

	if (!StorageVariable() )
	{
		DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		WaitkeyAndAutoExit (10);
	}

	return 0;
}

static char Polish_Display_Keytone_Open (char *msg)
{
	memset (beepKG, 0, sizeof (beepKG) );
	beepKG[0] = '1';

	if (!StorageVariable() )
	{
		DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		WaitkeyAndAutoExit (10);
	}

	return 0;
}

static char BasicKeyTone (char *msg)	//key tone setting
{
	 static unsigned char ToneClose1[20] = {"1.关闭   "};
	 static unsigned char ToneClose2[20] = {"1.Close  "};
	 static unsigned char ToneOpen1[20] = {"2.打开   "};
	 static unsigned char ToneOpen2[20] = {"2.Open   "};

	static struct MenuFuc_Language2 Menu[]={
		{ToneClose1,ToneClose2,Polish_Display_Keytone_Close},
		{ToneOpen1,ToneOpen2,Polish_Display_Keytone_Open}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		sizeof(Menu) / sizeof(Menu[0]),
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	my_strdel (ToneClose1, '*');
	my_strdel (ToneClose2, '*');
	my_strdel (ToneOpen1, '*');
	my_strdel (ToneOpen2, '*');

	if (beepKG[0] == '0')
	{
		strcat ( (s8*) ToneClose1, "*");
		strcat ( (s8*) ToneClose2, "*");
	}
	else
	{
		strcat ( (s8*) ToneOpen1, "*");
		strcat ( (s8*) ToneOpen2, "*");
	}

	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
				my_strdel (ToneClose1, '*');
				my_strdel (ToneClose2, '*');
				my_strdel (ToneOpen1, '*');
				my_strdel (ToneOpen2, '*');

				if (beepKG[0] == '0')
				{
					strcat ( (s8*) ToneClose1, "*");
					strcat ( (s8*) ToneClose2, "*");
				}
				else
				{
					strcat ( (s8*) ToneOpen1, "*");
					strcat ( (s8*) ToneOpen2, "*");
				}
			}
		}
		else
		{
			return MenuOp.RetVal;
		}
	}
}


static char PowerOffScreenTime (char *msg)	//key tone setting
{
	unsigned char temp[20];
	unsigned char retval;
	PasswordInputArgument arg;

	clear_lcd();

	if (Language[0] == LOCAL_LANGUAGE)
	{
		strcpy ( (s8*) temp, "当前屏保时间:");
	}
	else
	{
		strcpy ( (s8*) temp, "Current time");
	}

	strcat ( (s8*) temp, (sc8*) ScreenOffTimeout);
	DrawTitle_to_row_Language2 (1, temp, temp);

	PassWordArgDeInit (&arg, temp, 2);
	arg.DiplayRow = 2;
	arg.LocalTitle = NULL;
	arg.xingflag = 0;
	arg.clearlcdflag = 0;
	arg.LocalTitle = "【屏保时间设置】";
	arg.OtherTitle = "Screen timeout set";
	retval = PassWordInput_MiddledDisplay (&arg);

	if (retval == KEY_Enter)
	{
		strcpy ( (s8*) ScreenOffTimeout, (sc8*) temp);
		retval = atoi ( (sc8*) temp);

		if (retval == 0) strcpy ( (s8*) ScreenOffTimeout, "30");

		if (!StorageVariable() )
		{
			DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2 (3, "更改成功!", "change success!");
		}

		retval = WaitkeyAndAutoExit (10);
	}

	return retval;
}


static char LanguageSet_English (char *msg)
{
	memset (Language, 0, sizeof (Language) );
	Language[0] = OTHER_LANGUAGE;

	if (!StorageVariable() )
	{
		DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		WaitkeyAndAutoExit (10);
	}

	return 0;
}
static char LanguageSet_Chinese (char *msg)
{
	memset (Language, 0, sizeof (Language) );
	Language[0] = LOCAL_LANGUAGE;

	if (!StorageVariable() )
	{
		DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		WaitkeyAndAutoExit (10);
	}

	return 0;
}
static char LanguageSet (char *msg)	//key tone setting
{
	static unsigned char ToneClose1[20] = {"1.中文   "};
	static unsigned char ToneClose2[20] = {"1.Chinese  "};
	static unsigned char ToneOpen1[20] = {"2.英文   "};
	static unsigned char ToneOpen2[20] = {"2.English  "};

	static struct MenuFuc_Language2 Menu[]={
		{ToneClose1,ToneClose2,LanguageSet_Chinese},
		{ToneOpen1,ToneOpen2,LanguageSet_English}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,sizeof(Menu) / sizeof(Menu[0]),0,0,0,MaxRowOneScr,1,0,0
	};

	my_strdel (ToneClose1, '*');
	my_strdel (ToneClose2, '*');
	my_strdel (ToneOpen1, '*');
	my_strdel (ToneOpen2, '*');

	if (Language[0] == LOCAL_LANGUAGE)
	{
		strcat ( (s8*) ToneClose1, "*");
		strcat ( (s8*) ToneClose2, "*");
	}
	else
	{
		strcat ( (s8*) ToneOpen1, "*");
		strcat ( (s8*) ToneOpen2, "*");
	}

	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
				my_strdel (ToneClose1, '*');
				my_strdel (ToneClose2, '*');
				my_strdel (ToneOpen1, '*');
				my_strdel (ToneOpen2, '*');

				if (Language[0] == LOCAL_LANGUAGE)
				{
					strcat ( (s8*) ToneClose1, "*");
					strcat ( (s8*) ToneClose2, "*");
				}
				else
				{
					strcat ( (s8*) ToneOpen1, "*");
					strcat ( (s8*) ToneOpen2, "*");
				}
			}
		}
		else
		{
			return MenuOp.RetVal;
		}
	}
}




static char DisplayCID (char *msg)
{
	unsigned char CmdBuff[50];
	unsigned char temp[50];
	unsigned char retval;

	clear_lcd();
	if(NetworkSwitch[0]==WiFi)
	{
		DrawTitle_to_row_Language2 (1, "当前网络为", "Current network is");
		DrawTitle_to_row_Language2 (2, "WiFi请切换", "WiFi,Please change to");
		DrawTitle_to_row_Language2 (3, "为GPRS后操作", "GPRS before operation");
		WaitkeyAndAutoExit (10);
	    return 0;
	}
		
	DrawTitle_to_row_Language2 (0, "SIM卡CID码", "SIM cid");

	retval = GetCCID (temp);
	if(retval == 0)
	{
		DrawTitle_to_row_Language2 (2, "读取失败", "Failed to read!");		
		WaitkeyAndAutoExit (10);
		return 0;
	}

	if (strlen ( (sc8*) temp) > 16)
	{
		memset (CmdBuff, 0, 30);
		memcpy (CmdBuff, temp, 16);
		DrawTitle_to_row (1, CmdBuff);
		DrawTitle_to_row (2, temp + 16);
	}
	else
	{
		DrawTitle_to_row (1, temp);
	}

	WaitkeyAndAutoExit (10);
	return 0;
}

static char Display_IMEI (char *msg)
{
	unsigned char buff[50];

	clear_lcd();
	if(NetworkSwitch[0]==WiFi)
	{
		DrawTitle_to_row_Language2 (1, "当前网络为", "Current network is");
		DrawTitle_to_row_Language2 (2, "WiFi请切换", "WiFi,Please change to");
		DrawTitle_to_row_Language2 (3, "为GPRS后操作", "GPRS before operation");
		WaitkeyAndAutoExit (10);
	    return 0;
	}
	DrawTitle_to_row (0, "IMEI");

	GetIMEI (buff);

	DrawTitle_to_row (2, buff);
	return (WaitkeyAndAutoExit (10) );
}

static char Display_ChipId (char *msg)
{
	unsigned char buff[50];

	clear_lcd();
	DrawTitle_to_row_Language2 (0, "芯片id", "Chip ID");

	kmy_MiscGetChipUniqueId (buff);

	DrawTitle_to_row (3, &buff[12]);
	buff[12] = 0;
	DrawTitle_to_row (2, &buff[0]);
	return (WaitkeyAndAutoExit (30) );
}


static char Display_RandomNumber (char *msg)
{
	unsigned char buff[21];

	clear_lcd();
	DrawTitle_to_row_Language2 (0, "随机数", "Random num");

	kmy_MiscGetRandomNumber (buff, sizeof (buff) - 1);

	DrawTitle_to_row (2, buff);
	return (WaitkeyAndAutoExit (30) );
}

static char Display_KMY_LibVersion (char *msg)
{
	unsigned char buff[21];

	clear_lcd();
	DrawTitle_to_row_Language2 (0, "KMY库版本", "KMY LibVersion");

	kmy_MiscGetLibraryVersion (buff);

	DrawTitle_to_row (2, buff);
	return (WaitkeyAndAutoExit (30) );
}

static char BasicApnSetApn (char *msg)
{
	unsigned char buff[sizeof (ApnApn)];
	unsigned char retval;
	
	clear_lcd();
	strcpy((s8*)buff,(sc8*)ApnApn);
	DrawTitle_to_row(0,"Apn is:");
	text_out_to_row(1,buff);
	text_out_to_row(3,"Change: Confirm");
	text_out_to_row(4,"Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	retval=GetString_abcABC123PYWuBi_Language2("输入Apn","Input Apn",SRF_abc|SRF_ABC|SRF_123,SRF_abc,buff,sizeof(buff),1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy ( (s8*) ApnApn, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (3, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "setting,pleas wait...");
		retval = set_apn();

		if (retval != TCP_OK)
		{
			DrawTitle_to_row (3, "Set APN error");
		}
		else
		{
			DrawTitle_to_row (3, "Set APN success");
		}
	}
	kmy_EnableDisableTime3(1);

	return (WaitkeyAndAutoExit (10) );
}
static char BasicApnSetUsername (char *msg)
{
	unsigned char buff[sizeof (ApnUsername)];
	unsigned char retval;
	
	clear_lcd();
	strcpy((s8*)buff,(sc8*)ApnUsername);
	DrawTitle_to_row(0,"ApnUsername is:");
	text_out_to_row(1,buff);
	text_out_to_row(3,"Change: Confirm");
	text_out_to_row(4,"Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	retval=GetString_abcABC123PYWuBi_Language2("输入Apn用户名","Input ApnUsername",SRF_abc|SRF_ABC|SRF_123,SRF_abc,buff,sizeof(buff),1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy ( (s8*) ApnUsername, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (3, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "setting,pleas wait...");
		retval = set_apn();

		if (retval != TCP_OK)
		{
			DrawTitle_to_row (3, "Set APN error");
		}
		else
		{
			DrawTitle_to_row (3, "Set APN success");
		}
	}
	
	kmy_EnableDisableTime3(1);
	return (WaitkeyAndAutoExit (10) );
}
static char BasicApnSetPassword (char *msg)
{
	unsigned char buff[sizeof (ApnPassword)];
	unsigned char retval;	

	clear_lcd();
	strcpy((s8*)buff,(sc8*)ApnPassword);
	DrawTitle_to_row(0,"ApnPassword is:");
	text_out_to_row(1,buff);
	text_out_to_row(3,"Change: Confirm");
	text_out_to_row(4,"Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	retval=GetString_abcABC123PYWuBi_Language2("I输入Apn密码","Input ApnPassword",SRF_abc|SRF_ABC|SRF_123,SRF_abc,buff,sizeof(buff),1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy ( (s8*) ApnPassword, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (3, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "setting,pleas wait...");
		retval = set_apn();

		if (retval != TCP_OK)
		{
			DrawTitle_to_row (3, "Set APN error");
		}
		else
		{
			DrawTitle_to_row (3, "Set APN success");
		}
	}
	kmy_EnableDisableTime3(1);
	return (WaitkeyAndAutoExit (10) );
}

char BasicApnSet(char *msg)
{
	static struct MenuFuc_Language2 Menu[]={
		{"1.APN","1.APN",BasicApnSetApn},
		{"2.APN用户名","2.User",BasicApnSetUsername},
		{"3.APN密码","3.Password",BasicApnSetPassword},
	};

	struct MenuOperate_Language2 MenuOp={
		Menu,
		sizeof(Menu) / sizeof(Menu[0]),
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				return KEY_ReturnInitInterface;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
			}
		}
		else
		{
			return MenuOp.RetVal;
		}
	}
}





char ReadSomeMessage (char *msg)
{
	static struct MenuFuc_Language2 Menu[]={
		{"1.版本信息","1.Version",DisplayVersion},
		{"2.SIM卡CID码","2.SIM card CID",DisplayCID},
		{"3.IMEI号","3.IMEI number",Display_IMEI},
		{"4.读芯片唯一ID","4.Chip unique ID",Display_ChipId},
		{"5.读随机数","5.Random number",Display_RandomNumber},
		{"6.读KMY库版本","6.KMY_LibVersion",Display_KMY_LibVersion},
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		sizeof(Menu) / sizeof(Menu[0]),
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};


	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
			}
		}
		else
		{
			CurProc = CurProc_Return;
			return MenuOp.RetVal;
		}
	}
}

unsigned char TranslateKeyToDecimal(unsigned char KeyValue)
{
    switch(KeyValue)
    {
        case KEY0:
            return 0;
         
        case KEY1:
            return 1;
         
        case KEY2:
            return 2;
         
        case KEY3:
            return 3;
         
        case KEY4:
            return 4;
         
        case KEY5:
            return 5;
         
        case KEY6:
            return 6;
         
        case KEY7:
            return 7;
         
        case KEY8:
            return 8;
         
        case KEY9:
            return 9;
         
        default:
            break;
    }
    return 0;
}

unsigned char SetBitOfDecimal(int OldDecimal, unsigned char ValueOfBit, unsigned char bit)
{
    unsigned char NewDecimal=0;
    if(bit == 0)
    {
        NewDecimal = OldDecimal%10;
        NewDecimal += ValueOfBit*10;
    }
    else if(bit == 1)
    {
        NewDecimal = (OldDecimal/10)*10;
        NewDecimal += ValueOfBit;
    }
    return NewDecimal;
}

unsigned char GetBitOfDecimal(unsigned char Decimal, unsigned char bit)
{
    if(bit == 0)
    {
        return ((Decimal/10)%10);
    }
    else if(bit == 1)
    {
        return ((Decimal/1)%10);
    }
    return 0;
}
 
 
 
static char TimeDateSet (char *msg) //1.time data setting
{
    unsigned char FlashFlag = 0x01 | 0x02 | 0x04;
    unsigned int year = 0;
    static unsigned char year_H=0;
    static unsigned char year_L=0;
    static unsigned char month = 0;
    static unsigned char day = 0;
    static unsigned char hour = 0;
    static unsigned char minute = 0;
    static unsigned char second = 0;
    unsigned char sBuff[100]={0};
     
    unsigned char TempKey = KEY_NONE;
    unsigned char Index=0;
    unsigned char MoveFlag=0;
    unsigned char ret=0;
    #define startx 7
    #define Row 2
    #define MaxArray 14
    char YearofMonth[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
     
    unsigned char CursorArray[MaxArray][4] = {
                                              //Min, x, y, Max
                                              {0, startx+6*0, (Row+1)*13-2, 9},
                                              {0, startx+6*1, (Row+1)*13-2, 9}, 
                                              {0, startx+6*2, (Row+1)*13-2, 9},
                                              {0, startx+6*3, (Row+1)*13-2, 9},
                                               
                                              {0, startx+6*5, (Row+1)*13-2, 9},
                                              {0, startx+6*6, (Row+1)*13-2, 9},
                                               
                                              {0, startx+6*8, (Row+1)*13-2, 9},
                                              {0, startx+6*9, (Row+1)*13-2, 9},
                                               
                                              {0, startx+6*11, (Row+1)*13-2, 9},
                                              {0, startx+6*12, (Row+1)*13-2, 9}, //4
                                               
                                              {0, startx+6*14, (Row+1)*13-2, 9},
                                              {0, startx+6*15, (Row+1)*13-2, 9},
                                               
                                              {0, startx+6*17, (Row+1)*13-2, 9},
                                              {0, startx+6*18, (Row+1)*13-2, 9},
                                             };
    typedef struct
    {
        unsigned char *DestValue;
        unsigned char Subtract;
    }DT;
     
    DT dt[] = {
               {&year_H, 0},
               {&year_H, 0},
               {&year_L, 2},
               {&year_L, 2},
                
               {&month, 4},
               {&month, 4},
                
               {&day, 6},
               {&day, 6},
                
               {&hour, 8},
               {&hour, 8},
                
               {&minute, 10},
               {&minute, 10},
                
               {&second, 12},
               {&second, 12},
              };
     
    kmy_TimeGetTime (&year, &month, &day, &hour, &minute, &second, NULL);
    year_H=year/100;
    year_L=year%100;
    clear_lcd();
    DrawTitle_to_row_Language2 (0, "Time Date", "Time Date");
    kmy_PostKey(KEY_pseudo);
    while (1)
    {
        TempKey = kmy_GetKeyValue();
         
        if (TempKey == KEY_NONE) continue;
         
        switch (TempKey)
        {
            case KEY_pseudo:
                if((FlashFlag & 0x01) == 0x01)
                {
                    kmy_LcdClear();
                    DrawTitle_to_row_Language2 (0, "Time Date", "Time Date");
                      
                    FlashFlag &= ~0x01;
                }
                if((FlashFlag & 0x02) == 0x02)
                {
                    memset(sBuff, 0, sizeof(sBuff));
                    sprintf((char *)sBuff, "%02d%02d-%02d-%02d %02d:%02d:%02d", year_H,year_L, month, day, hour, minute, second);
                    text_out_to_row_x(Row, startx, sBuff);
                     
                    FlashFlag &= ~0x02;
                }
                if((FlashFlag & 0x04) == 0x04)
                {
                    #define CURSOR_HEIGHT 5
                    clear_area(0, CursorArray[Index][2], 132, CursorArray[Index][2]+CURSOR_HEIGHT);
                     
                    draw_rect(CursorArray[Index][1], CursorArray[Index][2], 5, CURSOR_HEIGHT, mode_Black);
                     
                    FlashFlag &= ~0x04;
                }
                if(MoveFlag)
                {
                    kmy_PostKey (KEY_RIGHT);//move Index to next
                    MoveFlag = 0;
                }
                break;
            case KEY0:
            case KEY1:
            case KEY2:
            case KEY3:
            case KEY4:
            case KEY5:
            case KEY6:
            case KEY7:
            case KEY8:
            case KEY9:
                if((TranslateKeyToDecimal(TempKey) >= CursorArray[Index][0]) && (TranslateKeyToDecimal(TempKey) <= CursorArray[Index][3]))
                {
                    *dt[Index].DestValue = SetBitOfDecimal(*dt[Index].DestValue, TranslateKeyToDecimal(TempKey),Index-dt[Index].Subtract);
                    FlashFlag |= 0x02;
                    kmy_PostKey(KEY_pseudo);
                }
                MoveFlag = 1;
                break;
            case KEY_LEFT:
                BeepNormal();
                if ( (Index - 1) >= 0)
                {
                    Index -= 1;
                }
                else
                {
                    Index = MaxArray - 1;
                }
                FlashFlag |= 0x04;
                kmy_PostKey(KEY_pseudo);
                break;
             
            case KEY_RIGHT:
                BeepNormal();
                if ( (Index + 1) < MaxArray)
                {
                    Index += 1;
                }
                else
                {
                    Index = 0;
                }
                FlashFlag |= 0x04;
                kmy_PostKey(KEY_pseudo);
                break;
            case KEY_UP:
            case KEY_DOWN:
                BeepNormal();
                if(TempKey == KEY_UP)
                {
                    ret = GetBitOfDecimal(*dt[Index].DestValue, Index-dt[Index].Subtract);
                    if(ret +1 <= CursorArray[Index][3])
                    {
                        ret += 1;
                        *dt[Index].DestValue = SetBitOfDecimal(*dt[Index].DestValue, ret, Index-dt[Index].Subtract);
                    } 
                }
                else
                {
                    ret = GetBitOfDecimal(*dt[Index].DestValue, Index-dt[Index].Subtract);
                    if(ret -1 >= CursorArray[Index][0])
                    {
                        ret -= 1;
                        *dt[Index].DestValue = SetBitOfDecimal(*dt[Index].DestValue, ret, Index-dt[Index].Subtract);
                    } 
                }
                FlashFlag |= 0x02;
                kmy_PostKey(KEY_pseudo);
                break;
            case KEY_Enter:
                BeepNormal();
                year=year_H*100+year_L;
                 
                if(day>YearofMonth[month])
                {
                    if(month==2)
                    {
                        if(year%4==0&&year%100!=0||year%400==0)
                        {
                            if(day>29)month=0;
                        }
                        else
                        {
                            month=0;
                        }
                    }
                    else
                    {
                        month=0;
                    }
                }
                if(month==0||month>12||hour>24||minute>60||second>60)
                {
                    kmy_LcdClear();
                    DrawTitle_to_row(1,"Time date format");
                    DrawTitle_to_row(2," error");
                    WaitkeyAndAutoExit(2);
                }
                else
                {
                    
                    kmy_TimeSetTime (year, month, day, hour, minute, second);
                    msleep(500);
                }
             
                return KEY_Enter;
            case KEY_ReturnInitInterface:
            case KEY_ReturnPrevious:
                BeepNormal();
                return TempKey;
            default:break;
        }
    }
}

static char TimeZoneSet(char *msg)	
{
	unsigned char buff[sizeof(TimeZone)];
	unsigned char retval;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)TimeZone);
	DrawTitle_to_row(0,"Time Zone is:");
	text_out_to_row(1,buff);
	text_out_to_row(3,"Change: Confirm");
	text_out_to_row(4,"Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	retval=GetString_abcABC123PYWuBi_Language2("输入Time Zone","Input Time Zone",SRF_abc|SRF_ABC|SRF_123,SRF_123,buff,sizeof(buff),1);

	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	strcpy((s8*)TimeZone,(sc8*)buff);
	if(!StorageVariable())
	{
		DrawTitle_to_row(3,"Change falure!");
	}
	else
	{
		DrawTitle_to_row(3,"Change success");
	}

	return (WaitkeyAndAutoExit(10));

}


void ChangeNetwork(void)
{
	clear_lcd();
	DrawTitle_to_row_Language2(2,"更改网络","Change Network");
	DrawTitle_to_row_Language2(3,"请等候...","Please wait...");
	if(NetworkSwitch[0]==WiFi)
	{
	    send_at_cmd("AT+QPOWD=1\r");// power down GPRS module
		kmy_GprsOnOff(0);
    	kmy_WifiInit();
		printf("WiFi Mode\r\n");
	}
	else
	{
		kmy_WiFi_OnOff (0);
		kmy_GprsInit();
		msleep(1000);
		Sim900B_init(); 
		printf("GPRS Mode\r\n");
	}	
}


//////////////////////////////////////////////////////////////////////
// Newton:

typedef struct SettingItem {
	const unsigned char * itemName;
	unsigned short itemLen;
	unsigned char * content;
} SettingItem;

typedef struct StoreItem {
	unsigned char itemName[64];
	unsigned short itemLen;
	unsigned char * content;
} StoreItem;

SettingItem settingItems[] = {
	{"Network Selection(G>GPRS;W>WiFi)", 1, NetworkSwitch},
	{"WPA Passphrase(8-30 Characters)", 30, WLANWP},
	{"DHCP Client(0:Disable,1:Enable)", 1, dhcpclient},
	{"Local IP", 25, LocalIpPortBuf},
	{"Local Mask", 25, MaskBuff},
	{"Local Gateway", 25, GatewayBuff},

	{"Local DNS", 25, DNSBuff},
	{"GPRS APN",50, ApnApn},
	{"GPRS APN UserName",20, ApnUsername},
	{"GPRS APN Password", 20, ApnPassword},
	{"Web Server Domain/IP,Port", 50, Domain_IP},
	{"Version URL", 100, VersionURL},
	{"Upgrade URL", 100, UpgradeURL},
	{"Order Request Web Url", 150, RequestWebURL},
	{"Order Accept/Reject Callback Url", 150, CallbackURL},
	{"Order Reject Reason", 200, RejectReason},

	{"Restaurant ID for this machine", 10, ResID},
	{"Login Web Username", 20, LoginWebUsername},
	{"Login Web Password", 20, LoginWebPassword},
	{"Beep Duration", 2, BeepDuration},
	{"Beep Enable", 1, beepKG},
	{"Accept Time Mode", 1, AcceptTimeMode},
	{"Auto Check Order Interval", 3, CheckOrderInterval},
	{"Open Network Time", 8, NetOpenTime},
	{"Close Network Time", 8, NetCloseTime},
	//{"Print Date Format", 1, PrintDateFormat},
	//{"Print Time Format", 1, PrintTimeFormat},
	//{"Print Page Header", 30, PrintPageHeader},
	//{"Print Page Footer", 100, PrintPageFooter},
	//{"Print Receipt Count", 1, PrintReceiptCount},
	{"Auto Accept Order", 1, AutoAcceptOrder},
	{"Accept time options", 150, AcceptTimeOptions},
	{"Time Zone(eg.UTC+08:00)", 20, TimeZone},
};
unsigned char itemCnt = sizeof(settingItems) / sizeof(settingItems[0]);

unsigned char searchPosInItems(char *itemName)
{
	unsigned char i;
	
	for(i = 0; i < itemCnt; i++)
	{
		if(strcmp((char *)itemName, (char *)settingItems[i].itemName) == 0)
			break;
	}
	return i;		
}

unsigned short GetSum(unsigned char buff[], int len)
{
    unsigned short sum = 0, i = 0;
	
    for (i = 0; i < len; i++)
    {
        sum += buff[i];
    }
	
	return sum;
}

char * NT_itoa(int number, char *buffer, unsigned char base)
{
	if(buffer == NULL)	return NULL;
	
	if(base == 16)		sprintf(buffer, "%x", number);
	else if(base == 8)	sprintf(buffer, "%o", number);
	else 				sprintf(buffer, "%d", number);
	return buffer;
}



#define COLITEM_TAG	"#"
#define SUBITEM_TAG	"*"

char NT_StoreParam(char *cmdString)
{
	StoreItem tempItem;
	char *pfront, *pback, *ptemp;
	char buffer[128] = {0};
	unsigned char pos = 0;

	pfront = pback = cmdString;
	while( *pfront )
	{
		// Get setting name
		pfront = strstr(pback, SUBITEM_TAG);
		if(pfront == NULL)	return 2;
		
		ptemp = pfront++;
		*ptemp = '\0';
		strcpy((char *)tempItem.itemName, (char *)pback);
		pback = pfront;
		
		// Get setting len
		pfront = strstr(pback, SUBITEM_TAG);
		if(pfront == NULL)	return 3;	
		
		ptemp  = pfront++;
		*ptemp = '\0';
		strcpy(buffer, pback);
		tempItem.itemLen = atoi(buffer);
		pback =  pfront;
		memset(buffer, 0, sizeof(buffer)/sizeof(buffer[0]));
		
		// Get setting content
		pfront = strstr(pback, COLITEM_TAG);
		if(pfront == NULL)
			return 4;
		
		ptemp = pfront++;
		*ptemp = '\0';
		tempItem.content = (unsigned char *)malloc(tempItem.itemLen);
		if(tempItem.content == NULL)
		{
			kmy_USART1SendString("Malloc Failed\n");
			return 5;
		}
		strncpy((char *)tempItem.content, (char *)pback, tempItem.itemLen);
		pback = pfront;
		
		pos = searchPosInItems((char *)tempItem.itemName);
		if(pos  < itemCnt)
			strncpy((char *)settingItems[pos].content,(char *)tempItem.content,tempItem.itemLen);

		memset(buffer, 0, sizeof(buffer));

		strcpy(buffer, (char *)tempItem.itemName);
		strcat(buffer, " ");
		strcat(buffer, (char *)tempItem.content);
		strcat(buffer, "\n");
		free(tempItem.content);
	}

	return  !StorageVariable();
}

char * NT_MakeParameter(char sendBuff[])
{
	char * iterator = sendBuff + 2;		// 2 bytes data for  length
	unsigned char pos = 0;
	unsigned char convertBuf[32] = {0}; 
	unsigned short check_sum = 0;
	unsigned short length;
	
	// copy the current config to 
	for(pos = 0; pos < itemCnt; pos++)
	{	
		strcat(iterator, (char *)settingItems[pos].itemName);
		strcat(iterator, SUBITEM_TAG);
		strcat(iterator, (char *)NT_itoa(settingItems[pos].itemLen, (char *)convertBuf, 10));
		strcat(iterator, SUBITEM_TAG);
		strncat(iterator, (char *)settingItems[pos].content,settingItems[pos].itemLen);
		strcat(iterator, COLITEM_TAG);
	}
	
	// the checksum didn't contain length
	length = strlen(iterator);
	check_sum = GetSum((unsigned char*)iterator, length);
	iterator[length] = check_sum >> 8;
	iterator[length+1] = check_sum & 0x00ff;
	iterator[length+2] = 0;
	
	// insert the length into the begining of sendBuff
	sendBuff[0] = length >> 8;
	sendBuff[1] = length & 0x00ff;
	return sendBuff;
}

#define STATE_READ_CMD	0
#define STATE_ACK_CMD	1
#define STATE_READ_LEN	2
#define STATE_READ_DATA	3
#define STATE_UPLOAD	4
#define STATE_UPLOAD_ACK 5
#define STATE_CHECKSUM	6

#define CMD_DOWNLOAD	"DW"
#define CMD_UPLOAD		"UP"
#define ACK_OKSTR		"OK"


char UpdateParameter(char *msg)
{
	unsigned char RecvBuff[2000] = {0}, retval = 0;
	int RecvLen = 0;
	char State = STATE_READ_CMD;
	int ReadLen = 0;
	int ReadLen2 = 0;
	int ReadPos = 0;
	unsigned short CheckSum = 0, localSum = 0;
	unsigned char OldNetwork;

	unsigned char converter[32] = {0};
	
	OldNetwork = NetworkSwitch[0];
	kmy_EnableDisableTime3(0);

	clear_lcd();
	DrawTitle_to_row_Language2(0,"Updating...","Updating...");
	kmy_USART1ReceiveReset();
	
	while(1)
	{
		retval=GetKeyValue();
		if(retval != KEY_NONE)break;

		switch(State)
		{
			case STATE_READ_CMD://read AT
				RecvLen = 0;
				memset(RecvBuff, 0, sizeof(RecvBuff));
				
				RecvLen = kmy_USART1ReceiveData(RecvBuff, 2, 100);
				if(RecvLen == 2)
				{		
					if(strstr((char*)RecvBuff, CMD_UPLOAD))
						State = STATE_UPLOAD;
					
					else if( strstr( (char *)RecvBuff, CMD_DOWNLOAD))// upload parameters list.
						State = STATE_ACK_CMD;
				}
				break;

			case STATE_ACK_CMD://send OK
				kmy_USART1ReceiveReset();
				kmy_USART1SendString(ACK_OKSTR);
				State = STATE_READ_LEN;
				break;

			case STATE_READ_LEN://read 2 bytes of len
				RecvLen = 0;
				memset(RecvBuff, 0, sizeof(RecvBuff));
				
				RecvLen = kmy_USART1ReceiveData(RecvBuff, 2, 100);
				if(RecvLen == 2)
				{
					ReadLen = (RecvBuff[0]<<8) | RecvBuff[1];
					ReadLen2 = ReadLen;
					
					ReadPos = 0;
					ReadPos += 2;
					ReadLen += 2;
					State = STATE_READ_DATA;
				}
				break;

			case STATE_READ_DATA://read "ReadLen" bytes datas
				RecvLen = 0;
				RecvLen = kmy_USART1ReceiveData(RecvBuff + ReadPos, ReadLen, 100);
			
				ReadPos += RecvLen;
				ReadLen -= RecvLen;

				if(ReadLen == 0)	State = STATE_CHECKSUM;
				break;

			case STATE_UPLOAD:
				memset(RecvBuff, 0, sizeof(RecvBuff));
				NT_MakeParameter( (char *)RecvBuff);	
				kmy_USART1SendString( RecvBuff);
				State = STATE_UPLOAD_ACK;
				break;

			case STATE_UPLOAD_ACK:
				RecvLen = 0;
				memset(RecvBuff, 0, sizeof(RecvBuff));
				
				RecvLen = kmy_USART1ReceiveData(RecvBuff, 2, 100);
				if(RecvLen == 2)
				{
					if( strstr( (char *)RecvBuff, ACK_OKSTR))
					{
						State = STATE_READ_CMD;
						clear_lcd();
						DrawTitle_to_row_Language2(1, "Read Config Success", "Read Config Success");

						kmy_NetStateSet();
						WaitkeyAndAutoExit(10);
						kmy_EnableDisableTime3(1);
						return 0;
					}		
					else
					{
						clear_lcd();
						DrawTitle_to_row_Language2(1, "Read Config Failed", "Read Config Failed");
						WaitkeyAndAutoExit(10);
						kmy_EnableDisableTime3(1);
						return 1;
					}
					
				}
				break;
			
				
			case STATE_CHECKSUM://check parity, send ok or error
				//有效数据位置是从2开始到ReadLen2的数据
				CheckSum = (RecvBuff[ReadLen2+2] << 8) | RecvBuff[ReadLen2 + 3];
				localSum = GetSum(RecvBuff+2, ReadLen2);

				sprintf((char *)converter, "Len:%u Sum:%u", ReadLen2, CheckSum);
				text_out_to_row(2, converter);
				
				if(CheckSum == localSum)
				{
					kmy_USART1SendString("OK");
					RecvBuff[ReadLen2+2] = '\0';

					retval = NT_StoreParam((char*)RecvBuff+2);
					if(retval == 0)
					{
						clear_lcd();
						DrawTitle_to_row_Language2(1,"Update Success","Update Success");
						
						kmy_NetStateSet();
						WaitkeyAndAutoExit(10);
					}
					else 
					{
						clear_lcd();
						if(retval == 1)			DrawTitle_to_row(0,"Update Fail");
						else if(retval == 2)	DrawTitle_to_row(0,"ERROR 2");
						else if(retval == 3)	DrawTitle_to_row(0,"ERROR 3");
						else if(retval == 4)	DrawTitle_to_row(0,"ERROR 4");
						else 					DrawTitle_to_row(0,"Other Error");
						WaitkeyAndAutoExit(10);
					}
					if(OldNetwork != NetworkSwitch[0])ChangeNetwork();
					DEST_IP=0;
		            DEST_PORT=0;
					kmy_EnableDisableTime3(1);
					return 0;
				}
				else
				{	
					kmy_USART1SendString("ERROR");
					if(OldNetwork != NetworkSwitch[0])ChangeNetwork();
					kmy_EnableDisableTime3(1);
					msleep(5000);
					//M: return 0;
					return 0;
				}
		}
	}
	kmy_EnableDisableTime3(1);
	return 0;
}


static char NetworkSet_WiFi (char *msg)
{
	memset (NetworkSwitch, 0, sizeof (NetworkSwitch) );
	NetworkSwitch[0] = WiFi;

	if (!StorageVariable() )
	{
		DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		WaitkeyAndAutoExit (10);
	}
	return 0;
}
static char NetworkSet_GPRS (char *msg)
{
	memset (NetworkSwitch, 0, sizeof (NetworkSwitch) );
	NetworkSwitch[0] = GPRS;

	if (!StorageVariable() )
	{
		DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		WaitkeyAndAutoExit (10);
	}
	return 0;
}
static char NetworkSet (char *msg)	//key tone setting
{
	static unsigned char ToneClose1[20] = {"1.GPRS   "};
	static unsigned char ToneClose2[20] = {"1.GPRS   "};
	static unsigned char ToneOpen1[20] = {"2.WiFi   "};
	static unsigned char ToneOpen2[20] = {"2.WiFi   "};
	unsigned char OldNetwork;

	static struct MenuFuc_Language2 Menu[] =
	{
		{ToneClose1, ToneClose2, NetworkSet_GPRS},
		{ToneOpen1, ToneOpen2, NetworkSet_WiFi}
	};
	struct MenuOperate_Language2 MenuOp =
	{
		(struct MenuFuc_Language2*) Menu,
		sizeof(Menu) / sizeof(Menu[0]),
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	kmy_EnableDisableTime3(0);
    OldNetwork = NetworkSwitch[0];
	
	my_strdel (ToneClose1, '*');
	my_strdel (ToneClose2, '*');
	my_strdel (ToneOpen1, '*');
	my_strdel (ToneOpen2, '*');

	if (NetworkSwitch[0] == GPRS)
	{
		strcat ( (s8*) ToneClose1, "*");
		strcat ( (s8*) ToneClose2, "*");
	}
	else
	{
		strcat ( (s8*) ToneOpen1, "*");
		strcat ( (s8*) ToneOpen2, "*");
	}

	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				if(OldNetwork != NetworkSwitch[0])ChangeNetwork();
				kmy_EnableDisableTime3(1);
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
				my_strdel (ToneClose1, '*');
				my_strdel (ToneClose2, '*');
				my_strdel (ToneOpen1, '*');
				my_strdel (ToneOpen2, '*');

				if (NetworkSwitch[0] == GPRS)
				{
					strcat ( (s8*) ToneClose1, "*");
					strcat ( (s8*) ToneClose2, "*");
				}
				else
				{
					strcat ( (s8*) ToneOpen1, "*");
					strcat ( (s8*) ToneOpen2, "*");
				}
			}
		}
		else
		{
			if(OldNetwork != NetworkSwitch[0])ChangeNetwork();			
			kmy_EnableDisableTime3(1);
			return MenuOp.RetVal;
		}
	}
}


char BasicSettings (char *msg)
{
	static struct MenuFuc_Language2 Menu[] =
	{
		{"1.屏幕对比度设置", "1.Contrast", ContrastSet},			
		{"2.打印黑度设置","2.Print density set",PrintDensitySet},			
		{"3.按键音设置", "3.Keytone", BasicKeyTone},
		{"4.时间日期设置", "4.DateTime", TimeDateSet},
		{"5.时区设置","5.Time Zone set",TimeZoneSet},		
		{"6.屏保时间设置", "6.Timeout", PowerOffScreenTime},
		{"7.语言设置", "7.Language", LanguageSet},
		{"8.读取相关信息", "8.Version Info", ReadSomeMessage},
#ifdef HaveWiFiModule			
		{"9.网络设置", "9.Network Settings", NetworkSet},
#endif
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		sizeof(Menu) / sizeof(Menu[0]),
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
			}
		}
		else
		{
			CurProc = CurProc_Return;
			return MenuOp.RetVal;
		}
	}
}

char TerminalIDSet (char *msg) //
{
	unsigned char buff[24];
	unsigned char retval;

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) TerminalID);
	DrawTitle_to_row_Language2 (0, "【当前终端ID】", "Current ID");
	text_out_to_row_Language2 (1, buff, buff);
	text_out_to_row_Language2 (3, "更改:请按确认", "Change: Confirm");
	text_out_to_row_Language2 (4, "退出:请按取消", "Exit: Return");

	retval = WaitkeyAndAutoExit (10);

	if (retval != KEY_Enter) return retval;

	retval = GetString_abcABC123PYWuBi_Language2 ("【输入终端ID】", "TerminalID edit", SRF_123 | SRF_ABC | SRF_abc, SRF_123, buff, sizeof (buff) - 1, 1);

	if (retval != KEY_Enter) return retval;

	clear_lcd();

	if (strlen ( (sc8*) buff) == 0)
	{
		DrawTitle_to_row_Language2 (2, "长度不能为0", "Length error!");
	}
	else
	{
		memset (TerminalID, 0, sizeof (TerminalID) );
		strcpy ( (s8*) TerminalID, (sc8*) buff);

		if (!StorageVariable() )
		{
			DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2 (3, "更改成功!", "change success!");
		}
	}

	return (WaitkeyAndAutoExit (10) );
}

char TerminalPasswordSet (char *msg) //
{
	unsigned char buff[24];
	unsigned char retval;

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) TerminalPassword);
	DrawTitle_to_row_Language2 (0, "【当前终端密码】", "TerminalPassword");
	text_out_to_row_Language2 (1, buff, buff);
	text_out_to_row_Language2 (3, "更改:请按确认", "Change: Confirm");
	text_out_to_row_Language2 (4, "退出:请按取消", "Exit: Return");

	retval = WaitkeyAndAutoExit (10);

	if (retval != KEY_Enter) return retval;

	retval = GetString_abcABC123PYWuBi_Language2 ("【输入终端密码】", "TerminalPassword", SRF_123 | SRF_ABC | SRF_abc, SRF_123, buff, sizeof (buff) - 1, 1);

	if (retval != KEY_Enter) return retval;

	clear_lcd();

	if (strlen ( (sc8*) buff) == 0)
	{
		DrawTitle_to_row_Language2 (2, "长度不能为0", "Length error!");
	}
	else
	{
		memset (TerminalPassword, 0, sizeof (TerminalPassword) );
		strcpy ( (s8*) TerminalPassword, (sc8*) buff);

		if (!StorageVariable() )
		{
			DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2 (3, "更改成功!", "change success!");
		}
	}

	return (WaitkeyAndAutoExit (10) );
}


static char IpAddSet (char *msg) //IP地址设置
{
	unsigned char buff[51];
	unsigned char retval;
	
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) Domain_IP);

	abcABC123PYWuBiArgDeInit (&argAa1, buff, 50);
	argAa1.srfFlag = SRF_123 | SRF_ABC | SRF_abc;
	argAa1.LocalTitle = "【输入Domain/IP,port】";
	argAa1.OtherTitle="Input Domain/IP, port";
	argAa1.HaveoldFlag = 1;
	retval = GetString_abcABC123PYWuBi2 (&argAa1);

	if (retval != KEY_Enter) return retval;
	kmy_EnableDisableTime3(0);

	clear_lcd();

	if (strlen ( (sc8*) buff) > 50 || strlen ( (sc8*) buff) < 9)
	{
		DrawTitle_to_row_Language2 (2, "错误:地址长度不对!", "Error:length wrong!");
	}
	else
	{
		memset (Domain_IP, 0, sizeof (Domain_IP) );
		strcpy ( (s8*) Domain_IP, (sc8*) buff);
		DEST_IP=0;
		DEST_PORT=0;

		if (!StorageVariable() )
		{
			DrawTitle_to_row_Language2 (2, "更改失败!", "change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2 (2, "更改成功!", "change success!");
		}
	}

	kmy_EnableDisableTime3(1);
	return (WaitkeyAndAutoExit (10) );
}



static char PIN_OpenClose (char *msg)
{
#if 0
	char retval, buff[1024];
	char temp[20];
	char PinTimes[2], PukTimes[3];

loop:
	retval = ReadPINPUKTimes (PinTimes, PukTimes);

	if (retval == 0)
	{
		if (PinTimes[0] > '0')
		{
			clear_lcd();
			DrawTitle_to_row_Language2 (0, "输入PIN", "Input PIN");

			if (Language[0] == '1')
			{
				strcpy (buff, "remain times:");
			}
			else
			{
				strcpy (buff, "剩余次数:");
			}

			strcat (buff, PinTimes);
			DrawTitle_to_row_Language2 (1, buff, buff);
			retval = PassWordInput2 (NULL, temp, 5, 1, 0, KEY_NONE, 2);

			if (retval != KEY_Enter) return 0;

			if (strlen (temp) != 4)
			{
				clear_lcd();
				DrawTitle_to_row_Language2 (1, "长度错误", "length error");
				sleep (2);
				return 0;
			}

			strcpy (buff, "AT+CLCK=\"SC\",");			//AT+CLCK="SC",1,"0907"

			if (RequirePin == NO)
			{
				strcat (buff, "1");
			}
			else
			{
				strcat (buff, "0");
			}

			strcat (buff, ",\"");
			strcat (buff, temp);
			strcat (buff, "\"\r");

			if (send_at_cmd_and_receive (buff, sizeof (buff), 2000, "OK\r") != TCP_OK)
			{
				clear_lcd();
				DrawTitle_to_row_Language2 (2, "PIN 错误", "PIN error");
				sleep (2);
				goto loop;
			}
			else
			{
				clear_lcd();

				if (RequirePin == 0)
				{
					DrawTitle_to_row_Language2 (2, "启用pin成功", "Enble PIN success");
					RequirePin = 1;
				}
				else
				{
					DrawTitle_to_row_Language2 (2, "禁用pin成功", "Disable PIN success");
					RequirePin = 0;
				}

				sleep (2);
				return 0;
			}
		}
		else
		{
			clear_lcd();
			DrawTitle_to_row_Language2 (2, "PIN 被锁", "PIN locked");
			sleep (2);
			PUK_Required();
			return 0;
		}
	}

	return 0;
#else
	return 0;
#endif
}
char PIN_Store (char *msg) //AT+CPWD="SC","0909","0505"
{
	unsigned char ret;
	unsigned char pin[7];

	clear_lcd();
	strcpy ( (s8*) pin, (sc8*) StorePIN);
	DrawTitle_to_row_Language2 (0, "PIN", "PIN");
	text_out_to_row_Language2 (1, "****", "****");
	text_out_to_row_Language2 (3, "修改:请按确认键", "change:confirm");
	text_out_to_row_Language2 (4, "返回:请按返回键", "exit:return");

	ret = WaitkeyAndAutoExit (10);

	if (ret != KEY_Enter) return ret;

	if (Language[0] == '1')
	{
		ret = PassWordInput2 ("Input PIN", pin, 6, 1, 1, KEY_NONE, 1);
	}
	else
	{
		ret = PassWordInput2 ("【输入PIN】", pin, 6, 1, 1, KEY_NONE, 1);
	}

	if (ret != KEY_Enter) return ret;

	if (strlen ( (sc8*) pin) != 4)
	{
		clear_lcd();
		DrawTitle_to_row_Language2 (2, "长度错误", "longth error");
		return (WaitkeyAndAutoExit (10) );
	}

	clear_lcd();
	strcpy ( (s8*) StorePIN, (sc8*) pin);

	if (!StorageVariable() )
	{
		DrawTitle_to_row_Language2 (3, "修改失败", "Change falure!");
	}
	else
	{
		DrawTitle_to_row_Language2 (3, "修改成功", "Change success!");
	}

	return (WaitkeyAndAutoExit (10) );
}
static char PIN_Change (char *msg) //AT+CPWD="SC","0909","0505"
{
#if 0
	PasswordInputArgument arg;
	unsigned char oldpassword[5];
	unsigned char newpassword1[5];
	unsigned char newpassword2[200];
	unsigned char retval;

	if (RequirePin == YES)  	//启用pin  之后才能改pin
	{
		PassWordArgDeInit (&arg, oldpassword, 4);
		arg.LocalTitle = "【输入原PIN】";
		retval = BT_PassWordInput_MiddledDisplay (&arg);

		if (retval != KEY_Enter) return retval;

again:
		PassWordArgDeInit (&arg, newpassword1, 4);
		arg.LocalTitle = "【输入新PIN】";
		retval = BT_PassWordInput_MiddledDisplay (&arg);

		if (retval != KEY_Enter) return retval;

		PassWordArgDeInit (&arg, newpassword2, 4);
		arg.LocalTitle = "【再次输入新PIN】";
		retval = BT_PassWordInput_MiddledDisplay (&arg);

		if (retval != KEY_Enter) return retval;

		if (strcmp (newpassword1, newpassword2) != 0)
		{
			clear_lcd();
			DrawTitle_to_row (1, "你两次输入的");
			DrawTitle_to_row (2, "新PIN不一致");
			DrawTitle_to_row (3, "请重新输入");
			retval = WaitkeyAndAutoExit (10);

			if (retval == KEY_Enter) goto again;
			else
			{
				return retval;
			}
		}

		//AT+CPWD="SC","0909","0505"
		strcpy (newpassword2, "AT+CPWD=\"SC\",\"");
		strcat (newpassword2, oldpassword);
		strcat (newpassword2, "\",\"");
		strcat (newpassword2, newpassword1);
		strcat (newpassword2, "\"\r");
		retval = send_at_cmd_and_return (newpassword2, 5000, "OK", "ERROR");

		clear_lcd();

		if (retval == TCP_OK)
		{
			DrawTitle_to_row (2, "修改成功");
		}
		else
		{
			DrawTitle_to_row (2, "修改失败");
		}

		return WaitkeyAndAutoExit (10);
	}
	else
	{
		clear_lcd();
		DrawTitle_to_row (2, "请先启用PIN");
		return WaitkeyAndAutoExit (10);
	}

	return 0;
#else
	return 0;
#endif
}

char PinSet (char *msg)
{
#if 0
	return (abeyant() );
#else
	static unsigned char BuffLocal[30], BuffOther[30];


	static struct MenuFuc_Language2 Menu[]={
		{BuffLocal,BuffOther,PIN_OpenClose},
		{"2.修改 PIN","2.Change PIN",PIN_Change},
		{"3.存储 PIN","3.Store PIN",PIN_Store},
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		sizeof(Menu) / sizeof(Menu[0]),
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	while (1)
	{
		if (RequirePin == 0)
		{
			strcpy ( (s8*) BuffLocal, "1.禁用 PIN");
			strcpy ( (s8*) BuffOther, "1.Close PIN");
		}
		else
		{
			strcpy ( (s8*) BuffLocal, "1.启用 PIN");
			strcpy ( (s8*) BuffOther, "1.Open PIN");
		}

		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
			}
		}
		else
		{
			return MenuOp.RetVal;
		}
	}

#endif
}

static char RestoreChuChangSet (char *msg)
{
	unsigned char retval;
	unsigned char OldNetwork=NetworkSwitch[0];

	clear_lcd();
	DrawTitle_to_row_Language2 (0, "特别警告:", "Warning:");
	DrawTitle_to_row_Language2 (1, "恢复出厂设置将导致,", "Are you sure");
	DrawTitle_to_row_Language2 (2, "此终端机成为一新机", "reset to the");
	DrawTitle_to_row_Language2 (3, "确定恢复:请按确认", "factory settings?");
	DrawTitle_to_row_Language2 (4, "退出:请按任意键", NULL);

	retval = WaitkeyAndAutoExit (20);

	if (retval != KEY_Enter) return retval;
	InitOrder();
	retval = RestoreConfigFile();
	clear_lcd();

	if (retval == 1)
	{
		if(OldNetwork != NetworkSwitch[0])ChangeNetwork();
		clear_lcd();
		DrawTitle_to_row_Language2 (2, "出厂设置恢复成功!", "Reset success");
	}
	else
	{
		DrawTitle_to_row_Language2 (2, "出厂设置恢复失败!", "Reset error");
	}

	return (WaitkeyAndAutoExit (10) );
}



static char OpPsword (char *msg) //终端操作密码设置，最多20  位密码
{
	unsigned char buff[16], buff2[16];
	char retval;
	PasswordInputArgument arg;

	PassWordArgDeInit (&arg, buff, sizeof (buff) );
	arg.LocalTitle = "【输入新密码】";
	arg.OtherTitle = "Input new password";
	retval = PassWordInput_MiddledDisplay (&arg);

	if (retval != KEY_Enter) return retval;

	PassWordArgDeInit (&arg, buff2, sizeof (buff2) );
	arg.LocalTitle = "【再次确认新密码】";
	arg.OtherTitle = "Again Input";
	retval = PassWordInput_MiddledDisplay (&arg);

	if (retval != KEY_Enter) return retval;

	clear_lcd();

	if (strcmp ( (sc8*) buff, (sc8*) buff2) == 0)
	{
		memset (EnterAdvancedSetPassWord, 0, sizeof (EnterAdvancedSetPassWord) );
		strcpy ( (s8*) EnterAdvancedSetPassWord, (sc8*) buff);

		if (!StorageVariable() )
		{
			DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2 (3, "更改成功!", "change success!");
		}
	}
	else
	{
		DrawTitle_to_row_Language2 (2, "错误!你两次输入", "Error,Two times");
		DrawTitle_to_row_Language2 (3, "的密码不一致!", "Input not same");
	}

	return (WaitkeyAndAutoExit (10) );
}

char Console (char *msg)
{
	return 0;
}

static char FormatEEPROM (char *msg)
{
	unsigned char retval;

	clear_lcd();
	text_out_to_row_Language2 (1, "格式化EEPROM,将导致", "Format EEPROM will");
	text_out_to_row_Language2 (2, "EEPROM中的所有数据都", "Lose all EEPROM data");
	text_out_to_row_Language2 (3, "丢失,确认请按confirm", "Can you sure Format");
	text_out_to_row_Language2 (4, "键", "it?");
	retval = WaitkeyAndAutoExit (10);

	if (retval != KEY_Enter) return retval;

	clear_lcd();
	DrawTitle_to_row_Language2 (2, "正在格式化", "Formating");
	EepromFile_FileSystemFormat();
	DrawTitle_to_row_Language2 (3, "格式化完毕", "Format Complete");
	retval = WaitkeyAndAutoExit (10);

	return retval;
}

char VersionURL_Set(char *msg)
{
	unsigned char buff[sizeof (VersionURL)];
	unsigned char retval=0;
	
	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) VersionURL);
	
	retval = GetString_abcABC123PYWuBi_Language2 ("Input Version URL", "Input Version URL", SRF_abc | SRF_ABC | SRF_123, SRF_abc, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;

	kmy_EnableDisableTime3(0);
	clear_lcd();
	strcpy ( (s8*) VersionURL, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	
	kmy_EnableDisableTime3(1);	
	return WaitkeyAndAutoExit(10);
}

char UpgradeURL_Set(char *msg)
{
	unsigned char buff[sizeof (UpgradeURL)];
	unsigned char retval=0;
	
	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) UpgradeURL);
	
	retval = GetString_abcABC123PYWuBi_Language2 ("Input Upgrade URL", "Input Upgrade URL", SRF_abc | SRF_ABC | SRF_123, SRF_abc, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy ( (s8*) UpgradeURL, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	
	kmy_EnableDisableTime3(1);
	
	return WaitkeyAndAutoExit(10);
}

char OrderURL_set(char *msg)
{
	unsigned char buff[sizeof (RequestWebURL)];
	unsigned char retval=0;
	
	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) RequestWebURL);
	
	retval = GetString_abcABC123PYWuBi_Language2 ("Input Web URL", "Input Web URL", SRF_abc | SRF_ABC | SRF_123, SRF_abc, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy ( (s8*) RequestWebURL, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	
	kmy_EnableDisableTime3(1);
	return WaitkeyAndAutoExit(10);
}
char Callback_set(char *msg)
{
	unsigned char buff[sizeof (CallbackURL)];
	unsigned char retval=0;
	
	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) CallbackURL);
	
	retval = GetString_abcABC123PYWuBi_Language2 ("Input Callback URL", "Input Callback URL", SRF_abc | SRF_ABC | SRF_123, SRF_abc, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);
	
	clear_lcd();
	strcpy ( (s8*) CallbackURL, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	
	kmy_EnableDisableTime3(1);
	return WaitkeyAndAutoExit(10);

}
char RejectReason_set(char *msg)
{
	char retval=0;
	
	kmy_EnableDisableTime3(0);
	
	retval=GetRejectReasonAndInput();
	if(retval==KEY_Enter)
	{
		if (!StorageVariable() )
		{
			clear_lcd();
			DrawTitle_to_row (2, "Change falure!");
		}
		else
		{	
			clear_lcd();
			DrawTitle_to_row (2, "Change success!");
		}
		
		kmy_EnableDisableTime3(1);
		return WaitkeyAndAutoExit(10);
	}
	
	kmy_EnableDisableTime3(1);
	return retval;
}
char RestaurantID_set(char *msg)
{
	unsigned char buff[sizeof (ResID)];
	unsigned char retval;

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) ResID);
	
	retval = GetString_abcABC123PYWuBi_Language2 ("Input Res ID ", "Input Res ID ", SRF_abc | SRF_ABC | SRF_123, SRF_abc, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy ( (s8*) ResID, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	
	kmy_EnableDisableTime3(1);
	return WaitkeyAndAutoExit(10);

}

char LoginUsername_set(char *msg)
{
	unsigned char buff[sizeof (LoginWebUsername)];
	unsigned char retval;
	
	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) LoginWebUsername);
	
	retval = GetString_abcABC123PYWuBi_Language2 ("Input Web Username", "Input Web Username", SRF_abc | SRF_ABC | SRF_123, SRF_abc, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy ( (s8*) LoginWebUsername, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}

	
	kmy_EnableDisableTime3(1);
	return WaitkeyAndAutoExit(10);

}

char LoginPassword_set(char *msg)
{
	unsigned char buff[sizeof (LoginWebPassword)];
	unsigned char retval;	

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) LoginWebPassword);

	retval = GetString_abcABC123PYWuBi_Language2 ("Input Web Password ", "Input Web Password ", SRF_abc | SRF_ABC | SRF_123, SRF_123, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy ( (s8*) LoginWebPassword, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	
	kmy_EnableDisableTime3(1);
	return WaitkeyAndAutoExit(10);

}

char CheckOrderInterval_set(char *msg)
{
	unsigned char buff[sizeof (CheckOrderInterval)];
	unsigned char retval;

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) CheckOrderInterval);

	retval = GetString_abcABC123PYWuBi_Language2 ("Interval (seconds)", "Interval (seconds)",  SRF_123, SRF_123, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy ( (s8*) CheckOrderInterval, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	
	kmy_EnableDisableTime3(1);
	return WaitkeyAndAutoExit(10);
}
char NetOpenTime_set(char *msg)
{
	char buff[sizeof (NetOpenTime)];
	unsigned char retval;

	clear_lcd();
	strcpy ( buff, (sc8*) NetOpenTime);
	
	retval = GetString_123TimeInput ("Input Open Net Time",SRF_123,SRF_123,(u8*)buff,6,1);
	if(retval != KEY_Enter)return retval;
	
	if(CheckTimeFormat(buff)==1)
	{
		clear_lcd();
		DrawTitle_to_row (2, "Incorrect time format");
		return WaitkeyAndAutoExit(10);
	}
	
	kmy_EnableDisableTime3(0);
	
	clear_lcd();
	strcpy ( (s8*) NetOpenTime, buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{	
		kmy_NetStateSet();
		DrawTitle_to_row (2, "Change success!");
	}
	
	kmy_EnableDisableTime3(1);
	return WaitkeyAndAutoExit(10);	
}
char NetCloseTime_set(char *msg)
{
	char buff[sizeof (NetCloseTime)];
	unsigned char retval;
	
	clear_lcd();
	strcpy ( buff, (sc8*) NetCloseTime);

	retval = GetString_123TimeInput ("Input Close Net Time",SRF_123,SRF_123,(u8*)buff,6,1);
	if(retval != KEY_Enter)return retval;
	
	if(CheckTimeFormat(buff)==1)
	{
		clear_lcd();
		DrawTitle_to_row (2, "Incorrect time format");
		return WaitkeyAndAutoExit(10);
	}
	
	kmy_EnableDisableTime3(0);
	
	clear_lcd();
	strcpy ( (s8*) NetCloseTime, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		kmy_NetStateSet();
		DrawTitle_to_row (2, "Change success!");
	}
	
	kmy_EnableDisableTime3(1);
	return WaitkeyAndAutoExit(10);
}
char BeepDuration_set(char *msg)
{
	unsigned char buff[sizeof (BeepDuration)];
	unsigned char retval;

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) BeepDuration);

	retval = GetString_abcABC123PYWuBi_Language2 ("Input Duration", "Input Duration", SRF_123, SRF_123, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;

	clear_lcd();
	strcpy ( (s8*) BeepDuration, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	return WaitkeyAndAutoExit(10);

}

static char Network_Close (char *msg)
{
	Net_ONOFF=1;
	clear_lcd();
	DrawTitle_to_row_Language2 (2, "Network OFF", "Network OFF");
	printf("Network OFF\r\n");
	return WaitkeyAndAutoExit (8);	
}
static char Network_Open (char *msg)
{
	Net_ONOFF=0;
	clear_lcd();
	DrawTitle_to_row_Language2 (2, "Network ON", "Network ON");	
	printf("Network ON\r\n");
	return WaitkeyAndAutoExit (8);	
}

char NetworkONOFF_set(char *msg)
{
	 static unsigned char NetworkClose1[20] = {"1.关闭  "};
	 static unsigned char NetworkClose2[20] = {"1.OFF  "};
	 static unsigned char NetworkOpen1[20] = {"2.打开   "};
	 static unsigned char NetworkOpen2[20] = {"2.ON  "};

	static struct MenuFuc_Language2 Menu[2] =
	{
		{NetworkClose1, NetworkClose2, Network_Close},
		{NetworkOpen1, NetworkOpen2, Network_Open}
	};
	struct MenuOperate_Language2 MenuOp =
	{
		(struct MenuFuc_Language2*) Menu,
		2,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	my_strdel (NetworkClose1, '*');
	my_strdel (NetworkClose2, '*');
	my_strdel (NetworkOpen1, '*');
	my_strdel (NetworkOpen2, '*');

	if (Net_ONOFF== 1)
	{
		strcat ( (char*)NetworkClose1, "*");
		strcat ( (char*)NetworkClose2, "*");
	}
	else
	{
		strcat ( (char*)NetworkOpen1, "*");
		strcat ( (char*)NetworkOpen2, "*");
	}

	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
				my_strdel (NetworkClose1, '*');
				my_strdel (NetworkClose2, '*');
				my_strdel (NetworkOpen1, '*');
				my_strdel (NetworkOpen2, '*');
			
				if (Net_ONOFF== 1)			
				{
					strcat ( (char*)NetworkClose1, "*");
					strcat ( (char*)NetworkClose2, "*");
				}
				else
				{
					strcat ( (char*)NetworkOpen1, "*");
					strcat ( (char*)NetworkOpen2, "*");
				}
			}
		}
		else
		{
			return MenuOp.RetVal;
		}
	}
}

char AcceptTimeMode_set(char *msg)
{
	unsigned char buff[sizeof (AcceptTimeMode)];
	unsigned char retval;

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) AcceptTimeMode);

	retval = GetString_abcABC123PYWuBi_Language2 ("Input Duration", "Input Duration", SRF_123, SRF_123, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;

	clear_lcd();
	strcpy ( (s8*) AcceptTimeMode, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	return WaitkeyAndAutoExit(10);

}

char PrintNewlineFlag_set(char *msg)
{
	clear_lcd();
	DrawTitle_to_row(1,"This function reserve");
	return WaitkeyAndAutoExit(8);

}
char PrintDateFormat_set(char *msg)
{
	clear_lcd();
	DrawTitle_to_row(1,"This function reserve");
	return WaitkeyAndAutoExit(8);
}
char PrintTimeFormat_set(char *msg)
{
	clear_lcd();
	DrawTitle_to_row(1,"This function reserve");
	return WaitkeyAndAutoExit(8);
}
char PrintPageHeader_set(char *msg)
{
	unsigned char buff[sizeof (PrintPageHeader)];
	unsigned char retval=0;

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) PrintPageHeader);
	
	retval = GetString_abcABC123PYWuBi_Language2 ("Input PrintPageHeader", "Input PrintPageHeader", SRF_abc | SRF_ABC | SRF_123, SRF_abc, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;

	clear_lcd();
	strcpy ( (s8*) PrintPageHeader, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	return WaitkeyAndAutoExit(10);
}

char PrintPageFooter_set(char *msg)
{
	unsigned char buff[sizeof (PrintPageFooter)];
	unsigned char retval=0;

	clear_lcd();
	strcpy ( (s8*) buff, (sc8*) PrintPageFooter);
	
	retval = GetString_abcABC123PYWuBi_Language2 ("Input PrintPageFooter", "Input PrintPageFooter", SRF_abc | SRF_ABC | SRF_123, SRF_abc, buff, sizeof (buff), 1);

	if (retval != KEY_Enter) return retval;

	clear_lcd();
	strcpy ( (s8*) PrintPageFooter, (sc8*) buff);

	if (!StorageVariable() )
	{
		DrawTitle_to_row (2, "Change falure!");
	}
	else
	{
		DrawTitle_to_row (2, "Change success!");
	}
	return WaitkeyAndAutoExit(10);
}


char PrintReceptCount_set(char *msg)
{
	clear_lcd();
	DrawTitle_to_row(1,"This function reserve");
	return WaitkeyAndAutoExit(8);
}

char AutoAccpetOrder_set(char *msg)
{
	clear_lcd();
	DrawTitle_to_row(1,"This function reserve");
	return WaitkeyAndAutoExit(8);
}

char LCDIdelIest_set(char *msg)
{
	clear_lcd();
	DrawTitle_to_row(1,"This function reserve");
	return WaitkeyAndAutoExit(8);
}

char InitParameterSet(char *msg)
{
	const static struct MenuFuc_Language2 Menu[22] =
	{
		{"1.Update Paras", "1.Update Paras", UpdateParameter},
		{"2.APN", "2.APN", BasicApnSetApn},
		{"3.APN username", "3.APN username", BasicApnSetUsername},
		{"4.APN password", "4.APN password", BasicApnSetPassword},
		{"5.Domain/IP,port", "5.Domain/IP,port", IpAddSet},			
		{"6.Version URL", "6.Version URL", VersionURL_Set},
		{"7.Upgrade URL", "7.Upgrade URL", UpgradeURL_Set},
		{"8.Order URL", "8.Order URL", OrderURL_set},
		{"9.Callback URL", "9.Callback URL", Callback_set},		
		{"10.Res ID", "10.Res ID", RestaurantID_set},
		{"11.Login username", "11.Login username", LoginUsername_set},
		{"12.Login password", "12.Login password", LoginPassword_set},
		{"13.Beep enable", "13.Beep enable", BasicKeyTone},
		{"14.Check interval", "14.Check interval", CheckOrderInterval_set},
		{"15.Open Net time", "15.Open Net time", NetOpenTime_set},
		{"16.Close Net time", "16.Close Net time", NetCloseTime_set},
		{"17.Beep duration", "17.Beep duration", BeepDuration_set},	
		{"18.Reject Reason", "18.Reject Reason", RejectReason_set},
		{"19.Print page header", "19.Print page header", PrintPageHeader_set},
		{"20.Print page footer", "20.Print page footer", PrintPageFooter_set},
		{"21.Accept time mode","21.Accept time mode",AcceptTimeMode_set},
		{"22.Network ON/OFF", "22.Network ON/OFF", NetworkONOFF_set},
		/*{"16.Connect Mode", "13.Connect Mode", ConnectMode_set},
		{"17.newline flag", "17.newline flag", PrintNewlineFlag_set},
		{"18.Print date format", "18.Print date format", PrintDateFormat_set},
		{"19.Print time format", "19.Print time format", PrintTimeFormat_set},
		{"20.Print page header", "20.Print page header", PrintPageHeader_set},
		{"21.Print page footer", "21.Print page footer", PrintPageFooter_set},
		{"22.Recept count", "22.Recept count", PrintReceptCount_set},
		{"23.Auto accpet order", "23.Auto accpet order", AutoAccpetOrder_set},
		{"24.LCD idel test", "24.LCD idel test", LCDIdelIest_set},
		{"25.Test text", "25.Test text", TestText_set},*/
	};
	struct MenuOperate_Language2 MenuOp =
	{
		(struct MenuFuc_Language2*) Menu,
		22,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};


	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
			}
		}
		else
		{
			CurProc = CurProc_Return;
			return MenuOp.RetVal;
		}
	}
}




char AdvancedSet (char *msg)
{
	unsigned char OperateCode[30];
	char retval;
	PasswordInputArgument arg;
	static struct MenuFuc_Language2 Menu[] =
	{
		{"1.高级设置密码设置", "1.Advanced Password", OpPsword},		//注意:  第一个字符串的长度不能大于MaxByteOneLine-1
		{"2.初始化变量设置", "2.FoodOrder paras", InitParameterSet},
		{"3.PIN设置", "3.PIN set", PinSet},
		{"4.恢复出厂设置", "4.Reset Factory", RestoreChuChangSet},		
		{"5.格式化EEPROM", "5.Format EEPROM", FormatEEPROM},
		{"6.终端ID设置", "6.Terminal ID", TerminalIDSet},
		{"7.初始密钥设置", "7.Terminal Password", TerminalPasswordSet},
	};
	struct MenuOperate_Language2 MenuOp={Menu,sizeof(Menu) / sizeof(Menu[0]),0, 0,0,MaxRowOneScr, 1,0,0};


	PassWordArgDeInit (&arg, OperateCode, 16);
	arg.LocalTitle = "【请输入密码】";
	retval = PassWordInput_MiddledDisplay (&arg);

	if (retval != KEY_Enter) return retval;

	if (strcmp ( (sc8*) EnterAdvancedSetPassWord, (sc8*) OperateCode) != 0)
	{
		DrawTitle_to_row_Language2 (2, "密码错误!", "Password error");
		return (WaitkeyAndAutoExit (10) );
	}

	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (NULL);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
			}
		}
		else
		{
			return (MenuOp.RetVal);
		}
	}
}



char MainMenuScreen_Settings (char *msg)
{
	const static struct MenuFuc_Language2 Menu[] =
	{
		{"1.基本设置", "1.Basic Settings", BasicSettings},
		{"2.高级设置", "2.Advanced Settings", AdvancedSet},
#ifdef HaveWiFiModule			
		{"3.WiFi设置","3.WiFi Settings",WIFISet}		
#endif	
	};
	struct MenuOperate_Language2 MenuOp =
	{
		(struct MenuFuc_Language2*) Menu,
		sizeof(Menu) / sizeof(Menu[0]),
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};
	
	kmy_EnableDisableTime3(0);

	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				kmy_EnableDisableTime3(1);
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
			}
		}
		else
		{
			CurProc = CurProc_Return;
			kmy_EnableDisableTime3(1);
			return MenuOp.RetVal;
		}
	}
}


char WANSet(char *msg)
{
//	unsigned char OperateCode[30];
//	char retval;
//	PasswordInputArgument arg;
	static struct MenuFuc_Language2 Menu[]={
		{"1.IP地址设置","1.Local IP set",LoclaIPSet},		//注意:  第一个字符串的长度不能大于MaxByteOneLine-1
		{"2.网关设置","2.Gateway set",GatewaySet},
		{"3.子网掩码设置","3.Mask set",MaskSet},
		{"4.DNS设置","4.DNS set",DNSSet},
	};
	//struct MenuOperate_Language2 MenuOp={Menu,4,0, 0,0,MaxRowOneScr, 1,0,0};
	struct MenuOperate_Language2 MenuOp={Menu,sizeof(Menu) / sizeof(Menu[0]),0,0,0,MaxRowOneScr,1,0,0};

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(NULL);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return (MenuOp.RetVal);
		}
	}
}

static char LoclaIPSet(char *msg)//本地IP地址设置
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy((s8*)buff,(sc8*)LocalIpPortBuf);
	DrawTitle_to_row_Language2(0,"【当前IP地址】","Current IP");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按确认","Change: Confirm");
	text_out_to_row_Language2(4,"退出:请按取消","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)
	{		
		kmy_EnableDisableTime3(1);
		return retval;
	}
	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入IP地址】";
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)
	{		
		kmy_EnableDisableTime3(1);
		return retval;
	}

	clear_lcd();	
	DrawTitle_to_row_Language2(2,"请等待...","Please Wait...");
	if(strlen((sc8*)buff)>21){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:IP地址不对!","Error:IP wrong!");
	}else{
		memset(LocalIpPortBuf,0,sizeof(LocalIpPortBuf));
		strcpy((s8*)LocalIpPortBuf,(sc8*)buff);
		if(!StorageVariable()){
			clear_lcd();
			DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2(2,"更改成功!","change success!");
		}
	}
	kmy_EnableDisableTime3(1);

	return (WaitkeyAndAutoExit(10));
}


static char GatewaySet(char *msg)//本地网关地址设置
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy((s8*)buff,(sc8*)GatewayBuff);
	DrawTitle_to_row_Language2(0,"【当前网关】","Current Gateway");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按确认","Change: Confirm");
	text_out_to_row_Language2(4,"退出:请按取消","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)
	{
		kmy_EnableDisableTime3(1);
		return retval;
	}

	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入网关】";
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)
	{		
		kmy_EnableDisableTime3(1);
		return retval;
	}

	clear_lcd();
	DrawTitle_to_row_Language2(2,"请等待...","Please Wait...");
	if(strlen((sc8*)buff)>21){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:网关不对!","Error:Gateway wrong!");
	}else{
		memset(GatewayBuff,0,sizeof(GatewayBuff));
		strcpy((s8*)GatewayBuff,(sc8*)buff);
		if(!StorageVariable()){
			clear_lcd();
			DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2(2,"更改成功!","change success!");
		}
	}
	kmy_EnableDisableTime3(1);

	return (WaitkeyAndAutoExit(10));
}

static char MaskSet(char *msg)//子网掩码
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;
	
	kmy_EnableDisableTime3(0);

	clear_lcd();
	strcpy((s8*)buff,(sc8*)MaskBuff);
	DrawTitle_to_row_Language2(0,"【当前子网掩码】","Current Mask");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按确认","Change: Confirm");
	text_out_to_row_Language2(4,"退出:请按取消","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)
	{		
		kmy_EnableDisableTime3(1);
		return retval;
	}


	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入子网掩码】";
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)
	{		
		kmy_EnableDisableTime3(1);
		return retval;
	}


	clear_lcd();	
	DrawTitle_to_row_Language2(2,"请等待...","Please Wait...");
	if(strlen((sc8*)buff)>21){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:子网掩码不对!","Error:Mask wrong!");
	}else{
		memset(MaskBuff,0,sizeof(MaskBuff));
		strcpy((s8*)MaskBuff,(sc8*)buff);
		if(!StorageVariable()){
			clear_lcd();
			DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2(2,"更改成功!","change success!");
		}
	}
	kmy_EnableDisableTime3(1);
	return (WaitkeyAndAutoExit(10));
}


static char DNSSet(char *msg)//dns设置
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	
	kmy_EnableDisableTime3(0);
	strcpy((s8*)buff,(sc8*)DNSBuff);
	DrawTitle_to_row_Language2(0,"【当前DNS地址】","Current DNS");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按确认","Change: Confirm");
	text_out_to_row_Language2(4,"退出:请按取消","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)
	{		
		kmy_EnableDisableTime3(1);
		return retval;
	}


	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入DNS地址】";
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)
	{		
		kmy_EnableDisableTime3(1);
		return retval;
	}


	clear_lcd();	
	DrawTitle_to_row_Language2(2,"请等待...","Please Wait...");
	if(strlen((sc8*)buff)>21){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:DNS地址不对!","Error:DNS wrong!");
	}else{
		memset(DNSBuff,0,sizeof(DNSBuff));
		strcpy((s8*)DNSBuff,(sc8*)buff);
		if(!StorageVariable()){
			clear_lcd();
			DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2(2,"更改成功!","change success!");
		}
	}
	kmy_EnableDisableTime3(1);

	return (WaitkeyAndAutoExit(10));
}


char WIFISet(char *msg)
{
	static struct MenuFuc_Language2 Menu[]={
		{"1.搜索网络","1.Search WiFi",Search_Wifi},	
		{"2.参数设置","2.Parameter Set",WIFIParmeterSet},
		{"3.本地设置","3.Local settings",WANSet},
		{"4.重置模块","4.Reset WiFi module",Reset_WiFi}
	};
	struct MenuOperate_Language2 MenuOp={Menu,sizeof(Menu) / sizeof(Menu[0]),0, 0,0,MaxRowOneScr, 1,0,0};

	while(1){
		if(NetworkSwitch[0]!=WiFi)
    	{    	    
			kmy_EnableDisableTime3(0);
			NetworkSwitch[0]=WiFi;
			if (!StorageVariable() )
        	{
        		DrawTitle_to_row_Language2 (3, "网络更改失败!", "Change failure!");
        		WaitkeyAndAutoExit (2);
				
				kmy_EnableDisableTime3(1);
				return 0;
        	}        	
			ChangeNetwork();			
			kmy_EnableDisableTime3(1);
    	}
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter)
		{
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(NULL);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else
			{
				clear_lcd();
				MenuOp.flashflag=1;
			}
		}
		else
		{
			return (MenuOp.RetVal);
		}
	}
}

static char WIFIParmeterSet(char *msg)
{
	static struct MenuFuc_Language2 Menu[]={
			{"1.网络密钥管理","1.WP Manage",WPManage},
			{"2.DHCP开关","2.DHCP Set",DhcpSet},
		};
		struct MenuOperate_Language2 MenuOp={Menu,sizeof(Menu) / sizeof(Menu[0]),0, 0,0,MaxRowOneScr, 1,0,0};
	
		while(1){
			MenuHandler_Language2(&MenuOp);
			if(MenuOp.RetVal==KEY_Enter){
				MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(NULL);
				

				if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface)
				{
					CurProc=CurProc_Return;
					// save wifi saveconfig
					//WifiSaveConfig();
					return MenuOp.FucRetVal;
				} 
				else{clear_lcd();MenuOp.flashflag=1;}
			}else{
				// save wifi saveconfig
				//WifiSaveConfig();
				return (MenuOp.RetVal);
			}
		}


}

static char WIFIParStore(char *msg)
{
//	unsigned char retval;
	kmy_EnableDisableTime3(0);

	if(!StorageVariable()){
		clear_lcd();
		DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		WaitkeyAndAutoExit(10);
	}
	kmy_EnableDisableTime3(1);

	return 0;
	
}


static char DhcpSet(char *msg)
{	
	static unsigned char OptionCn1[20]={"1.关闭Dhcp功能   "};
	static unsigned char OptionEn1[20]={"1.Disable  "};
	static unsigned char OptionCn2[20]= {"2.打开Dhcp功能   "};
	static unsigned char OptionEn2[20]= {"2.Enable  "};

	static struct MenuFuc_Language2 Menu[]={
		{OptionCn1,OptionEn1,WIFIParStore},
		{OptionCn2,OptionEn2,WIFIParStore}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,sizeof(Menu) / sizeof(Menu[0]),0,0,0,MaxRowOneScr,1,0,0
	};

	my_strdel(OptionCn1,'*');
	my_strdel(OptionEn1,'*');
	my_strdel(OptionCn2,'*');
	my_strdel(OptionEn2,'*');

	if(dhcpclient[0]=='0'){
		strcat((s8*)OptionCn1,"*");
		strcat((s8*)OptionEn1,"*");
	}else{
		strcat((s8*)OptionCn2,"*");
		strcat((s8*)OptionEn2,"*");
	}

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			dhcpclient[0] = MenuOp.Menucnt+0x30;
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{
				clear_lcd();
				MenuOp.flashflag=1;
				my_strdel(OptionCn1,'*');
				my_strdel(OptionEn1,'*');
				my_strdel(OptionCn2,'*');
				my_strdel(OptionEn2,'*');
				
				if(dhcpclient[0]=='0'){
					strcat((s8*)OptionCn1,"*");
					strcat((s8*)OptionEn1,"*");
				}else{
					strcat((s8*)OptionCn2,"*");
					strcat((s8*)OptionEn2,"*");
				}
			}
		}else{
			return MenuOp.RetVal;
		}
	}

}

