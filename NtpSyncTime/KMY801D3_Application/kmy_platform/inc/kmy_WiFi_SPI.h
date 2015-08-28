#ifndef _KMY_WIFI_SPI_H_
#define _KMY_WIFI_SPI_H_

#include "stm32f10x.h"


#define  int32 					   int32_t
#define  uint32 				   uint32_t
#define  int16 					   int16_t
#define  uint16 				   uint16_t
#define  int8 		  			   int8_t
#define  uint8 					   uint8_t


void kmy_WiFi_OnOff(unsigned char OnOff);
void kmy_Wifi_SPI_Init(void);
void Reset_Target(void);

//uint8_t ReadINTPin(void);

int      spi_Read(void);
uint8_t  spi_send(uint8_t tx_data);
uint16_t get_status(void);
uint16_t get_DataLen(void);
void     spi_recv(char *re_data, uint16_t len);
uint32_t spi_send_head(uint8_t mode, uint16_t len);
void     spi_recv_data(char *rx_buf, uint16_t len);
int      RAK_SendCMD(uint8_t cmd,void* buf, uint16_t len);

#endif
