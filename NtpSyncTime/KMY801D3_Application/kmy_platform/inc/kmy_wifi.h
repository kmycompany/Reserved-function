#ifndef _KMY_WIFI_H_
#define _KMY_WIFI_H_

#include <stdio.h>
#include "stm32f10x.h"
#include "kmy_WiFi_SPI.h"


#define  HOST_RESERVE      		   0X97
#define  HOST_DUMMY        		   0X00

#define  RAK_SSID_LEN	 		   32
#define	 RAK_PSK_LEN			   64
#define  RAK_BSSID_LEN	 		   6
#define  RAK_AP_SCANNED_MAX		   10
#define  RAK_MAX_DATA_SIZE	   	   1400
#define  RAK_USER_NAME_LEN		   17
#define  RAK_USER_PSK_LEN		   17

#define  RAK_RSPCODE_LEN		   2
#define  SPI_VALID          	   0X80
#define  SPI_CMD_ACK        	   0X85

/*cmd */
#define  SYS_INIT_CMD              0xA0
#define  READ_STATUS_CMD           0xA1
#define  READ_DATA_CMD             0xA2
#define  SCAN_CMD                  0xA3
#define  GET_SCAN_CMD              0xA4
#define  SET_PSK_CMD               0xA5
#define  CONNECT_CMD               0xA6
#define  GET_CONN_STATUS_CMD       0xA7
#define  SET_LISTEN_CMD            0xA8
#define  GET_RSSI_CMD              0xA9
#define  SET_APCONFIG_CMD          0xAA
#define  SET_CHANNEL_CMD           0xAB
#define  SET_DHCP_MODE_CMD         0xAC
#define  SET_IPSTATIC_CMD          0xAD
#define  IPCONFIG_QUERY_CMD        0xAE
#define  DNS_QUERY_CMD             0xAF
#define  PING_CMD        		   0xB0
#define  CREATE_UDP_CLIENT_CMD     0xB1
#define  CREATE_UDP_SERVER_CMD     0xB2
#define  CREATE_TCP_CLIENT_CMD     0xB3
#define  CREATE_TCP_SERVER_CMD     0xB4
#define  CLOSEPORT_CMD		   	   0xB5
#define  SEND_DATA_CMD             0xB6
#define  DISCONNECT_CMD            0xB7
#define  SET_PWR_CMD               0xB8
#define  GET_STORECONFIG_CMD       0xB9
#define  GET_STOREWEB_CMD          0xBA
#define  SET_BOOT_CMD              0xBC
#define  DEL_DATA_CMD              0xBD
#define  GET_VERSION_CMD           0xBE
#define  SET_RESET_CMD             0xBF
#define  STORE_DATA_CMD            0xC0
#define  STORE_CONFIG_CMD          0xC1
#define  EASY_CONFIG_CMD		   0xC2
#define  WPS_CMD				   0xC3
#define  CREATE_UDP_MULTICAST_CMD  0xC4
#define  STORE_WEB_CMD			   0xC5
#define  AOTU_CONNECT_CMD		   0xC6
#define  START_WEB_CMD			   0xC7
#define  MAX_CMD_NUM               0xC8


/*wifi rsp status code*/
#define  RSPCODE_NET_DISC      	   0xCC
#define  RSPCODE_NET_CONNE     	   0xCB
#define  RSPCODE_SOCKET_CLOSE  	   0xCA
#define  RSPCODE_SOCKET_SVR    	   0xC9
#define  RSPCODE_RECV_DATA     	   0xC8


#define RUN_OK					   0X00

#define RAK_BUSY				   0x55
#define RAK_BUFFER_FULL   		   0X58

#define RAK_RECV_HEAD_LEN		   12

extern uint32					           rak_spiTimer1;
extern uint32					           rak_spiTimer2;
extern uint32					           rak_spiTimer3;

#define RAK_RESET_TIMER1       	   rak_spiTimer1=0
#define RAK_INC_TIMER_1        	   rak_spiTimer1++


/***********************************Command structure**********************************/

/*cmd struct*/
typedef struct
{
    uint32_t   						cmd;
}rak_common;

/*close socket struct*/
typedef struct {
    uint32_t 						cmd;
    uint16_t 						dummy;
	uint16_t 						flag;
}rak_close_t;

/*client socket struct*/
typedef struct {
    uint32_t 						cmd;
    uint32_t 						dest_addr;
    uint16_t 						dest_port;
    uint16_t 						local_port;
}rak_client_t;

/*sever socket struct*/
typedef struct {
    uint32_t 						cmd;
    uint16_t 						dummy;
    uint16_t 						port;
}rak_server_t;

/*send struct*/
typedef struct {
    uint32_t      					cmd;
	uint32_t 						dest_addr;
	uint32_t						dest_port;
    uint16_t      					socket_flag;
    uint16_t      					len;
    char 							buffer[RAK_MAX_DATA_SIZE];
}rak_send_t;

/*channel struct*/
typedef struct {
    uint32_t 						cmd;
    uint32_t 						channel;
}rak_channel_t;

/*scan struct*/
typedef struct
{
    uint32_t   						cmd;
    uint32_t  						channel;
    char      						ssid[RAK_SSID_LEN];
}rak_scan_t;

/*getsan struct*/
typedef struct
{
    uint32_t   						cmd;
    uint32_t  						scan_num;
}rak_getscan_t;

/*connect struct*/
typedef struct {
    uint32_t  						cmd;
    uint32_t  						mode;
    char      						ssid[RAK_SSID_LEN];
}rak_conn_t;

/*dhcp struct*/
typedef struct {
    uint32_t 						cmd;
    uint32_t 						mode;
}rak_ipdhcp_t;

/*ap struct*/
typedef struct {
	uint32_t 						cmd;
    uint8_t  						hidden;
    uint8_t  						countryCode[3];
}rak_apconfig_t;

typedef struct {
    uint8_t  						hidden;
    uint8_t  						countryCode[3];
}rak_ap_param;

/*listen struct*/
typedef struct {
	uint32_t 						cmd;
    uint32_t 						time;
}rak_listen_t;


/*power mode struct*/
typedef struct {
    uint32_t 						cmd;
    uint32_t 						powermode;
}rak_pwr_mode_t;

/*psk struct*/
typedef struct {
    uint32_t 						cmd;
    char 							psk[RAK_PSK_LEN];
}rak_psk_t;

/*ip struct*/
typedef struct {
    uint32_t 						addr;
    uint32_t 						mask;
    uint32_t 						gw;
    uint32_t 						dns1;
    uint32_t 						dns2;
}rak_ip_param;


typedef struct {
    uint32_t      					feature_bitmap;
    uint8_t       					net_type;
    uint8_t       					channel;
    uint8_t       					sec_mode;
    uint8_t       					dhcp_mode;
    char          					ssid[33];
    char          					psk[65];
	uint8_t							dummy[2];
    rak_ip_param   					ip_param;
    rak_ap_param	   				ap_param;
}param_t;

/* param struct*/
typedef struct {
	uint32_t 	  					cmd;
    param_t							rak_param;
}rak_param_t;

/* web struct*/
typedef struct {
    uint32_t  						cmd;
	param_t    						net_params;
	char    						user_name[RAK_USER_NAME_LEN];
	char    						user_psk[RAK_USER_PSK_LEN];
}rak_web_t;

/*ping struct*/
typedef struct {
    uint32_t 						cmd;
    uint32_t 						hostaddr;
	uint32_t 						count;
    uint32_t 						size;
}rak_ping_t;

/*dns struct*/
typedef struct {
    uint32_t 						cmd;
    uint8_t 						name[42];
}rak_dns_t;

/*ipstatic struct*/
typedef struct {
    uint32_t 						cmd;
    uint32_t 						addr;
    uint32_t 						mask;
    uint32_t 						gw;
    uint32_t 						dns1;
    uint32_t 						dns2;
}rak_ipstatic_t;

/*api struct*/
typedef struct {
	uint8							band;
	uint8							powerMode;
	uint8							macAddress[6];
	rak_channel_t					uChannel;
	rak_scan_t						uScanFrame;
	rak_getscan_t 					uGetscan;
	rak_conn_t						uConnFrame;
	rak_ipdhcp_t					uIpdhcpFrame;
	rak_pwr_mode_t					uPwrModeFrame;
	rak_psk_t						uPskFrame;
	rak_param_t						uParamFrame;
	rak_web_t						uWebFrame;
	rak_ping_t						uPingFrame;
	rak_dns_t						uDnsFrame;
	rak_server_t    				uServerFrame;
	rak_ipstatic_t					uIpstaticFrame;
	rak_apconfig_t					uApconfigFrame;
	rak_listen_t					uListenFrame;
} rak_api;


/***********************************Response structure**********************************/


/*scan info struct*/
typedef struct {
	uint8							rfChannel;
	uint8							rssiVal;
	uint16							securityMode;
	uint16							ssid_len;
	uint8							ssid[RAK_SSID_LEN];
	uint8							bssid[RAK_BSSID_LEN];
} rak_scanInfo;



/*scan rsp struct*/
typedef struct {
	uint8                     		rspCode[RAK_RSPCODE_LEN];
	uint16                          ap_num;
	uint8                           status;
} rak_scanResponse;

/*getscan rsp struct*/
typedef struct {
	uint8                     		rspCode[RAK_RSPCODE_LEN];
	rak_scanInfo		            strScanInfo[RAK_AP_SCANNED_MAX];
	uint8                           status;
}rak_getscanResponse;

/*getrssi rsp struct*/
typedef struct {
	uint8                     		rspCode[RAK_RSPCODE_LEN];
	uint16		                    rssi;
	uint8                           status;
}rak_getrssiResponse;

/*socketEst rsp struct*/
typedef struct {
	uint8                     		rspCode[RAK_RSPCODE_LEN];
	uint16         					socket_flag;
	uint16         					dummy;
	uint16         					ip_port;
	uint8		   					ip_addr[4];
} rak_recvsocketEst;

/*version rsp struct*/
typedef struct {
	 uint8                     		rspCode[RAK_RSPCODE_LEN];
	 uint8							host_fw[8];
	 uint8							dummy;
	 uint8							wla_fw[6];
	 uint8                          status;
}rak_versionFrame;

/*ip rsp struct*/
typedef struct {
	uint8                     		rspCode[RAK_RSPCODE_LEN];
	uint8				        	macAddr[6];
	uint8				        	ipaddr[4];
	uint8				        	netmask[4];
	uint8				        	gateway[4];
	uint8				        	dns1[4];
	uint8				        	dns2[4];
	uint8                           status;
}rak_ipparamFrameRcv;

/*socket rsp struct*/
typedef struct {
	uint8                     		rspCode[RAK_RSPCODE_LEN];
	uint16                       	socket_flag;
	uint8                           status;
} rak_socketFrameRcv;

/*init rsp struct*/
typedef struct {
	uint8                     		rspCode[RAK_RSPCODE_LEN];
	uint8                       	strdata[17];
	uint8                           status;
} rak_initResponse;

/*recv rsp struct*/
typedef struct {
	uint8          					rspCode[RAK_RSPCODE_LEN];
	uint16         					socket_flag;
	uint16         					data_len;
	uint16         					ip_port;
	uint8		   					ip_addr[4];
	uint8		   					recvDataBuf[RAK_MAX_DATA_SIZE];
} rak_recvFrame;

/*mgmt rsp struct*/
typedef struct {
	uint8               			rspCode[RAK_RSPCODE_LEN];
    uint8               			status;
} rak_mgmtResponse;

/*easy and wps rsp struct*/
typedef struct {
	uint8               			rspCode[RAK_RSPCODE_LEN];
	uint8							ssid[RAK_SSID_LEN];
	uint8               			sec_mode;
	uint8							psk[RAK_PSK_LEN];
    uint8               			status;
} rak_easynetResponse;

/*param rsp struct*/
typedef struct {
	uint8               			rspCode[RAK_RSPCODE_LEN];
    param_t							rak_param;
	uint8               			status;
}rak_paramResponse;

/*web rsp struct*/
typedef struct {
    uint8               			rspCode[RAK_RSPCODE_LEN];
	param_t    						net_params;
	char    						user_name[RAK_USER_NAME_LEN];
	char    						user_psk[RAK_USER_PSK_LEN];
	uint8               			status;
}rak_webResponse;

/*dns rsp struct*/
typedef struct {
	uint8               			rspCode[RAK_RSPCODE_LEN];
	uint8							addr[4];
    uint8               			status;
} rak_dnsResponse;


/*cmd rsp struct*/
typedef union {
	uint8                     		rspCode[RAK_RSPCODE_LEN];                    		// command code response
	rak_initResponse				initResponse;
	rak_scanResponse			  	scanResponse;
	rak_getscanResponse			  	getscanResponse;
	rak_getrssiResponse			  	getrssiResponse;
	rak_ipparamFrameRcv			  	ipparamFrameRcv;
	rak_socketFrameRcv        		socketFrameRcv;
	rak_recvFrame					recvFrame;
	rak_recvsocketEst           	recvsocketEst;
	rak_versionFrame				versionFrame;
	rak_easynetResponse				easynetFrame;
	rak_mgmtResponse          		mgmtResponse;
	rak_paramResponse				paramFrame;
	rak_webResponse					webFrame;
	rak_dnsResponse					DnsFrame;
	uint8					        CmdRspBuf[RAK_MAX_DATA_SIZE+52];
} rak_CmdRsp;


/*===================================================*/
/**
 * Interrupt Handeling Structure
 */
typedef struct {
	uint8					dataPacketPending;
	uint8				    recvFull;
	uint8         			isrRegLiteFi;
}rak_intStatus;


uint16_t rak_get_socket_status(void);

uint16_t rak_read_status(void);


/*===========================================================================
 *
 * @fn          uint16_t rak_read_data(rak_CmdRsp *uCmdRsp)
 * @brief       Sends the read command to the Wi-Fi module
 * @param[in]   rak_CmdRsp *uCmdRsp,Pointer to CmdRsp structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description
 * This API is used to read data.
 */
uint16_t rak_read_data(rak_CmdRsp *uCmdRsp);


int16 rak_read_packet(rak_CmdRsp *uCmdRspFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_sys_init(rak_CmdRsp *uCmdRsp)
 * @brief       Sends the init  command to the Wi-Fi module
 * @param[in]   rak_CmdRsp *uCmdRsp,Pointer to CmdRsp structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description
 * This API is used to Initialization Wi-Fi module.
 */
uint16_t rak_sys_init(rak_CmdRsp *uCmdRsp);
void SYS_Delay(uint32_t us);
void rak_clearPktIrq(void);
uint8 rak_checkPktIrq(void);
uint8 rak_checkRecvFullIrq(void);
uint8 rak_intHandler(void);


/*===========================================================================
 *
 * @fn          uint16_t rak_getscan(rak_getscan_t *GetScanFrame)
 * @brief       Sends the get scan command to the Wi-Fi module
 * @param[in]   rak_getscan_t *GetScanFrame,Pointer to getscan structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to get scan info.   
 */
uint16_t rak_getscan(rak_getscan_t *GetScanFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_uscan(rak_scan_t *ScanFrame)
 * @brief       Sends the Scan command to the Wi-Fi module
 * @param[in]   rak_scan_t *ScanFrame,Pointer to scan structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to scan for Access Points. 
 */
uint16_t rak_uscan(rak_scan_t *ScanFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_set_psk(rak_psk_t *PskFrame)
 * @brief       Sends the set network psk command to the Wi-Fi module
 * @param[in]   rak_psk_t *PskFrame,Pointer to psk structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to set password.
 */
uint16_t rak_set_psk(rak_psk_t *PskFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_connect(rak_conn_t	*uConnFrame)
 * @brief       Sends the connect command to the Wi-Fi module
 * @param[in]   rak_conn_t	*uConnFrame,Pointer to conn structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to connect for Access Points. 
 */
uint16_t rak_connect(rak_conn_t	*uConnFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_get_version(void)
 * @brief       Sends the get version command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description
 * This API is used to get version.
 */
uint16_t rak_get_version(void);


/*===========================================================================
 *
 * @fn          uint16_t rak_get_net_status(void)
 * @brief       Sends the get net status command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to get network status for Access Points. 
 */
uint16_t rak_get_net_status(void);


/*===========================================================================
 *
 * @fn           uint16_t rak_get_rssi(void)
 * @brief       Sends the get rssi command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to get rssi values.   
 */
uint16_t rak_get_rssi(void);


/*===========================================================================
 *
 * @fn          uint16_t rak_ipconfig_dhcp(uint8_t mode)
 * @brief       Sends the set dhcp command to the Wi-Fi module
 * @param[in]   uint8_t mode
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to set dhcp mode. 
 */
uint16_t rak_ipconfig_dhcp(uint8_t mode);


/*===========================================================================
 *
 * @fn          uint16_t  rak_send_data(uint32_t dest_addr,uint32_t dest_port,uint8_t socket,
					uint16_t len,uint8_t *buf)
 * @brief       Sends the send data command to the Wi-Fi module
 * @param[in]    dest_addr, dest_port, socket,len, *buf
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to send TCP/UDP data using an already opened socket. 
 */
uint16_t rak_send_data(uint32_t dest_addr,uint32_t dest_port,uint8_t socket,uint16_t len,uint8_t *buf);


/*===========================================================================
 *
 * @fn         uint16_t rak_open_socket(uint16 local_Port,uint16 dest_Port,
						uint8 rak_SocketCmd,uint32_t dest_Ip)
 * @brief       Sends the open socket command to the Wi-Fi module
 * @param[in]   local_Port, dest_Port,rak_SocketCmd, dest_Ip
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description
 * This API is used to open socket .
 */
uint16_t rak_open_socket(uint16 local_Port,uint16 dest_Port,uint8 rak_SocketCmd,uint32_t dest_Ip);


/*===========================================================================
 *
 * @fn          uint16_t rak_setpwrmode(uint8_t mode)
 * @brief       Sends the set pwrmode command to the Wi-Fi module
 * @param[in]   uint8_t mode
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to set power mode .  
 */
uint16_t rak_setpwrmode(uint8_t mode);


/*===========================================================================
 *
 * @fn          uint16_t rak_set_channel(rak_channel_t  *uChannelFrame)
 * @brief       Sends the set channel command to the Wi-Fi module
 * @param[in]   rak_channel_t  *uChannelFrame,Pointer to channel structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to set channel. 
 */
uint16_t rak_set_channel(rak_channel_t  *uChannelFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_set_ipstatic(rak_ipstatic_t *IpstaticFrame)
 * @brief       Sends the set ip command to the Wi-Fi module
 * @param[in]   rak_ipstatic_t *IpstaticFrame,Pointer to ipstatic structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to set ip info. 
 */
uint16_t rak_set_ipstatic(rak_ipstatic_t *IpstaticFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_storeconfig_data(rak_param_t *uParamFrame)
 * @brief       Sends the storeconfig data command to the Wi-Fi module
 * @param[in]   rak_param_t *uParamFrame,Pointer to param structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to Save network information.   
 */
uint16_t rak_storeconfig_data(rak_param_t *uParamFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_store_config(void)
 * @brief       Sends the store config command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to Save the current network information.   
 */
uint16_t rak_store_config(void);



/*===========================================================================
 *
 * @fn          uint16_t rak_apconfig(rak_apconfig_t	*uApconfigFrame)
 * @brief       Sends the apconfig command to the Wi-Fi module
 * @param[in]   rak_apconfig_t	*uApconfigFrame,Pointer to apconfig structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to set ap param. 
 *  
 */
uint16_t rak_apconfig(rak_apconfig_t	*uApconfigFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_reset(void)
 * @brief       Sends the reset command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to reset module.  
 */
uint16_t rak_reset(void) ;


/*===========================================================================
 *
 * @fn          uint16_t rak_disconnect(void)
 * @brief       Sends the disconnect command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to disconnect for Access Points.  
 */
uint16_t rak_disconnect(void);


/*===========================================================================
 *
 * @fn          uint16_t rak_ping(rak_ping_t *uPingFrame)
 * @brief       Sends the ping command to the Wi-Fi module
 * @param[in]   rak_ping_t *uPingFrame,Pointer to ping structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to ping hostaddr.  
 */
uint16_t rak_ping(rak_ping_t *uPingFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_query_dns(rak_dns_t *uDnsFrame)
 * @brief       Sends the dns command to the Wi-Fi module
 * @param[in]   rak_dns_t *uDnsFrame,Pointer to dns structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to Resolve the domain name address. 
 */
uint16_t rak_query_dns(rak_dns_t *uDnsFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_socket_close(char flag)
 * @brief       Sends the close socket  command to the Wi-Fi module
 * @param[in]   char flag
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to  close an already open socket. 
 */
uint16_t rak_socket_close(char flag);


/*===========================================================================
 *
 * @fn           uint16_t rak_easy_config(void)
 * @brief       Sends the easy config command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to easy connect for Access Points.
 */
uint16_t rak_easy_config(void);


/*===========================================================================
 *
 * @fn          uint16_t rak_wps(void)
 * @brief       Sends the wps command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to wps connect.   
 */
uint16_t rak_wps(void);


/*===========================================================================
 *
 * @fn          uint16_t rak_auto_connect(void)
 * @brief       Sends the auto connect command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to auto connect.
 */
uint16_t rak_auto_connect(void);


/*===========================================================================
 *
 * @fn          uint16_t rak_store_web(rak_web_t	*uWebFrame)
 * @brief       Sends the store web command to the Wi-Fi module
 * @param[in]   rak_web_t	*uWebFrame,Pointer to web structure
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to Modify webserver information.  
 */
uint16_t rak_store_web(rak_web_t	*uWebFrame);


/*===========================================================================
 *
 * @fn          uint16_t rak_start_web(void)
 * @brief       Sends the start web command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to open websever.   
 */
uint16_t rak_start_web(void);


/*===========================================================================
 *
 * @fn           uint16_t rak_get_storeconfig(void)
 * @brief       Sends the get storeconfig command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to get Network data stored. 
 */
uint16_t rak_get_storeconfig(void);


/*====================================================*/
/**
 * @fn          int16 rak_init_struct(rak_api *ptrStrApi)
 * @brief       Initialize the global parameter structure
 * @param[in]   rak_api *ptrStrApi, pointer to the global parameter structure
 * @param[out]  none
 * @return      status
 *	        0  = SUCCESS
 * @description	This function is used to initialize the global parameter structure with parameters
 * 		used to configure the Wi-Fi module.
 *
 */
int16 rak_init_struct(rak_api *ptrStrApi);


/*===========================================================================
 *
 * @fn          uint16_t rak_ipconfig_query(void)
 * @brief       Sends the get ipconfig command to the Wi-Fi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -1 = SPI busy / Timeout
 *              -2 = Failure
 *              0  = SUCCESS
 * @section description 
 * This API is used to get ip  information.
 */
uint16_t rak_ipconfig_query(void);

#define RAK_TRUE    					1
#define RAK_FALSE						0

//#define RAK_TICKS_PER_SECOND   			500
#define RAK_TICKS_PER_SECOND   			1

#define RAK_EASYCONFIG_TIMEOUT       	30 * RAK_TICKS_PER_SECOND
#define RAK_WPS_TIMEOUT       			30 * RAK_TICKS_PER_SECOND
#define RAK_AUTOCONNECT_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_STARTWEB_TIMEOUT       		100 * RAK_TICKS_PER_SECOND
#define RAK_SETCHANNEL_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_SETPSK_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_APCONFIG_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_CONNECT_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_GETVERSION_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_IPCONFIG_QUERY_TIMEOUT		10 * RAK_TICKS_PER_SECOND
#define RAK_IPSTATIC_TIMEOUT			10 * RAK_TICKS_PER_SECOND
#define RAK_DISCONNECT_TIMEOUT			10 * RAK_TICKS_PER_SECOND
#define RAK_PING_TIMEOUT       			10 * RAK_TICKS_PER_SECOND
#define RAK_DNS_TIMEOUT       			10 * RAK_TICKS_PER_SECOND
#define RAK_SCAN_TIMEOUT       			10 * RAK_TICKS_PER_SECOND
#define RAK_GETSCAN_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_GETNET_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_GETRSSI_TIMEOUT       		1 * RAK_TICKS_PER_SECOND
#define RAK_GETCONFIG_TIMEOUT           10 * RAK_TICKS_PER_SECOND
#define RAK_STORECONFIG_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_STOREWEB_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_DHCP_TIMEOUT       			10 * RAK_TICKS_PER_SECOND
#define RAK_OPENSOCKET_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_SETLISTEN_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_CLOSESOCKET_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_STOREDATA_TIMEOUT       	10 * RAK_TICKS_PER_SECOND


#define RAK_IRQ_DATAPACKET  			0x01
#define RAK_IRQ_RECVFULL				0X02
#define RAK_INC_TIMER_3        			rak_spiTimer3++
#define RAK_RESET_TIMER3       			rak_spiTimer3=0
/*@ Command response timeout */
#define RAK_RESPONSE_TIMEOUT(A)    		RAK_RESET_TIMER3; \
	                                    while (rak_checkPktIrq() != RAK_TRUE) \
                                        {                                  \
	                                        if (RAK_INC_TIMER_3 > A)  \
	                                        {							   \
		                                       retval = 1;				   \
		                                       break;					   \
	                                        }                               \
                                        };                                  \
									    rak_read_packet(&uCmdRspFrame);     \
		                                rak_clearPktIrq();                  

#define HEAD_DELAY   	50
#define TIMER_NUM		100000

extern   volatile 	rak_intStatus		rak_strIntStatus;
extern      rak_api             rak_strapi;
extern      rak_CmdRsp	        uCmdRspFrame;

uint16_t rak_socket(uint16 local_Port,uint16 dest_Port,
						uint8 rak_SocketCmd,uint32_t dest_Ip);
uint16_t  send_data(uint32_t dest_addr,uint32_t dest_port,uint8_t socket,
					uint16_t len,uint8_t *buf);



#endif
