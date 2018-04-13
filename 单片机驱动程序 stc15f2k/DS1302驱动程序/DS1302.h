/*******************************************************************************
* �ļ��� : DS1302.h
* ʵ�ֹ���:
* �汾     ����            ����            ˵��
* V1.0     zhi-z            2015/08/30       ��ʼ�汾

* ����   : MCU: STC15F2K60S2     ����:22.1184    MHz
           
*******************************************************************************/  

#ifndef __DS1302_H__
#define __DS1302_H__

//sbit SCLK =  P1^1;//����ʱ������˿�
//sbit DAT  =  P1^2;//����IO�˿�
//sbit RST  =  P1^3;//��λ�����

sbit SCLK =  P4^4;//����ʱ������˿�
sbit DAT  =  P4^2;//����IO�˿�
sbit RST  =  P4^1;//��λ�����



#define		DS1302_SEC_ADD			0x80//��
#define		DS1302_MIN_ADD			0x82//��
#define		DS1302_HR_ADD			0x84//ʱ
#define		DS1302_DAY_ADD			0x86//��
#define		DS1302_MONTH_ADD		0x88//��
#define		DS1302_WEEK_ADD			0x8A//����
#define		DS1302_YEAR_ADD			0x8C//��

#define		DS1302_CONTROL_ADD		0x8E//����
#define		DS1302_CHARGER_ADD		0x90//������					 
#define		DS1302_CLKBURST_ADD		0xBE//ʱ�Ӷ��ֽ�



void DS1302_Init();													//��ʼ��DS1302

void DS1302_Data_Input(unsigned char Address,unsigned char Data);	//��������
unsigned char DS1302_Data_Output(unsigned char Address);			//�������

void DS1302_Write_Time(void);										//д���ʼ��ʱ��
void DS1302_Read_Time(void);										//��ȡʱ��

#endif