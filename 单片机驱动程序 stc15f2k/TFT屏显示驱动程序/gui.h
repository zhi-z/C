/*******************************************************************************
* �ļ��� : ILI9325.h

* �汾     ����            ����            ˵��
* V1.0     zhi-z           2015/07       ��ʼ�汾

* ����   : MCU: STC15F2K60S2     ����: 22.1184   MHz
           
*******************************************************************************/

#ifndef _gui_h_
#define _gui_h

#include "ILI9325.h"
#include "bmp.h"
#include "16x8.h"


/*���峣����ɫ����ɫ��*/
#define RED 0xf800	//��
#define YELLOW 0xffe0	//��
#define GREEN 0x07e0	//��
#define CYAN 0x07ff	//��
#define BLUE 0x001f	//��
#define PURPLE 0xf81f	//��
#define BLACK 0x0000	//��
#define WHITE 0xffff	//��
#define GRAY 0x7bef	//��

//------------------------- GUI��TFT�Ľӿں����궨�� ---------------------------
#define GUI_write_data       TFT_write_data		//�����β�,д�������ֽڵ�����
#define GUI_write_data_u16   TFT_write_data_u16			//д16λ����
#define GUI_setxy            TFT_setxy   			 //�趨һ�����ص������
#define GUI_setwindow        TFT_setwindow			 //���ô���
//------------------------------------------------------------------------------

//extern void Avoid_warning(); //����һ��,������־���
extern unsigned int RGB(unsigned char R,unsigned char G,unsigned char B);//RGB��ɫ���
extern void GUI_clearscreen(unsigned int b_color);//����
extern void GUI_Point(unsigned char x, unsigned int y, unsigned int color);//����
extern void GUI_HLine(unsigned char x, unsigned int y, unsigned char length, unsigned int color);//������
extern void GUI_RLine(unsigned char x, unsigned int y, unsigned int high, unsigned int color);//������
extern void GUI_line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);//��ֱ��
extern void GUI_box(unsigned char sx,unsigned int sy,unsigned char ex,unsigned int ey,unsigned int color);//��ʵ�ľ���
extern void GUI_rectangle( unsigned char sx,unsigned int sy,unsigned char ex,unsigned int ey,unsigned int color);//�����ľ���
extern void GUI_DisPicture(unsigned char x, unsigned int y, unsigned char length, unsigned int high,unsigned char *pic);//��ʾͼƬ

//��ʾ1λ16������
extern void GUI_sprintf_nu(unsigned char x, unsigned int y,unsigned char dat, unsigned int color,unsigned int b_color);

//�޷����ַ������Զ�������ʽ��ʾ
extern void GUI_sprintf_char_to_bit(unsigned char x, unsigned int y,unsigned char dat,unsigned int color,unsigned int b_color);

//�޷����ַ�������ʮ������ʽ��ʾ
extern void GUI_sprintf_char_to_dec(unsigned char x,unsigned int y,unsigned char dat,unsigned int color,unsigned int b_color);

//�޷����ַ�������ʮ��������ʽ��ʾ
extern void GUI_sprintf_char_to_hex(unsigned char x, unsigned int y,unsigned char dat, unsigned int color,unsigned int b_color);

//��ʾӢ�Ļ������ַ�
extern void GUI_sprintf_char(unsigned char x, unsigned int y,unsigned char c, unsigned int color,unsigned int b_color);

//��ʾӢ���ַ���
extern void GUI_sprintf_string(unsigned char x, unsigned int y,unsigned char code *s, unsigned int color,unsigned int b_color);

//��ʾԤ���庺���ַ���
extern void GUI_sprintf_HZ(unsigned char x,unsigned int y,unsigned char w,unsigned int h,unsigned char *s,unsigned int color,unsigned int b_color);

//��ʮ������ʽ��ʾ�޷��ų�������,//(x��y)����ʾ���ݵ����Ͻ�����
extern void GUI_sprintf_U32_to_dec(unsigned char x, unsigned int y, unsigned long int dat, unsigned int color,unsigned int b_color);


#endif













