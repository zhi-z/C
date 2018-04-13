/*******************************************************************************
* �ļ��� : lcd12864.h

* �汾     ����            ����            ˵��
* V1.1     zhi-z        2014/08/03       ��ʼ�汾

* ����   : MCU:stc15f2k60s2      ����:22.1184   MHz
           
******************************************************************************/
#ifndef __lcd12864_H_
#define __lcd12864_H_

#include<STC15F2K60S2.h>


#define byte unsigned char
#define uint unsigned int
#define port P0

sbit lcd_rs = P2^2;
sbit lcd_rw = P2^1;
sbit lcd_e = P2^0; 
sbit lcd_12864_psb = P2^3;

void lcd_delay1ms(unsigned int x);		   //��ʱ����
void write_com(unsigned char com);		   //д�����
void write_dat(unsigned char dat);		   //д���ݺ���
void lcd_init();						   //��ʼ��
void display_12864(byte x,byte y, byte * p); //��ʾλ��
void place(byte x,byte y);					 //��ʾ

#endif