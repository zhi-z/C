
/*******************************************************************************
* �ļ��� : ILI9325.c

* �汾      ����            ����            ˵��
* V1.0      zhi-z           2015/07       ��ʼ�汾

* ����   : MCU: STC15F2K60S2     ����: 22.1184   MHz
           
*******************************************************************************/

#include "ILI9325.h"

/*******************************************************************************
* �������� : TFT��ʼ��
* �������� : �ⲿ
* ������� : ��
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void ILI9325_initial( )
{ 
	TFT_CS=1;
	TFT_delayms(5);
	TFT_RST=0;
	TFT_delayms(5);
	TFT_RST=1;
	TFT_delayms(50);//�����ʵ�������ʱ�������ȶ���ʾ
	TFT_write_cmd_data(0x0001,0x0100); //Դɨ������SS=0, ������ƶ������Ǵ�S1��S
	                                   //720
	                                   
	//��Դɨ������SS=1, ������ƶ������Ǵ�S720��S1��
	//SM=0������ɨ�裨SM=1������ɨ�裩

	TFT_write_cmd_data(0x0002,0x0700); //B/C=1��EOR=1���е��ã�B/C=0��֡/�����ã�
	TFT_write_cmd_data(0x0003,0x1030); //AM=0����ַ��ˮƽ�����Զ���1
	//I/D[1:0] = 11��ˮƽ��ֱ���������
	//BGR=1:����BGR��ʽ
	//TRI=0��DFM=*��8λ�����������ݴ���
	TFT_write_cmd_data(0x0004,0x0000); //�����������ã�������
	
	//��ʾ����
	TFT_write_cmd_data(0x0008,0x0207); //���ú���غ�ǰ�� 
	TFT_write_cmd_data(0x0009,0x0000);//���÷���ʾ��ʱ����ISC[3:0]
	TFT_write_cmd_data(0x000A,0x0000); //֡��ǹ���

	TFT_write_cmd_data(0x000C,0x0000);//RGB��ʾ�ӿڿ���1
	TFT_write_cmd_data(0x000D,0x0000);//֡���λ��
	TFT_write_cmd_data(0x000F,0x0000);//RGB��ʾ�ӿڿ���2
	
	//��Դ����
	TFT_write_cmd_data(0x0010,0x0000);   
	TFT_write_cmd_data(0x0011,0x0007);  
	TFT_write_cmd_data(0x0012,0x0000);  
	TFT_write_cmd_data(0x0013,0x0000); 
	//vgh 
	TFT_write_cmd_data(0x0010,0x1290);   
	TFT_write_cmd_data(0x0011,0x0227);
	//delayms(100);
	//vregiout 
	TFT_write_cmd_data(0x0012,0x001d); //0x001b
	//delayms(100); 
	//vom amplitude
	TFT_write_cmd_data(0x0013,0x1500);
	//delayms(100); 
	//vom H
	TFT_write_cmd_data(0x0029,0x0018); 
	TFT_write_cmd_data(0x002B,0x000D); 
	
	//٤��У��
	TFT_write_cmd_data(0x0030,0x0004);
	TFT_write_cmd_data(0x0031,0x0307);
	TFT_write_cmd_data(0x0032,0x0002);//0006
	TFT_write_cmd_data(0x0035,0x0206);
	TFT_write_cmd_data(0x0036,0x0408);
	TFT_write_cmd_data(0x0037,0x0507); 
	TFT_write_cmd_data(0x0038,0x0204);//0200
	TFT_write_cmd_data(0x0039,0x0707); 
	TFT_write_cmd_data(0x003C,0x0405);//0504
	TFT_write_cmd_data(0x003D,0x0F02); 
	
	//��������
	TFT_write_cmd_data(0x0050,0x0000);//ˮƽ��ʼλ��
	TFT_write_cmd_data(0x0051,0x00EF);//ˮƽ��ֹλ��
	TFT_write_cmd_data(0x0052,0x0000);//��ֱ��ʼλ��
	TFT_write_cmd_data(0x0053,0x013F);//��ֱ��ֹλ��

	TFT_write_cmd_data(0x0060,0xA700);//��ɨ�����ã�GS=1����G320ɨ�赽G1��320��
	TFT_write_cmd_data(0x0061,0x0001); 
	TFT_write_cmd_data(0x006A,0x0000);
	//
	TFT_write_cmd_data(0x0080,0x0000); 
	TFT_write_cmd_data(0x0081,0x0000); 
	TFT_write_cmd_data(0x0082,0x0000); 
	TFT_write_cmd_data(0x0083,0x0000); 
	TFT_write_cmd_data(0x0084,0x0000); 
	TFT_write_cmd_data(0x0085,0x0000); 
	//
	TFT_write_cmd_data(0x0090,0x0010); 
	TFT_write_cmd_data(0x0092,0x0600); 
	TFT_write_cmd_data(0x0093,0x0003); 
	TFT_write_cmd_data(0x0095,0x0110); 
	TFT_write_cmd_data(0x0097,0x0000); 
	TFT_write_cmd_data(0x0098,0x0000);
	TFT_write_cmd_data(0x0007,0x0133);	
}


/*******************************************************************************
* �������� : TFTд����
* �������� : �ⲿ
* ������� : CH ����ĸ��ֽ�, CL ����ĵ��ֽ�
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void TFT_write_cmd(unsigned char CH,unsigned char CL)
{
	TFT_CS=0;
	TFT_RS=0;
	TFT_DATA=CH;
	TFT_WR=0;
	TFT_WR=1;
	TFT_DATA=CL;
	TFT_WR=0;
	TFT_WR=1;
	TFT_CS=1;
}


/*******************************************************************************
* �������� : TFTд�����ֽڵ�����
* �������� : �ⲿ
* ������� : DH,DL�ֱ�Ϊ�����ֵĸ�8λ�͵�8λ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void TFT_write_data(unsigned char DH,unsigned char DL)
{
	TFT_CS=0;
	TFT_RS=1;
	TFT_DATA=DH;
	TFT_WR=0;
	TFT_WR=1;
	TFT_DATA=DL;	
	TFT_WR=0;
	TFT_WR=1;
	TFT_CS=1;
}


/*******************************************************************************
* �������� : TFTд�����ֽڵ�����(16λ����)
* �������� : �ⲿ
* ������� : dat,Ҫд���16λ������
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void TFT_write_data_u16(unsigned int dat)
{
	TFT_write_data(dat>>8,dat);
}


/*******************************************************************************
* �������� : TFTд�����д����
* �������� : �ⲿ
* ������� : cmd,һ���ֽڵ�����;  dat,һ���ֽڵ�����
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void TFT_write_cmd_data (unsigned char cmd,unsigned int dat)
{
	TFT_write_cmd(0x00,cmd);
	TFT_write_data(dat>>8,dat);
}

/*******************************************************************************
* �������� : TFT���ô���,�趨������ʾ�Ĵ��ڵ�λ�úʹ�С
* �������� : �ⲿ
* ������� : x0 ˮƽ��ʼλ��; x1 ˮƽ��ֹλ��; y0 ��ֱ��ʼλ��; y1 ��ֱ��ֹλ��
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void TFT_setwindow(unsigned char x0,unsigned int y0,unsigned char x1,unsigned int y1)
{
	TFT_write_cmd_data(WINDOW_XADDR_START,x0);//ˮƽ��ʼλ��
	TFT_write_cmd_data(WINDOW_XADDR_END,x1);//ˮƽ��ֹλ��
	TFT_write_cmd_data(WINDOW_YADDR_START,y0);//��ֱ��ʼλ��
	TFT_write_cmd_data(WINDOW_YADDR_END,y1);//��ֱ��ֹλ��
	TFT_write_cmd_data(GRAM_XADDR,x0);//����X����λ��
	TFT_write_cmd_data(GRAM_YADDR,y0);//����Y����λ��
	TFT_write_cmd(0x00,0x22);//LCD_WriteCMD(GRAMWR);
}


/*******************************************************************************
* �������� : TFT�趨Ҫ��ʾ�ĵ������,�趨Ҫ��ʾ��һ�����ص��λ��
* �������� : �ⲿ
* ������� : x ���ص�ĺ�����; y ���ص�������� 
* ���ز��� : ��
* ������� : ������ʵ������LCD_setwindow������һ����������ʾ����Ϊ��С������һ��
             ���ص㡣
*******************************************************************************/

void TFT_setxy(unsigned char x,unsigned int y)
{
	TFT_setwindow(x,y,x,y);	
}

/*******************************************************************************
* �������� : ��������ɫ�Բ����,��Ҫ����ʱ��ȥ��ע��
* �������� : �ⲿ
* ������� : ��
* ���ز��� : ��
* ������� : ��TFT����������ʾ�����͸�����ɫ�ĵ�ɫ��Ļ��
*******************************************************************************/

///*��ɫ��ֵ:��ɫ����ɫ����ɫ����ɫ����ɫ����ɫ������ɫ����ɫ*/
//U16 code color[]={0xf800,0x07e0,0x001f,0xffe0,0x0000,0xffff,0x07ff,0xf81f};

//void TFT_test()
//{
//	U16 temp,num;
//	U8 n,c;

//	//��ʾ����	
//	TFT_setwindow(0,0,239,319);
//	for(n=0;n<8;n++)
//	{
//		temp=color[n];										
//		for(num=40*240;num>0;num--)
//		TFT_write_data_u16(temp);
//	}
//	TFT_delayms(1000);

//	//��ʾ��ɫ��ɫ
//	for(c=0;c<8;c++)
//	{
//		temp = color[c];
//		TFT_setwindow(0,0,239,319);
//		for(n=0;n<240;n++)
//		{
//			for(num=0;num<320;num++)
//			TFT_write_data_u16(temp);
//		}
//		TFT_delayms(1000);
//	}
//}

/*******************************************************************************
* �������� : ������ʱ����
* �������� : �ⲿ
* ������� : z,��ʱ����
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void TFT_delayms(unsigned int z)
{
	unsigned int i , j ;
	for (i = z ;i > 0 ; i--)
		for(j = 1700 ; j > 0 ; j-- );
}