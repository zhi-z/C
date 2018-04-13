
/*******************************************************************************
* �ļ��� : 12864.h

* �汾     ����            ����            ˵��
* V1.0     zhi-z        2014/08/04       ��ʼ�汾

* ����   : MCU: STC15F2K60S2     ����: 22.1184   MHz
           
*******************************************************************************/



#ifndef __12864_H__
#define __12864_H__

#include "STC15F2K60S2.h"

//��һ����ʾ������	 ��ַ0x80��0x87
//�ڶ�����ʾ������	 ��ַ0x90��0x97
//��������ʾ������	 ��ַ0x88��0x8f
//��������ʾ������	 ��ַ0x98��0x9f

extern void LCD_12864_INIT() ;       //Һ����ʼ������
extern void LCD_12864_write_POS(unsigned char x , unsigned char y );//дλ�ú���
extern void close_LCD_12864_back_light(); //�ر�Һ������
extern void open_LCD_12864_back_light();   //��Һ������
extern void show_Typeface(unsigned char x,unsigned char y,unsigned char *p); //��ʾ���� �ַ�
							// x ,y Ϊ���� �ַ�����,������ ,pΪ�ַ���ָ��

void LCD_12864_write_dat(unsigned char dat);   //д���ݺ���
void LCD_12864_write_cmd(unsigned char cmd);   //дָ���
void LCD_12864_delay(unsigned int z);  	//��ʱ����	

sbit LCD_12864_RS = P2^2 ;  		//  ָ��/����ѡ���
sbit LCD_12864_RW = P2^1 ;  		//  ��/дѡ���
sbit LCD_12864_EN = P2^0 ;		//	ʹ�ܶ�
//sbit LCD_12864_PSB = P2^3 ;		//  ��/����ѡ���	
sbit LCD_12864_back_light  = P2^7; 	//  �������	

#endif /*__12864_H__*/


 
