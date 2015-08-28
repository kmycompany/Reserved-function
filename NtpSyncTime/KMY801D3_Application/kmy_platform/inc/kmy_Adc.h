#ifndef _kmy_adc_h
#define _kmy_adc_h

#include "stm32f10x.h"

void kmyAdcInit(void);
//函数功能：ADC 初始化
//函数参数：无
//返回值：无
//注意事项：无

uint16_t kmyGetAdc(void);   
//函数功能：获取采样值
//函数参数：无
//返回值：返回采样值
//注意事项：无

uint16_t kmyGetVoltage(void);
//函数功能：获取采样值
//函数参数：无
//返回值：返回电压值
//注意事项：无

#endif
