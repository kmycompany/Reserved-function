#include "Version.h"
//#include "Stm32f10x.h"
#include "LCD.h"
#include "kmy_LCDDrv.h"
#include "key.h"
#include <stdio.h>
#include <string.h>

const char VersionString[] = "main_ver0.1";	 
const char VersionDate[] = __DATE__" "__TIME__;

char DisplayVersion (char *msg)
{
	char buff[60];
	char buffother[60];
	strcpy (  buff,  VersionString);
	strcpy (  buffother,  VersionString);
	clear_lcd();
	DrawTitle_to_row_Language2 (0, "软件版本", "Software Ver");
	text_out_to_row_Language2 (1, (unsigned char*)buff, (unsigned char*)buffother);
	text_out_to_row_Language2 (2,(unsigned char*)VersionDate,(unsigned char*)VersionDate);
	text_out_to_row_Language2 (3, "硬件版本:20130809", "HardVer:20130809");
	text_out_to_row_Language2 (4, "备注:KMY801D3", "Mark:KMY801D3");
	return (WaitkeyAndAutoExit (15) );
}




