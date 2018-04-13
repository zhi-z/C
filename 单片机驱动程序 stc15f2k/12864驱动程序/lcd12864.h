/*******************************************************************************
* �ļ��� : lcd12864.h
* ʵ�ֹ���:12864��������
* �汾     ����            ����            ˵��
* V1.0     zhi-z            2015/08/30       ��ʼ�汾

* ����   : MCU: STC15F2K60S2     ����:22.1184    MHz
           
*******************************************************************************/  
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


void write_com(unsigned char com);		//д����
void write_dat(unsigned char dat);		//д����
void lcd_init();						//lcd��ʼ��
void display(byte x,byte y, byte * p);	//��ʾ����,x,yΪ����
void place(byte x,byte y);				//��ʾλ��



#endif