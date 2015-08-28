#include "SyncTime.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lcd.h"
#include "setting.h"
#include "kmy_time.h"
char SyncTime(char *buff)
{
//	unsigned char buff[100]={"Date: Thu, 05 Feb 2014 06:02:55 GMT"};
//	unsigned char TimeZone[10]={"UTC+08:00"};
//	unsigned char timezone;
	unsigned char Zone_hour;
	unsigned char Zone_min;
	unsigned int year = 0;
	unsigned char month = 0;
	unsigned char day = 0;
	unsigned char hour = 0;
	unsigned char minute = 0;
	unsigned char second = 0;
	char YearofMonth[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	char *p=NULL;


	p=strstr(buff,"Date:");
	if(p==NULL)goto FailExit;
	p=strchr(p,',');
	if(p==NULL)goto FailExit;
	p+=2;
	day=atoi(p);
	printf("day:<%d>\r\n",day);

	p=strchr(p,' ');
	if(p==NULL)goto FailExit;
	p++;
	if(strstr(p,"Jan"))month=1;
	else if(strstr(p,"Feb"))month=2;
	else if(strstr(p,"Mar"))month=3;
	else if(strstr(p,"Apr"))month=4;
	else if(strstr(p,"May"))month=5;
	else if(strstr(p,"Jun"))month=6;
	else if(strstr(p,"Jul"))month=7;
	else if(strstr(p,"Aug"))month=8;
	else if(strstr(p,"Sep"))month=9;
	else if(strstr(p,"Oct"))month=10;
	else if(strstr(p,"Nov"))month=11;
	else month=12;
	printf("month:<%d>\r\n",month);

	p=strchr(p,' ');
	if(p==NULL)goto FailExit;
	p++;
	year=atoi(p);
	printf("year:<%d>\r\n",year);

	p=strchr(p,' ');
	if(p==NULL)goto FailExit;
	p++;
	hour =atoi(p);
	printf("hour:<%d>\r\n",hour);

	p=strchr(p,':');
	if(p==NULL)goto FailExit;
	p++;
	minute=atoi(p);
	printf("minute:<%d>\r\n",minute);

	p=strchr(p,':');
	if(p==NULL)goto FailExit;
	p++;
	second=atoi(p);
	printf("second:<%d>\r\n",second);

	if(strchr((char*)TimeZone,'+'))
	{
		p=strchr((char*)TimeZone,'+');
		if(p!=NULL)
		{
			p++;
			Zone_hour=atoi(p);
			p=strchr((char*)TimeZone,':');
			if(p!=NULL)
			{
			    p++;
				Zone_min=atoi(p);
				minute+=Zone_min;
				if(minute>=60)
				{
					minute-=60;
					Zone_hour++;
				}
			}
			
			hour+=Zone_hour;
		}
		if(hour>=24)
		{
			hour-=24;
			day++;
		}
		
//		year=year_H*100+year_L;
		
		if(day>YearofMonth[month])
		{
			 if(month==2)
			 {
				   if(year%4==0&&year%100!=0||year%400==0)
				   {
					   if(day>29)month++;
					   day=1;
				   }
				   else
				   {
					   month++;
					   day=1;
				   }
			 }
			 else
			 {
				   month++;
				   day=1;
			 }
		}
		
		if(month>12)
		{
			month=1;
			year++;
		}
	}
	else
	{
		p=strchr((char*)TimeZone,'-');
		if(p!=NULL)
		{
			p++;
			Zone_hour=atoi(p);
			p=strchr((char*)TimeZone,':');
			if(p!=NULL)
			{
				p++;
				Zone_min=atoi(p);
				if(minute>=Zone_min)
				{
					minute-=Zone_min;
				}
				else
				{
					minute=60-(Zone_min-minute);
					Zone_hour++;
				}
			}
			if(hour>=Zone_hour)hour-=Zone_hour;
			else
			{
				hour=24-(Zone_hour-hour);
				day--;
			}
		}

//		year=year_H*100+year_L;
		if(day==0)
		{
			month--;
			if(month==0)
			{
				month=12;
				year--;
			}	
			if(month==2)
			{
				   if(year%4==0&&year%100!=0||year%400==0)
				   {
					   day=29;
				   }
				   else
				   {
					   day=YearofMonth[month];
				   }
			 }
			 else
			 {
				   day=YearofMonth[month];
			 }
		}
	}	
	printf("Time:%4d-%02d-%02d %02d:%02d:%02d\r\n",year,month,day,hour,minute,second);

	kmy_TimeSetTime (year, month, day, hour, minute, second);
//	clear_lcd();
//	DrawTitle_to_row_Language2(2,"同步时间成功!","Sync time success"); 
//	return WaitkeyAndAutoExit(5);
    printf("Sync time success\r\n");
	return 1;	
FailExit:
//	clear_lcd();
//	DrawTitle_to_row_Language2(2,"同步时间失败!","Sync time fail"); 
//	return WaitkeyAndAutoExit(5);
    printf("Sync time fail\r\n");
    return 0;
}


