
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Wifi.h"
#include "kmy_WiFi.h"
#include "kmy_WiFi_SPI.h"
#include "kmy_KeyDrv.h"
#include "setting.h"
#include "MenuOperate.h"
#include "ShuRuFa.h"
#include "global_extern_variable.h"
#include "lcd.h"
#include "kmy_Time.h"
#include "key.h"

volatile uint32 	 DEST_IP=0;			        //同网络中 服务器的IP地址
volatile uint16      DEST_PORT=0;
volatile uint16      socket_flag;
char        FirstGetSSID=1; 



void rak_asciiDotAddressTouint32(uint8 *hexAddr, int8 *asciiDotAddress)
{
  uint8			i;
  // loop counter
  uint8			cBufPos;
  // which char in the ASCII representation
  uint8			byteNum;
  // which byte in the 32BitHexAddress
   uint8			cBuf[4];
  // character buffer

  byteNum = 3;
  cBufPos = 0;
  for (i = 0; i < strlen((char *)asciiDotAddress); i++) {
    // this will take care of the first 3 octets
    if (asciiDotAddress[i] == '.')
	{
	    // we are at the end of the address octet
      cBuf[cBufPos] = 0;
      // terminate the string
      cBufPos = 0;
      // reset for the next char
      hexAddr[byteNum--] = (uint8)atoi((char *)cBuf);
      // convert the strint to an integer
    }
    else {
      cBuf[cBufPos++] = asciiDotAddress[i];
    }
  }
  // handle the last octet
  // // we are at the end of the string with no .
  cBuf[cBufPos] = asciiDotAddress[i];
  // terminate the string
  hexAddr[byteNum] = (uint8)atoi((char *)cBuf);
  // convert the strint to an integer
}


int16 rak_init_struct(rak_api *ptrStrApi)
{
	//scan
	ptrStrApi->uScanFrame.channel = RAK_SCAN_CHANNEL;
	strcpy((char *)&ptrStrApi->uScanFrame.ssid, RAK_SCAN_SSID);

	//psk
	strcpy((char *)&ptrStrApi->uPskFrame.psk, RAK_SET_PSK);

    //channel
	ptrStrApi->uChannel.channel=RAK_AP_CHANNEL;

	//beacon
	ptrStrApi->uListenFrame.time = RAK_LISTEN_MODE;

	//apconfig
	ptrStrApi->uApconfigFrame.hidden = RAK_AP_HIDE_MODE;
	strcpy((char *)&ptrStrApi->uApconfigFrame.countryCode, RAK_COUNTRYCODE);

	//ipcinfig
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uIpstaticFrame.addr, (int8 *)RAK_IPSTATIC_IP_ADDRESS);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uIpstaticFrame.mask, (int8 *)RAK_IPSTATIC_NETMASK);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uIpstaticFrame.gw, (int8 *)RAK_IPSTATIC_GATEWAY);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uIpstaticFrame.dns1, (int8 *)	RAK_IPSTATIC_DNS1);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uIpstaticFrame.dns2, (int8 *)	RAK_IPSTATIC_DNS2);

	//dhcp mode
	ptrStrApi->uIpdhcpFrame.mode= RAK_DHCP_MODE;

	//getscan num
	ptrStrApi->uGetscan.scan_num=RAK_GETSCAN_NUM;

	//connect
	strcpy((char *)&ptrStrApi->uConnFrame.ssid, "360WiFi-6408");
	ptrStrApi->uConnFrame.mode=	RAK_NET_MODE;

	//ping
	ptrStrApi->uPingFrame.count=RAK_PING_COUNT;
	ptrStrApi->uPingFrame.size=RAK_PING_SIZE;
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uPingFrame.hostaddr, (int8 *)RAK_PING_ADDRESS);

	//dns
	strcpy((char *)&ptrStrApi->uDnsFrame.name, RAK_DNS_NAME);

	//rak param
	ptrStrApi->uParamFrame.rak_param.net_type	 =RAK_PARAM_NET_MODE;
	ptrStrApi->uParamFrame.rak_param.channel	 =RAK_PARAM_CHANNEL;
	ptrStrApi->uParamFrame.rak_param.sec_mode	 =RAK_PARAM_SECURITY;
	ptrStrApi->uParamFrame.rak_param.dhcp_mode 	 =RAK_PARAM_DHCP_MODE;
	strcpy((char *)&ptrStrApi->uParamFrame.rak_param.ssid, RAK_PARAM_SSID);
	strcpy((char *)&ptrStrApi->uParamFrame.rak_param.psk, RAK_PARAM_PSK);
	ptrStrApi->uParamFrame.rak_param.dummy[0]=0;
	ptrStrApi->uParamFrame.rak_param.dummy[1]=0;
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uParamFrame.rak_param.ip_param.addr, (int8 *)RAK_PARAM_IP_ADDRESS);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uParamFrame.rak_param.ip_param.mask, (int8 *)RAK_PARAM_NETMASK);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uParamFrame.rak_param.ip_param.gw, (int8 *)RAK_PARAM_GATEWAY);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uParamFrame.rak_param.ip_param.dns1, (int8 *)	RAK_PARAM_DNS1);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uParamFrame.rak_param.ip_param.dns2, (int8 *)	RAK_PARAM_DNS2);
	ptrStrApi->uParamFrame.rak_param.ap_param.hidden = RAK_PARAM_HIDE_MODE;
	strcpy((char *)&ptrStrApi->uParamFrame.rak_param.ap_param.countryCode, RAK_COUNTRYCODE);



	/*rak  web param*/
	strcpy((char *)&ptrStrApi->uWebFrame.user_name, RAK_WEB_USERNAME);
	strcpy((char *)&ptrStrApi->uWebFrame.user_psk, RAK_WEB_USERPSK);
	ptrStrApi->uWebFrame.net_params.net_type	 =RAK_PARAM_NET_MODE;
	ptrStrApi->uWebFrame.net_params.channel	 =RAK_PARAM_CHANNEL;
	ptrStrApi->uWebFrame.net_params.sec_mode	 =RAK_PARAM_SECURITY;
	ptrStrApi->uWebFrame.net_params.dhcp_mode =RAK_PARAM_DHCP_MODE;
	strcpy((char *)&ptrStrApi->uWebFrame.net_params.ssid, RAK_PARAM_SSID);
	strcpy((char *)&ptrStrApi->uWebFrame.net_params.psk, RAK_PARAM_PSK);
	ptrStrApi->uWebFrame.net_params.dummy[0]=0;
	ptrStrApi->uWebFrame.net_params.dummy[1]=0;
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uWebFrame.net_params.ip_param.addr, (int8 *)RAK_PARAM_IP_ADDRESS);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uWebFrame.net_params.ip_param.mask, (int8 *)RAK_PARAM_NETMASK);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uWebFrame.net_params.ip_param.gw, (int8 *)RAK_PARAM_GATEWAY);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uWebFrame.net_params.ip_param.dns1, (int8 *)	RAK_PARAM_DNS1);
	rak_asciiDotAddressTouint32((uint8 *)&ptrStrApi->uWebFrame.net_params.ip_param.dns2, (int8 *)	RAK_PARAM_DNS2);
	ptrStrApi->uWebFrame.net_params.ap_param.hidden = RAK_PARAM_HIDE_MODE;
	strcpy((char *)&ptrStrApi->uWebFrame.net_params.ap_param.countryCode, RAK_COUNTRYCODE);


	return 0;
}

char kmy_WifiInit(void)
{
    int retval=0;
	
	kmy_Wifi_SPI_Init();
	rak_init_struct(&rak_strapi);
    //while(!ReadINTPin());
    msleep(1000);
	retval=rak_sys_init(&uCmdRspFrame);
	printf("retval:0x%02x\r\n",retval);
	if(retval!=RUN_OK)return retval;	
	printf("%s\r\n",uCmdRspFrame.initResponse.strdata);
	retval=rak_get_version();
	if(retval!=RUN_OK)return retval;
	printf("%s\r\n",uCmdRspFrame.initResponse.strdata);

/*************************固件升级程序↓******************************/	
//	rak_store_web(&rak_strapi.uWebFrame);
//	rak_start_web();
//    while(1);
/*************************固件升级程序↑******************************/   

	auto_connect_to_wifi();
	return 0;
}

#define DEBUG printf("----%d----\r\n",__LINE__);

int auto_connect_to_wifi(void)
{	
//	int retval=0;
//	int i;
	char ssid[35]={0};
	char psk[70]={0};
	
	memset(uCmdRspFrame.CmdRspBuf,0,sizeof(uCmdRspFrame.CmdRspBuf));
	if(rak_get_storeconfig()!=RUN_OK)goto DISCONNECT; //get config

	//strcpy(SSID,uCmdRspFrame.paramFrame.rak_param.ssid);
	strcpy(ssid,uCmdRspFrame.paramFrame.rak_param.ssid);
	strcpy(psk,uCmdRspFrame.paramFrame.rak_param.psk);
	printf("%s\r\n",uCmdRspFrame.paramFrame.rak_param.ssid);
    printf("%s\r\n",uCmdRspFrame.paramFrame.rak_param.psk);
	if(strlen(ssid)==0)goto DISCONNECT;
/*
    if(rak_uscan(&rak_strapi.uScanFrame)!=RUN_OK)goto DISCONNECT;	
	printf("WiFi_num:%d\r\n",uCmdRspFrame.scanResponse.ap_num);
	
	if(rak_getscan(&rak_strapi.uGetscan)!=RUN_OK)goto DISCONNECT;
    for(i=0;i<RAK_AP_SCANNED_MAX;i++)
    {
		if(strstr(uCmdRspFrame.getscanResponse.strScanInfo[i].ssid,ssid))retval=1;
	}
    if(retval!=1)goto DISCONNECT;
	
	if(strchr(dhcpclient,'1'))
	{
*/		
    if(rak_auto_connect()!=RUN_OK)goto DISCONNECT;//auto connect
/*		memcpy(&rak_strapi.uPingFrame.hostaddr,uCmdRspFrame.ipparamFrameRcv.gateway,4);
	}
	else
	{
		strcpy(rak_strapi.uPskFrame.psk,psk);
		printf("rak_strapi.uPskFrame.psk:<%s>\r\n",rak_strapi.uPskFrame.psk);
		if(rak_set_psk(&rak_strapi.uPskFrame)!=RUN_OK)goto DISCONNECT;
		
		strcpy(rak_strapi.uConnFrame.ssid,ssid);
		if(rak_connect(&rak_strapi.uConnFrame)!=RUN_OK)goto DISCONNECT;
		
		if(rak_get_net_status()!=RUN_OK)goto DISCONNECT;
		
		rak_asciiDotAddressTouint32((uint8 *)&rak_strapi.uIpstaticFrame.addr, LocalIpPortBuf);
		rak_asciiDotAddressTouint32((uint8 *)&rak_strapi.uIpstaticFrame.mask, MaskBuff);
		rak_asciiDotAddressTouint32((uint8 *)&rak_strapi.uIpstaticFrame.gw, GatewayBuff);
		rak_asciiDotAddressTouint32((uint8 *)&rak_strapi.uIpstaticFrame.dns1, DNSBuff);
		if(rak_set_ipstatic(&rak_strapi.uIpstaticFrame)!=RUN_OK)goto DISCONNECT;		
		rak_strapi.uPingFrame.hostaddr=rak_strapi.uIpstaticFrame.gw;	
	}
	
	if(rak_ping(&rak_strapi.uPingFrame)!=RUN_OK)goto DISCONNECT;//ping geteway  
*/	
    strcpy((char*)SSID,ssid);
	FirstGetSSID=0;
	if(rak_ipconfig_query()!=RUN_OK)goto DISCONNECT; //get config
	
	return 0;
	
DISCONNECT:
	memset(SSID,0,sizeof(SSID));
	printf("auto connect fail\r\n");
	return 1;
}


const unsigned char pstrwifiico[6][32]=
{
	//one signal
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	},
	//two	signal
	{0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	},
	//three signal
	{0x00,0x00,0x00,0x00,0x80,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	},	

	//four signal
	{0x00,0x00,0x00,0x00,0x80,0x00,0xE0,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00

	},
	//full signal
	{0x00,0x00,0x00,0x00,0x80,0x00,0xE0,0x00,0xF8,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00
	},
	//no signal
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};



void DisplayWiFiSignal(unsigned char Signal)
{
    if(Signal==0)draw_pic(pstrwifiico[5],0,0,16,12);
	else
	{
		Signal = Signal/10; 
		if(Signal>4) Signal = 4;
		draw_pic(pstrwifiico[Signal],0,0,16,12);
	}
}

char DisplayNetwork(int wifi_num)
{
 	int i;
	unsigned char tmp;
	static struct MenuFuc_Language2_Pic Menu[RAK_GETSCAN_NUM];
	struct MenuOperate_Language2_Pic MenuOp={Menu,10,0,0,0,4,1,0,0,};

	//printf("init MenuFuc_Language3\r\n");
	for(i=0;i<wifi_num;i++)
	{
	    if(uCmdRspFrame.getscanResponse.strScanInfo[i].ssid_len == 0)break;
		
		Menu[i].MenuStrLocal = uCmdRspFrame.getscanResponse.strScanInfo[i].ssid;
		Menu[i].MenuStrOther = uCmdRspFrame.getscanResponse.strScanInfo[i].ssid;

		// 显示wifi信号
        tmp=((signed char)uCmdRspFrame.getscanResponse.strScanInfo[i].rssiVal+113)/2;
		printf("SSID:%s 【%d】\r\n",uCmdRspFrame.getscanResponse.strScanInfo[i].ssid,tmp);
		tmp = tmp/10; if(tmp>4) tmp = 4;
		Menu[i].pPic = pstrwifiico[tmp];
	}
	
	MenuOp.MenuTotal = i;//修改菜单的总条数
	if(i==0)//There is no wlan network
	{
		printf("no network\r\n");
		return KEY_ReturnPrevious;
	}
	
	while(1)
    {
		MenuHandler_Language2_Pic(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter)
		{
			//set connet ssid
			strcpy((char *)SSID,(char*)uCmdRspFrame.getscanResponse.strScanInfo[MenuOp.Menucnt].ssid);
			printf("SSID:%s\r\n",SSID);
    		return MenuOp.FucRetVal;
		}
		else
		{
			InitFlag = 1;
			CurProc=CurProc_Return;
			return MenuOp.RetVal;
		}
	}
}

char Reset_WiFi(char *msg)
{
//	unsigned char retval;
	
	clear_lcd();
	DrawTitle_to_row_Language2(2,"请等待...","Pls wait...");

	kmy_EnableDisableTime3(0);

	if(rak_get_net_status()==RUN_OK)
	{
		rak_disconnect();
	}
	
	Reset_Target();
	//while(!ReadINTPin());
	msleep(1000);
	rak_sys_init(&uCmdRspFrame);
	printf("%s\r\n",uCmdRspFrame.initResponse.strdata);
	if(rak_get_version()!=RUN_OK)
	{
		clear_lcd();
		DrawTitle_to_row_Language2(2,"失败!","failure!");
	}
	else
	{
		printf("%s\r\n",uCmdRspFrame.initResponse.strdata);
		clear_lcd();
	    DrawTitle_to_row_Language2(2,"成功!","Success!");
	}
	memset(SSID,0,sizeof(SSID));
	kmy_EnableDisableTime3(1);
	return (WaitkeyAndAutoExit(5));
}

char WPManage(char *msg)
{
	unsigned char buff[40];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)WLANWP);
	DrawTitle_to_row_Language2(0,"【当前网络密钥】","Current Password");
	text_out_to_row_Language2(1,buff,buff);

	abcABC123PYWuBiArgDeInit(&argAa1,buff,30);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入网络密钥】";
	argAa1.OtherTitle="Input Password" ;
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)return retval;

	
	if(strlen((sc8*)buff)>30)
	{
	    clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:密钥长度不对!","Password length error!");
	}
	else
	{
		memset(WLANWP,0,sizeof(WLANWP));
		strcpy((s8*)WLANWP,(sc8*)buff);
		if(!StorageVariable())
		{
		    clear_lcd();
			DrawTitle_to_row_Language2(2,"失败!","failure!");
			WaitkeyAndAutoExit(3);
			return KEY_ReturnPrevious;
		}
		else
	    {
			strcpy(rak_strapi.uPskFrame.psk,(char*)WLANWP);
			printf("rak_strapi.uPskFrame.psk:<%s>\r\n",rak_strapi.uPskFrame.psk);
        	if(rak_set_psk(&rak_strapi.uPskFrame)!=RUN_OK)return retval;
		}
	}
	return (WaitkeyAndAutoExit(1));
}


char Search_Wifi(char* msg)
{
	int retval=0;
	int WiFi_num=0;
//	char gateway[5]={0};

    clear_lcd();
    DrawTitle_to_row_Language2(2,"【正在搜索网络...】","Searching WiFi...");			

	kmy_EnableDisableTime3(0);

	if(rak_get_net_status()==RUN_OK)
	{
		memset(SSID,0,sizeof(SSID));
		if(rak_disconnect()!=RUN_OK)goto FAIL;
	}

    if(rak_uscan(&rak_strapi.uScanFrame)!=RUN_OK)goto FAIL;
	
	WiFi_num=uCmdRspFrame.scanResponse.ap_num;
	printf("WiFi_num:%d\r\n",WiFi_num);
	if(WiFi_num>RAK_GETSCAN_NUM)WiFi_num=RAK_GETSCAN_NUM;
	
	if(rak_getscan(&rak_strapi.uGetscan)!=RUN_OK)goto FAIL;

	retval = DisplayNetwork(WiFi_num);
	if((retval==KEY_ReturnInitInterface)||(retval==KEY_ReturnPrevious))
	{
		memset(SSID,0,sizeof(SSID));
		kmy_EnableDisableTime3(1);
		return retval;
	}

	retval = WPManage(NULL);	
	if((retval==KEY_ReturnInitInterface)||(retval==KEY_ReturnPrevious))
	{
		memset(SSID,0,sizeof(SSID));
		kmy_EnableDisableTime3(1);
		return retval;
	}

	clear_lcd();
	DrawTitle_to_row_Language2(2,"【正在连接网络...】","Connecting WiFi...");

	strcpy(rak_strapi.uConnFrame.ssid,(char*)SSID);
	if(rak_connect(&rak_strapi.uConnFrame)!=RUN_OK)goto FAIL;
	
	if(rak_get_net_status()!=RUN_OK)goto FAIL;

	if(strchr((char*)dhcpclient,'1'))
	{
		if(rak_ipconfig_dhcp(RAK_DHCP_CLENT_ENABLE)!=RUN_OK)goto FAIL;		
		memcpy(&rak_strapi.uPingFrame.hostaddr,uCmdRspFrame.ipparamFrameRcv.gateway,4);
	}
	else
	{
		rak_asciiDotAddressTouint32((uint8 *)&rak_strapi.uIpstaticFrame.addr, (int8_t*)LocalIpPortBuf);
		rak_asciiDotAddressTouint32((uint8 *)&rak_strapi.uIpstaticFrame.mask, (int8_t*)MaskBuff);
		rak_asciiDotAddressTouint32((uint8 *)&rak_strapi.uIpstaticFrame.gw, (int8_t*)GatewayBuff);
		rak_asciiDotAddressTouint32((uint8 *)&rak_strapi.uIpstaticFrame.dns1, (int8_t*)DNSBuff);
		if(rak_set_ipstatic(&rak_strapi.uIpstaticFrame)!=RUN_OK)goto FAIL;		
		rak_strapi.uPingFrame.hostaddr=rak_strapi.uIpstaticFrame.gw;
	}
	
	if(rak_ping(&rak_strapi.uPingFrame)!=RUN_OK)goto FAIL;//ping geteway  

	if(rak_store_config()!=RUN_OK)goto FAIL;    	
	
	clear_lcd();
	DrawTitle_to_row_Language2(2,"成功!","Success!");
	
    rak_get_storeconfig();//get config infor
	printf("%s\r\n",uCmdRspFrame.paramFrame.rak_param.ssid);
	printf("%s\r\n",uCmdRspFrame.paramFrame.rak_param.psk);
    printf("Connect Success\r\n");
	FirstGetSSID=0;
	kmy_EnableDisableTime3(1);
    return WaitkeyAndAutoExit(5);
	
FAIL:
    //rak_get_socket_status();
    printf("Connect Fail\r\n");
	memset(SSID,0,sizeof(SSID));
	kmy_EnableDisableTime3(1);
    clear_lcd();
	DrawTitle_to_row_Language2(2,"失败!","Fail!");
    return WaitkeyAndAutoExit(10);
}

unsigned char WiFi_Connect(unsigned char *Dest_addr)
{
	char *p=NULL;
	char i;
	char IP[50]={0};
	char Port[6]={0};
	uint16_t retval;

	if(Dest_addr==NULL)return 1;
	if(strchr((char*)Dest_addr,','))
	{
		p=strchr((char*)Dest_addr,',');
		strncpy(IP,(char*)Dest_addr,p-(char*)Dest_addr);
		p++;
		strcpy(Port,p);
	}
	else
	{
		strcpy(IP,(char*)Dest_addr);
		strcpy(Port,"80");
	}
	printf("IP:%s; Port:%s\r\n",IP,Port);

	retval=rak_get_socket_status();
	if(retval==RSPCODE_SOCKET_SVR)return TCP_ConnectOK;
	else if(retval==RSPCODE_RECV_DATA)
	{
		rak_read_packet(&uCmdRspFrame);
		rak_clearPktIrq();
		printf("recvDataBuf:\r\n%s\r\n",uCmdRspFrame.recvFrame.recvDataBuf);
		memset(uCmdRspFrame.CmdRspBuf,0,sizeof(uCmdRspFrame.CmdRspBuf));
	}

	if(DEST_PORT==0)DEST_PORT=atoi(Port);
	if(DEST_IP==0)
	{
    	if(Dest_addr[0]>='0'&&Dest_addr[0]<='9')
    	{
    		rak_asciiDotAddressTouint32((uint8 *)&DEST_IP,(int8_t*)IP);		
    	}
    	else
    	{
            strcpy((char*)rak_strapi.uDnsFrame.name,IP);
    		if(rak_query_dns(&rak_strapi.uDnsFrame)!=RUN_OK)
    		{
    			printf("DNS fail\r\n");
    			return TCP_ConnectFailure;
    		}
    		memcpy((uint8*)&DEST_IP,uCmdRspFrame.DnsFrame.addr,4);
    		printf("Dest IP: %2x %2x %2x %2x\r\n",uCmdRspFrame.DnsFrame.addr[0],uCmdRspFrame.DnsFrame.addr[1],uCmdRspFrame.DnsFrame.addr[2],uCmdRspFrame.DnsFrame.addr[3]);
    	}
	}
		
	for(i=0;i<3;i++)
    {	
		retval=rak_open_socket(LOCAL_PORT,DEST_PORT,RAK_MODULE_SOCKET_MODE,(uint32_t)DEST_IP);
		if(retval==RUN_OK)break;
		printf("retval:%02x Fail to open socket,try again\r\n",retval);
		printf("Rsp:\r\n");
		for(retval=0;retval<30;retval++)
		{
			printf(" %2x",uCmdRspFrame.CmdRspBuf[retval]);
		}
		printf("\r\nEnd\r\n");
		WiFi_Close();
		retval=rak_get_socket_status();
		if(retval==RSPCODE_SOCKET_SVR)return TCP_ConnectOK;
		else if(retval==RSPCODE_RECV_DATA)
		{
		    rak_read_packet(&uCmdRspFrame);
    	 	rak_clearPktIrq();			
			printf("recvDataBuf:\r\n%s\r\n",uCmdRspFrame.recvFrame.recvDataBuf);
			memset(uCmdRspFrame.CmdRspBuf,0,sizeof(uCmdRspFrame.CmdRspBuf));
		}
		msleep(5000);
    }

    if(retval==RUN_OK)
    {
		socket_flag=uCmdRspFrame.recvFrame.socket_flag; 
		printf("socket_flag: %d\r\n",socket_flag);
		return TCP_ConnectOK;
	}
	else
	{
		printf("Fail to open socket\r\n");
		return TCP_ConnectFailure;
	}
}

void WiFi_Close(void)
{	
//	uint16_t retval;

	if(rak_socket_close(socket_flag)==RUN_OK)
	{
		printf("WiFi Close OK\r\n");
		return;
  	}
	printf("WiFi Close Error\r\n");
}

unsigned char WiFi_Send(unsigned char *buff,unsigned int len)
{
    uint16_t retval;
	
	retval=rak_send_data(0,0,socket_flag,len,buff);
	if(retval==RUN_OK)return TCP_SendOK;	
	return TCP_SendFailure;
}

unsigned char WiFi_Receive (unsigned char *recBuff, unsigned int recBuffSize, const char *okstr, unsigned long second)
{
	unsigned char keyval = KEY_NONE;
	unsigned int RecvLen=0;
	unsigned char ReceiveOK_flag=0;

	memset(recBuff,0,recBuffSize);	
	memset(uCmdRspFrame.CmdRspBuf,0,sizeof(uCmdRspFrame.CmdRspBuf));
	kmy_AutoPostKey(second);
	while(1)
	{
/*		
		while(!ReadINTPin())
		{			
			keyval = kmy_GetKeyValue();
			if (keyval == KEY_WaitTimeout)
			{
				if(ReceiveOK_flag)return TCP_ReceiveOK;
				else return TCP_ReceiveTimeOut;
			}
			if (keyval == KEY_ReturnPrevious||keyval == KEY_ReturnInitInterface) return TCP_UserSelfReturn;
		}
*/		
    	if(rak_checkPktIrq() == RAK_TRUE)
     	{
    	 	rak_read_packet(&uCmdRspFrame);
    	 	rak_clearPktIrq();
    		if(uCmdRspFrame.rspCode[0]==RSPCODE_RECV_DATA)
    		{				
    		    //printf("%s",uCmdRspFrame.recvFrame.recvDataBuf);
				RecvLen+=uCmdRspFrame.recvFrame.data_len;
				if(RecvLen<recBuffSize)strcat((char*)recBuff,(char*)uCmdRspFrame.recvFrame.recvDataBuf);
				else return TCP_ReceiveBuffNotEnough;
				if(strstr((char*)uCmdRspFrame.recvFrame.recvDataBuf,okstr))
				{
					ReceiveOK_flag=1;
					kmy_AutoPostKey(6);
				}				
				memset(uCmdRspFrame.CmdRspBuf,0,sizeof(uCmdRspFrame.CmdRspBuf));
    		}
     	}
		keyval = kmy_GetKeyValue();
		if (keyval == KEY_WaitTimeout)
		{
			if(ReceiveOK_flag)return TCP_ReceiveOK;
			else return TCP_ReceiveTimeOut;
		}
		if (keyval == KEY_ReturnPrevious||keyval == KEY_ReturnInitInterface) return TCP_UserSelfReturn;
	}
}



