/*******************************************************************************
* �ļ��� : DHT11.H

* �汾     ����            ����            ˵��
* V1.1      zhi-z        2014/08/03       ��ʼ�汾

* ����   : MCU:stc15f2k60s2      ����:22.1184    MHz
           
******************************************************************************/
# ifndef _DHT11_H_
# define _DHT11_H_

# include<STC15F2K60S2.h>
# include "intrins.h"
# include"DHT11.h"
# include "lcd12864.h"

sbit DHT11_DATA = P1^7;

unsigned char DHT11_DHT11_Read();	  //DHT11 ����ʪ�Ⱥ���
unsigned char DHT11_Read_Byte();	  //DHT11���ֽں���
void Delay20ms();					  //��ʱ����   20ms
void display();						  // ��ʾ��ʪ�Ⱥ���

# endif