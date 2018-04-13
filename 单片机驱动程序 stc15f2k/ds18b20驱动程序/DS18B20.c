/*******************************************************************************
* �ļ��� : ds12864.c
* ʵ�ֹ���:12864���Գ���
* �汾     ����            ����            ˵��
* V1.0     zhi-z            2015/08/30       ��ʼ�汾

* ����   : MCU: STC15F2K60S2     ����: 22.1184   MHz
           
*******************************************************************************/

#include <STC15F2K60S2.H>
#include"DS18B20.H"
#include"intrins.h"
#include"stdio.h"
unsigned char TempTH=0;
unsigned char TempTL=0;

/*******************************************************************************
* �������� : ��ʱ����
* ����ģ�� : �ڲ�
* ������� : ��
* ���ز��� : 
*******************************************************************************/
static void Delay(unsigned int i)
{
	unsigned int j,k;
	for(j=i;j>0;j--)
		for(k=1600;k>0;k--);
}
void Delay650us()		//@22.1184MHz
{
	unsigned char i, j;

	_nop_();
	i = 14;
	j = 249;
	do
	{
		while (--j);
	} while (--i);
}
void Delay45us()		//@22.1184MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 1;
	j = 244;
	do
	{
		while (--j);
	} while (--i);
}
void Delay480us()		//@22.1184MHz
{
	unsigned char i, j;

	_nop_();
	i = 11;
	j = 80;
	do
	{
		while (--j);
	} while (--i);
}
void Delay2us()		//@22.1184MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 8;
	while (--i);
}
void Delay15us()		//@22.1184MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 80;
	while (--i);
}
void Delay60us()		//@22.1184MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 2;
	j = 70;
	do
	{
		while (--j);
	} while (--i);
}
void Delay10us()		//@22.1184MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 52;
	while (--i);
}

/*******************************************************************************
* �������� : ��ʼ��
* ����ģ�� : �ڲ�
* ������� : ��
* ���ز��� :  Flag, �����ж�
*******************************************************************************/

bit InitDs18b20(void)
{

	bit Flag;

	DQ = 1;//���ͷ�����������ʱ
	Delay(2);
	
	DQ = 0;//�������͸�λ����
	Delay650us();//��������480us
	
	DQ = 1;//�ͷ�����
	Delay60us();//��ʱ�ȴ�Ӧ������
	Flag=DQ;
	if(!Flag)//�������Ӧ����������ʱ480us
	{
		Delay480us();
	}
	return Flag;
}
/*******************************************************************************
* �������� : д���ݡ���д��λ��д��λ
* ����ģ�� : �ڲ�
* ������� : dat
* ���ز��� : 
*******************************************************************************/

void WriteData(unsigned char dat)
{
	unsigned char variable;
	int i=0;
	for(i=0;i<8;i++)
	{
		variable=dat&0x01;
		if(variable)//д1
		{
			DQ = 0;//����������������2us
			Delay2us();
			DQ = 1;//�������ߵȴ�DS18B20����
			Delay60us();
		}
		else//д0
		{
			DQ = 0;//����������������60us
			Delay60us();
			DQ = 1;//�ͷ�����
			Delay2us();
		}
		dat=dat>>1;
	}
}

/*******************************************************************************
* �������� : �����ݡ�
* ����ģ�� : �ڲ�
* ������� : ��
* ���ز��� : dat 
*******************************************************************************/
unsigned char ReadData(void)
{
	unsigned char dat=0;
	int i=0;
	for(i=0;i<8;i++)
	{	
		dat=dat>>1;
		DQ = 0;
		Delay2us();
		DQ = 1;
		Delay10us();
		if(DQ)
		{
			dat=dat|0x80;
		}
		Delay60us();
	}
	return dat;
}

/*******************************************************************************
* �������� : ���¶ȡ�
* ����ģ�� : �ڲ�
* ������� : ��
* ���ز��� :  �� 
*******************************************************************************/
void Temperature(void)
{	
	if(!InitDs18b20())
	{
		WriteData(0xCC);//����ROM����
		WriteData(0x44);//�����¶�ת��
		Delay(500);			//��ʱ�ȴ��¶�ת�����
	}
	if(!InitDs18b20())
	{
		WriteData(0xCC);//����ROM����
		WriteData(0xBE);//���¶�
	}
	TempTL=ReadData();//���¶ȵ�8λ
	TempTH=ReadData();//���¶ȸ�8λ
	
}

/*******************************************************************************
* �������� : ��ʾ������
* ����ģ�� : �ڲ�
* ������� : ��
* ���ز��� : �� 
*******************************************************************************/
void DisplayTemp(void)
{
	int Temp;
	float Temp1;
	Temperature();
	Temp=(((unsigned int)TempTH)<<8)|TempTL;
	if(TempTH&0Xf8)
	{
		Temp=~Temp;
		Temp=Temp+1;
	}
	Temp=Temp&0x07ff;
	Temp1=(float)Temp*0.0625;
	printf("%f\n",Temp1);


}
