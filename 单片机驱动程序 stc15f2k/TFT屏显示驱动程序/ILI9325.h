
/*******************************************************************************
* �ļ��� : ILI9325.h

* �汾     ����            ����            ˵��
* V1.0                  2015/07       ��ʼ�汾

* ����   : MCU: STC15F2K60S2     ����: 22.1184   MHz
           
*******************************************************************************/


#ifndef __ILI9325_H__
#define __ILI9325_H__

#include <STC15F2K60S2.H>

//#define  U8 unsigned char 
//#define  U16 unsigned int

//============================================================
//����оƬ���϶���
#define WINDOW_XADDR_START	0x0050 // Horizontal Start Address Set
#define WINDOW_XADDR_END	0x0051 // Horizontal End Address Set
#define WINDOW_YADDR_START	0x0052 // Vertical Start Address Set
#define WINDOW_YADDR_END	0x0053 // Vertical End Address Set
#define GRAM_XADDR		0x0020 // GRAM Horizontal Address Set
#define GRAM_YADDR		0x0021 // GRAM Vertical Address Set
#define GRAMWR 			0x0022 // memory write
#define GUI_LCM_YMAX 319
#define GUI_LCM_XMAX 239 

sbit TFT_RS=P2^2;	//����/����ѡ��
sbit TFT_WR=P2^1;	//д
sbit TFT_RD=P2^0;	//��
sbit TFT_CS=P2^6;	//Ƭѡ
sbit TFT_RST=P2^7;	//��λ

#define TFT_DATA P0	//���ݿ�

extern void ILI9325_initial( );//TFT��ʼ��
extern void TFT_write_cmd(unsigned char CH,unsigned char CL);//д����
extern void TFT_write_data(unsigned char DH,unsigned char DL);//д�����ֽ�����
extern void TFT_write_data_u16(unsigned int dat);//д16λ����,ͨ������TFT_write_dataʵ��
extern void TFT_write_cmd_data (unsigned char cmd,unsigned int dat);//д��������
extern void TFT_setxy(unsigned char x,unsigned int y);//�趨��ǰ��ʾ����
extern void TFT_delayms(unsigned int z);  //������ʱ����
extern void TFT_setwindow(unsigned char x0,unsigned int y0,unsigned char x1,unsigned int y1);//���ô���
extern void TFT_test();//��������ɫ�Բ����
                          
#endif /*__ILI9325_H__*/














