/*********************************************************************************
* �ļ���     :  1602.h
* ����       :  1602�ӿ�����ͷ�ļ�
 
* �汾     ����            ����            ˵��
* V1.0      zhi-z       2015/08/5     ��ʼ�汾
*********************************************************************************/

#ifndef __1602_H__
#define __1602_H__

#include <stc15f2k60s2.H>

extern void delay (unsigned int z);
extern void init();


void write_com (char com);
void write_data (char date)	;
sbit rs = P2^2 ;			//д�������ѡ���
sbit rw = P2^1 ;		//��д���ƶ�
sbit cs = P2^0 ;		//����ʹ�ܶ�

extern void display();


#endif /*__1602_H__*/


 


