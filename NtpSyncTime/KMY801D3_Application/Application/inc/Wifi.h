#ifndef __WIFI_H
#define __WIFI_H

#include "kmy_wifi.h"
#include "kmy_WiFi_SPI.h"

#define HaveWiFiModule   //Is there WiFi module?

/*POWER MODE*/
#define RAK_POWER_MODE_0				0
#define RAK_POWER_MODE_1				1
#define RAK_POWER_MODE_2				2
#define RAK_POWER_MODE_3				3

/*AP HIDE MODE*/
#define HIDE_ENABLE			    		1
#define HIDE_DISENABLE					0

/*AP */
#define RAK_AP_CHANNEL		  			7
#define RAK_AP_SSID			     		"KMY801_AP"
#define RAK_AP_PSK			     		"123456789"
#define RAK_AP_HIDE_MODE				HIDE_DISENABLE
#define RAK_COUNTRYCODE				 	"CN"
#define RAK_LISTEN_MODE					100    //ms

/*PING*/
#define RAK_PING_ADDRESS				"192.168.1.1"

/*IP*/
#define RAK_IPSTATIC_IP_ADDRESS			"192.168.1.122"
#define RAK_IPSTATIC_NETMASK			"255.255.255.0"
#define RAK_IPSTATIC_GATEWAY			"192.168.1.1"
#define RAK_IPSTATIC_DNS1				"192.168.1.1"
#define RAK_IPSTATIC_DNS2				"8.8.8.8"


/*DHCP MODE */
#define RAK_DHCP_CLENT_ENABLE			0x00
#define RAK_DHCP_SERVER_ENABLE			0x01

/*NET MODE*/
#define NET_STATION      				0
#define NET_AP							1
#define NET_ADHOC						2

/*SCAN*/
#define RAK_SCAN_CHANNEL   				0
#define RAK_SCAN_SSID			 		""

/*PSK*/
#define RAK_SET_PSK			     		"123456789"

/*GETSCAN NUM*/
#define RAK_GETSCAN_NUM					10

/*DNS*/
#define RAK_DNS_NAME					"www.baidu.com"

/*NET MODE SELECT*/
#define RAK_NET_MODE			 		NET_STATION

/*DHCP SELECT*/
#define RAK_DHCP_MODE			 	  	RAK_DHCP_CLENT_ENABLE


/*Encryption mode*/
#define	RAK_SECURITY_WPA2   			1
#define	RAK_SECURITY_OPEN	 			0
#define RAK_PARAM_SECURITY		 		RAK_SECURITY_OPEN

/*RAK PARAM*/
#define RAK_PARAM_NET_MODE			 	NET_AP
#define RAK_PARAM_HIDE_MODE				HIDE_DISENABLE
#define RAK_PARAM_CHANNEL		  		6
#define RAK_PARAM_DHCP_MODE			 	RAK_DHCP_SERVER_ENABLE
#define RAK_PARAM_POWER_MODE			RAK_POWER_MODE_0
#define RAK_PARAM_SSID			     	"KMY801_AP"
#define RAK_PARAM_PSK			     	"123456789"
#define RAK_PARAM_IP_ADDRESS			"192.168.11.122"
#define RAK_PARAM_NETMASK			 	"255.255.255.0"
#define RAK_PARAM_GATEWAY			 	"192.168.11.1"
#define RAK_PARAM_DNS1				  	"192.168.11.1"
#define RAK_PARAM_DNS2				  	"0"
#define RAK_PARAM_BEACON_MODE		 	100

/*PING*/
#define RAK_PING_HOSTADDR				"192.168.1.1"
#define RAK_PING_COUNT					5
#define RAK_PING_SIZE					1000


 /*RAK WEB */
#define RAK_WEB_USERNAME				"admin"
#define RAK_WEB_USERPSK				 	"admin"
#define RAK_WEB_NET_MODE			 	NET_AP
#define RAK_WEB_HIDE_MODE				0
#define RAK_WEB_CHANNEL		  			6
#define RAK_WEB_DHCP_MODE			 	RAK_DHCP_SERVER_ENABLE
#define RAK_WEB_POWER_MODE			  	RAK_POWER_MODE_0
#define RAK_WEB_SSID			     	"KMY801_AP"
#define RAK_WEB_PSK			     		"123456789"
#define RAK_WEB_IP_ADDRESS				"192.168.11.122"
#define RAK_WEB_NETMASK			 		"255.255.255.0"
#define RAK_WEB_GATEWAY			 		"192.168.11.1"
#define RAK_WEB_DNS1				  	"192.168.11.1"
#define RAK_WEB_DNS2				  	"0"
#define RAK_WEB_BEACON_MODE		 		100


/*SOCKET MODE*/
#define	 RAK_SOCKET_TCP_CLIENT			0
#define	 RAK_SOCKET_TCP_SEVER	    	1
#define  RAK_SOCKET_UDP_CLIENT	    	2
#define	 RAK_SOCKET_UDP_SEVER			3


/*SOCKET PARAM */
#define  RAK_MODULE_SOCKET_MODE   		RAK_SOCKET_TCP_CLIENT

#define  LOCAL_PORT						0

//destination address
extern volatile uint32 	   DEST_IP;			        //同网络中 服务器的IP地址
extern volatile uint16      DEST_PORT;
extern volatile uint16      socket_flag;
extern char        FirstGetSSID; 


#define TCP_UserSelfReturn				0x14

#define TCP_ConnectFailure				0x50
#define TCP_ConnectOK					0x51

#define TCP_SendFailure					0x52
#define TCP_SendOK						0x53

#define TCP_ReceiveBuffNotEnough		0x54
#define TCP_ReceiveTimeOut				0x56
#define TCP_ReceiveOK					0x58

char kmy_WifiInit(void);
char Reset_WiFi(char *msg);

/*
　　function：Connect to server by wifi.
　　parameter：Dest_addr ==> The server domain or ip,port (e.g "weslen.yupage.com" or "31.170.167.97,80").   
　　return value：TCP_ConnectOK if connect ok.
*/
unsigned char WiFi_Connect(unsigned char *Dest_addr);


/*
　　function：Send data to server by wifi.
　　parameter：buff ==> Send data buffer.
               len ==> The lenth of data.
　　return value：TCP_SendOK if send ok.
*/
unsigned char WiFi_Send(unsigned char *buff,unsigned int len);


/*
　　function：Receive data from server by wifi.
　　parameter：recBuff ==> Receive buffer.
               recBuffSize ==> The size of receive buffer.
               okstr ==> If received the string means receive ok.
               second ==> Receive timeout value.
　　return value：TCP_ReceiveOK if receive ok.
*/
unsigned char WiFi_Receive (unsigned char *recBuff, unsigned int recBuffSize, const char *okstr, unsigned long second);


/*=============================================================================*/
/**
 * @fn			void rak_asciiDotAddressTouint32(uint8 *hexAddr, int8 *asciiDotAddress)
 * @brief		Convert an ASCII . notation network address to 4-byte hex address
 * @param[in]		int8 *asciiDotFormatAddress, source address to convert, must be a null terminated string
 * @param[out]		uint8 *hexAddr,	Output value is passed back in the 4-byte Hex Address
 * @return		none
 */
void rak_asciiDotAddressTouint32(uint8 *hexAddr, int8 *asciiDotAddress);

void WiFi_Close(void);
char Search_Wifi(char* msg);
char WPManage(char *msg);
int auto_connect_to_wifi(void);
void DisplayWiFiSignal(unsigned char Signal);

#endif

