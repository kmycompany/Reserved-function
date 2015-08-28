#ifndef __KMY_PRINTERDRV_H
#define __KMY_PRINTERDRV_H

#define Printer_OK							1
#define Printer_NoPaper						2

extern const  char WidthZoom1[4];//字体宽 X 1
extern const  char WidthZoom2[4];//字体宽 X 2
extern const  char WidthZoom3[4];//字体宽 X 3
extern const  char WidthZoom4[4];//字体宽 X 4
extern const  char WidthZoom5[4];//字体宽 X 5

extern const  char HighZoom1[4]; //字体高 X 1
extern const  char HighZoom2[4]; //字体高 X 2
extern const  char HighZoom3[4]; //字体高 X 3
extern const  char HighZoom4[4]; //字体高 X 4
extern const  char HighZoom5[4]; //字体高 X 5

extern const  char Zoom1[4];     //字体 X 1
extern const  char Zoom2[4];     //字体 X 2

/*目前只支持两种字体ASCII码的切换，使用时只需将以下字体命令strcat到打印字符串中即可*/
extern const  char Font1[4];     //字体 16 X 16
extern const  char Font2[4];     //字体 24 X 24

extern const char underlineOFF[4];     //下划线关
extern const char underlineON[4];     //下划线开


void kmy_PrinterInit(unsigned char mode);
//函数功能：打印机初始化
//函数参数：
//   mode:  0   初始化所有打印机参数
//          1   只初始化打印字体的宽、高。
//返回值：  无
//注意事项：无


char kmy_PrinterSetcommand(const char * command,unsigned char len);
//函数功能：打印机设置命令
//函数参数：
//	command:  命令字符串
//	len: 命令长度
//返回值：
//	设置成功返回(宏定义)Printer_OK
//  当设置缺纸检测时: 缺纸返回Printer_NoPaper
//注意事项：无


char kmy_PrinterString(unsigned char *Data);
//函数功能：打印机打印字符串
//函数参数：
//	*Data:  字符串
//返回值：
//	打印完成返回(宏定义)Printer_OK
//	缺纸返回(宏定义)Printer_NoPaper
//注意事项：
//	1. Data缓冲区字符串长度尽量小于2048bytes

char kmy_PrinterBitmap(const unsigned char *p,unsigned int width, unsigned int height,unsigned int LeftMarginNum);
//函数功能：打印机打印位图
//函数参数：
//	p:  位图数据缓冲区首地址
//	width:  位图宽度
//	height:  位图高度
//	LeftMarginNum:  位图从左边哪点开始打起
//返回值：
//	打印完成返回(宏定义)Printer_OK
//	缺纸返回(宏定义)Printer_NoPaper
//注意事项：
//	位图格式为:单色点阵液晶字模，纵向取模，字节倒序
//	LeftMarginNum+width的值应该小于384；
//	位图最大的宽度为384


char PrintBarCode(char *Databuff);
//函数功能：打印机打印条码(只支持打印code128码)
//函数参数：
//	*Databuff:  条码字符
//返回值：
//	打印完成返回(宏定义)Printer_OK
//	缺纸返回(宏定义)Printer_NoPaper
//注意事项：
//	1. 受打印宽度的影响，Databuff缓冲区字符串长度不能大于10个字符。

#endif

