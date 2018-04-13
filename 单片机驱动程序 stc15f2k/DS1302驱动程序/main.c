/*******************************************************************************
* �ļ��� : main.c
* ʵ�ֹ���:DS1302���Գ���
* �汾     ����            ����            ˵��
* V1.0     zhi-z           2015/08/30       ��ʼ�汾

* ����   : MCU: STC15F2K60S2     ����:22.1184    MHz
          ������ʾ����ʼʱ���Լ��� 
*******************************************************************************/  

#include"STC15F2K60S2.h"
#include"DS1302.h"
#include"intrins.h"
#include "uart.h"

unsigned char CurrentTime[16];//��ǰʱ��洢����
extern unsigned char Time_buf[8];//ʱ�仺������

unsigned char sec_buf = 0;	//����������һ�ε�����
unsigned char time=0;		//�л������õı���



/*******************************************************************************
* �������� : ������
* �������� : 
* ������� : ��
* ���ز��� : ��
* ������� : 
*******************************************************************************/
void main()
{
	DS1302_Init();			//DS1302��ʼ��
	DS1302_Write_Time();	//��ʼ������
	UartInit();				//���ڳ�ʼ��
	while(1)
	{
		DS1302_Read_Time();
		//��
		CurrentTime[0] = Time_buf[0]>>4;	//�õ�ǧλ
		CurrentTime[1] = Time_buf[0]&0x0F;	//�õ���λ
		CurrentTime[2] = Time_buf[7]>>4;	//�õ�ʮλ
		CurrentTime[3] = Time_buf[7]&0x0F;	//�õ���λ
		
		//��
		CurrentTime[4] = Time_buf[5]>>4;	//�õ�ʮλ
		CurrentTime[5] = Time_buf[5]&0x0F;	//�õ���λ
		
		//��
		CurrentTime[6] = Time_buf[4]>>4;	//�õ�ʮλ
		CurrentTime[7] = Time_buf[4]&0x0F;	//�õ���λ
		
		//ʱ
		CurrentTime[8] = Time_buf[3]>>4;	//�õ�ʮλ
		CurrentTime[9] = Time_buf[3]&0x0F;	//�õ���λ
		
		//��
		CurrentTime[10] = Time_buf[2]>>4;	//�õ�ʮλ
		CurrentTime[11] = Time_buf[2]&0x0F;	//�õ���λ
		
		//��
		CurrentTime[12] = Time_buf[1]>>4;	//�õ�ʮλ
		CurrentTime[13] = Time_buf[1]&0x0F;	//�õ���λ
		
		//����
		CurrentTime[15] = Time_buf[6];		//�õ���λ

		if(CurrentTime[13]!=sec_buf)		//��ǰʱ����ԭ��ʱ�䲻ͬ      
		{    
			sec_buf=CurrentTime[13];//��
			
/*******************��**********************************************/
			UART_Send_Byte(CurrentTime[0]+'0');
			UART_Send_Byte(CurrentTime[1]+'0');
			UART_Send_Byte(CurrentTime[2]+'0');
			UART_Send_Byte(CurrentTime[3]+'0');
			UART_Send_Str("��");
			UART_Send_Str("/");
/*******************��********************************************/
			UART_Send_Byte(CurrentTime[4]+'0');
			UART_Send_Byte(CurrentTime[5]+'0');
			UART_Send_Str("��");
			UART_Send_Str("/");
/*******************��*********************************************/
			UART_Send_Byte(CurrentTime[6]+'0');
			UART_Send_Byte(CurrentTime[7]+'0');
			UART_Send_Str("��");
			UART_Send_Str("\n");
/*******************����*********************************************/
			UART_Send_Str("����");
			UART_Send_Byte(CurrentTime[15]+'0');
			UART_Send_Str("\n");
/*******************ʱ************************************************/
			UART_Send_Byte(CurrentTime[8]+'0');
			UART_Send_Byte(CurrentTime[9]+'0');
			UART_Send_Str(":");
/*******************��************************************************/
			UART_Send_Byte(CurrentTime[10]+'0');
			UART_Send_Byte(CurrentTime[11]+'0');
			UART_Send_Str(":");
/*******************��************************************************/
			UART_Send_Byte(CurrentTime[12]+'0');
			UART_Send_Byte(CurrentTime[13]+'0');
			UART_Send_Str("\n");

			time = 0;
		}
	}
}
