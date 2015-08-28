
#ifndef __FOODORDER_H
#define __FOODORDER_H

#define MENU_COL 22
#define AUSTRALIA_PROJECT

#define WiFi_TCPConnectAddressInvalid	0x33
#define WiFi_TCPConnectOK				0x34
#define WiFi_TCPConnectAlready			0x35
#define WiFi_TCPConnectPDPDEACT			0x36
#define WiFi_TCPConnectFail				0x37

//define AT commands

#define AT_TcpStatus						"AT+QISTAT\r"
#define AT_IPconnect						"AT+QIDNSIP=0\r"
#define AT_DNSconnect						"AT+QIDNSIP=1\r"
#define AT_ATE0								"ATE0\r"
#define AT_SearchingForNetworks				"AT+COPS?\r"
#define AT_SignalStrength					"AT+CSQ\r"
#define AT_ADC								"AT+QADC?\r"
#define AT_GetIMEI							"AT+GSN\r"
#define AT_TcpSetApn						"AT+QIREGAPP=\""
#define AT_TcpHeadOff						"AT+QIHEAD=0\r"
#define AT_TcpConnect						"AT+QIOPEN=\"TCP\",\""
#define AT_TcpSend							"AT+QISEND="
#define AT_TcpClose1						"AT+QICLOSE\r"
#define AT_TcpClose2						"AT+QIDEACT\r"


//-----------For order project----------------------------

typedef struct 
{
    char RestaurantID[10];
	char OrderType[5];
	char OrderNumber[20];
	
//	char Reservation[400];
	char *OrderFood;
//	char DeliveryCharge[10];//DeliveryCost
//	char CC_HandelingFees[10];//Credit card fee + Handling fee
    char Discount[10];
	char Total[10];
	
	char CustomerType[5];//IsCustomerVerified
	char CustomerName[30];
	char CustomerAddress[200];
	
	char RequestTime[30];
	char PreviousNumberOfOrders[5];
	char PaymentStatus[5];
//	char PaymentCard[30];
	char CustomerPhone[30];
	char *CustomerComments;
	char Delivery[20];//accepted for
}ORDER_STRUCT;


typedef struct
{
	char CurMenuIndex;
	char OldHighLightRow;
	char CurHighLightRow;
	char MenuScrollCount;
	char MenuLayerCnt;
	char FlashFlag;
} MENUVAR;




char SelfTest_WiFi(char *msg);

char Latest_Orders (char *msg);
char Reject_Orders (char *msg);
char Confirm_Orders (char *msg);

void InitOrder(void);
void kmy_EnableDisableTime3(char OnOFF);//if OnOFF=1, WiFi_Thread(check order) is on; else is off.
void kmy_NetStateSet(void);
char HaveReceiveOrder(void);
char HaveUpgrade(void);

void GetRejectReason(char (*ReasonTable)[MENU_COL], char *Reason, char *RejectNum );
char CheckOrderFailWarn(void);
char CheckTimeFormat(char* buff);

#endif

