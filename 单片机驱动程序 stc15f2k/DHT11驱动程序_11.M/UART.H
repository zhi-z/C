
/*******************************************************************************
* 文件名 : UART.H

* 版本     作者            日期            说明
* V1.1                 2015/08/03       初始版本

* 描述   : MCU:      晶振:    MHz
           
*******************************************************************************/

#ifndef __UART_H__
#define __UART_H__

#include <string.h>
#include <stdio.h>

//UART 相关函数------------------------

extern void UartInit(void); //51单片机的串口初始化

extern void UART_Send_Byte(unsigned char mydata); //串口发送一个字节的函数

extern void UART_Send_Str(char *s);  		//串口发送字符串
extern void UART_Put_Num(unsigned long dat); //串口发送数值,以10进制的形式显示,可用于观察变量的值
//extern void UART_Put_Hex(unsigned int hex);  //串口发送数值,以16进制的形式显示,可用于观察变量的值
	
extern void UART_Send_Enter();                          //串口发送0d和0a两个字节,即回车换行
//extern void UART_Put_Inf(char *inf,unsigned long dat);//单片机的串口发送调试信息,
//														//示例 :UART_Put_Inf("i= ",i);  

extern void u32_to_str(unsigned long dat,char *str); //将一个32位的变量dat转为字符串，比如把1234转为"1234"

//extern unsigned long str_to_u32(char *str);   //将一个字符串转为32位的变量，比如"1234"转为1234

//extern unsigned char Hex2Str_16b(unsigned int hex,char *str);  //将数值转换为字符串,如 15 转为 "000F"



#endif /*__UART_H__*/


