#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ntp.h"
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

#define  NTP_SOCKET_UDP_CLIENT	    	2


unsigned char NtpWiFi_Connect(unsigned char *Dest_addr)
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
		retval=rak_open_socket(LOCAL_PORT,DEST_PORT,NTP_SOCKET_UDP_CLIENT	,(uint32_t)DEST_IP);
		if(retval==RUN_OK)
		{
			printf("open socket success!!!!\n");
			break;
		}
		
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

unsigned char NtpWiFi_Send(unsigned char *buff,unsigned int len,unsigned char *Dest_addr)
{
	char *p=NULL;
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
		
	retval=rak_send_data(DEST_IP,DEST_PORT,socket_flag,len,buff);
	if(retval==RUN_OK)return TCP_SendOK;	
	return TCP_SendFailure;
}

unsigned char NtpWiFi_Receive (unsigned char *recBuff, unsigned int recBuffSize, unsigned long second)
{
	unsigned char keyval = KEY_NONE;
	unsigned int RecvLen=0;
	int i;
	int j=0;


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
     		SyncFlag=1;
     		printf("begin to receive data...\n");
    	 	rak_read_packet(&uCmdRspFrame);
    	 	rak_clearPktIrq();
    		if(uCmdRspFrame.rspCode[0]==RSPCODE_RECV_DATA)
    		{				
    		    //printf("%s",uCmdRspFrame.recvFrame.recvDataBuf);
				RecvLen+=uCmdRspFrame.recvFrame.data_len;
				if(RecvLen<=recBuffSize)
					for(i=0;i<uCmdRspFrame.recvFrame.data_len;i++)					
						recBuff[j++]=uCmdRspFrame.recvFrame.recvDataBuf[i];		
				else return TCP_ReceiveBuffNotEnough;
			
				memset(uCmdRspFrame.CmdRspBuf,0,sizeof(uCmdRspFrame.CmdRspBuf));
    		}
     	}
		keyval = kmy_GetKeyValue();
		if (keyval == KEY_WaitTimeout)
		{
			
			return TCP_ReceiveOK;
		}
		if (keyval == KEY_ReturnPrevious||keyval == KEY_ReturnInitInterface) return TCP_UserSelfReturn;
	}
}



unsigned long TimeZoneConvert(unsigned long sec)
{
	char *p=NULL;
	unsigned long hour,min;
	if(strstr((char *)TimeZone,"UTC"))
	{
		p=strchr((char *)TimeZone,'+');
		if(p!=NULL)
		{
			hour=atoi(p);
			p+=3;
			min=atoi(p);
			sec=sec+hour*60*60+min*60;
		}
		
		p=strchr((char *)TimeZone,'-');
		if(p!=NULL)
		{
			hour=atoi(p);
			p+=3;
			min=atoi(p);
			sec=sec-hour*60*60-min*60;
		}
	}
	printf("hour:%u\nmin%u\n",hour,min);
	return sec;
}


int IsLeapYear(int year)
{
    if(year%4==0&&year%100!=0||year%400==0)
        return 1;
    return 0;
}


void TimeStd(unsigned long curtime,short *destyear,short *destmonth,short *destday,short *destweek,short *desthour,short *destmin,short *destsec)
{
	
	unsigned long  totaltime,atime=0,ttime,mtime=0,dtime=0,htime=0,mitime=0;
	short year = 1900;
	short month = 1;
	short day = 1;
	short hour = 0;
	short min = 0;
	short sec = 0;
	short week;
	int i;
	int totalday=0;
	
	totaltime=curtime;
	
	while(atime<totaltime)
	{
		ttime=atime;
		if(IsLeapYear(year))
			atime+=366*24*60*60;
		else
			atime+=365*24*60*60;
		year++;
	}
	year--;
	
	totaltime=totaltime-ttime;
	
	while(mtime<totaltime)
	{
		ttime=mtime;
		switch(month)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:mtime+=31*24*60*60;break;
			case 4:
			case 6:
			case 9:
			case 11:mtime+=30*24*60*60;break;
			case 2: mtime+=(IsLeapYear(year)==1)?29*24*60*60:28*24*60*60;break;
		}
		month++;
	}
	month--;
		
	totaltime=totaltime-ttime;
	while(dtime<totaltime)
	{
		ttime=dtime;
		dtime+=24*60*60;
		day++;
	}
	day--;
	totaltime=totaltime-ttime;
	while(htime<totaltime)
	{
		ttime=htime;
		htime+=60*60;
		hour++;
	}
	hour--;
//	hour+=8;
		
//	if(hour>=24)
//	{
//		day++;
//		hour-=24;
//	}
	totaltime=totaltime-ttime;
	while(mitime<totaltime)
	{
		ttime=mitime;
		mitime+=60;
		min++;
	}
	min--;
		
	sec=totaltime-ttime;
		
	for(i=1900;i<year;i++)
	{
		if(IsLeapYear(i))
			totalday+=366;
		else
			totalday+=365;
	}
		
	i=1;
	while(i<month)
	{
		switch(i)
		{
		    case 1:
			case 3:
			case 5:
		  	case 7:
           	case 8:
            case 10:totalday+=31;break;
	        case 4:
            case 6:
            case 9:
            case 11:totalday+=30;break;
            case 2: totalday+=(IsLeapYear(year)==1)?29:28;break;
        }
        i++;
    }

    totalday=totalday+day-1;
	printf("totalday:%d\n",totalday);

    week = (1 + totalday)%7;
	printf("week:%d\n",week);

	*destyear=year;
	*destmonth=month;
	*destday=day;
	*desthour=hour;
	*destmin=min;
	*destsec=sec;
	*destweek=week;
	
return;
}

unsigned long ntohl(unsigned long netsqu)
{
	unsigned long hostsqu=0;
	while(netsqu)
	{
		printf("hostsqu:%x\n",hostsqu);
		printf("netsqu:%x\n",netsqu);
		hostsqu = (hostsqu<<8) + netsqu%0x100;
		netsqu = netsqu/0x100;
	}
	return hostsqu;
}
int GetNTPTime(STNP_Header *H_SNTP)
{
   
    memset((void*)H_SNTP, 0,sizeof(STNP_Header));
    H_SNTP->LiVnMode = 0x1b;
    
   	NtpWiFi_Connect(NTP_SERVER_NAME);
	if(NtpWiFi_Send((unsigned char *)H_SNTP,sizeof(STNP_Header),NTP_SERVER_NAME)==TCP_SendOK)
	{
		printf("size:%d\n",sizeof(STNP_Header));
		printf("send data success!!!!\n");
	}
	
    NtpWiFi_Receive((unsigned char *)H_SNTP, sizeof(STNP_Header),10);
    WiFi_Close();
   
    return 0;
}

int SYNC_Time(void)
{
    STNP_Header HeaderSNTP;
    unsigned long t1,t2,t3,t4,dis,curtime;
	short year,month,day,week,hour,min,sec;
    
    t1=kmy_TimeGetRtcSeconds();
    t1+=JAN_1970;
    
    printf("sync time from %s\n", NTP_SERVER_NAME);
    if(GetNTPTime(&HeaderSNTP)<0)
        return -1;
        
   	t4=kmy_TimeGetRtcSeconds();
    t4+=JAN_1970;
    
    t2 = ntohl(HeaderSNTP.RecvTimeInt);
    t3 = ntohl(HeaderSNTP.TranTimeInt);
	t2 = TimeZoneConvert(t2);
	t3 = TimeZoneConvert(t3);

//	printf("LiVnMode:%x\n",HeaderSNTP.LiVnMode);
//	printf("HeaderSNTP.RecvTimeInt:%x\n",HeaderSNTP.RecvTimeInt);
//	printf("HeaderSNTP.TranTimeInt:%x\n",HeaderSNTP.TranTimeInt);

	printf("t1:%x\nt2:%x\nt3:%x\nt4:%x\n",t1,t2,t3,t4);
    dis = ( (t2-t1)+(t3-t4) )/2;
    if(dis<=0)
        printf("local time is faster then server %d seconds\n", (int)-dis);
    else
        printf("local time is slower then server %d seconds\n", (int)dis);
    
    printf("dis:%u\n",dis);

	curtime = kmy_TimeGetRtcSeconds()+JAN_1970+dis;
	TimeStd(curtime,&year,&month,&day,&week,&hour,&min,&sec);
	if(SyncFlag==1)
		kmy_TimeSetTime (year, month, day, hour, min, sec);
	if(SyncFlag==0)
		SyncSec=kmy_TimeGetRtcSeconds()+20;
	printf("%d-%02d-%02d %d %02d:%02d:%02d\n",year,month,day,week,hour,min,sec);
    return 0;
}

