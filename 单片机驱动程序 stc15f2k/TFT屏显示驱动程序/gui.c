
/*******************************************************************************
* �ļ��� : gui.c

* �汾     ����            ����            ˵��
* V1.0            2015/07       ��ʼ�汾

* ����   : MCU: STC15F2K60S2     ����: 22.1184   MHz
           
*******************************************************************************/

#include "gui.h"


/*******************************************************************************
* �������� : TFT����
* �������� : �ⲿ
* ������� : b_color�Ǳ�����ɫ
* ���ز��� : ��
* ������� : ʹ�ñ�����ɫ���TFTģ����Ļ��ȫ����ʾ����
*******************************************************************************/
void GUI_clearscreen(unsigned int b_color)
{
	unsigned int i,j;
	GUI_setwindow(0,0,240,320);
	for (i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			GUI_write_data_u16(b_color);
		}
			
	}
}
/*******************************************************************************
* �������� : ���㺯��
* �������� : �ⲿ
* ������� : (x y) �������,color �ǵ����ɫ 
* ���ز��� : ��
* ������� : ��ָ������ɫ��ָ��������λ���ϻ���һ����
*******************************************************************************/

void GUI_Point(unsigned char x, unsigned int y, unsigned int color)
{  
	GUI_setxy(x,y);
	GUI_write_data_u16(color);
}

/*******************************************************************************
* �������� : ������
* �������� : �ⲿ
* ������� : (x��y)�Ǻ���������꣬length�Ǻ��߳��ȣ�color �Ǻ�����ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_HLine(unsigned char x, unsigned int y, unsigned char length, unsigned int color) 
{  
	GUI_setwindow(x,y,x+length-1,y);
	while(length--)
	{
		GUI_write_data_u16(color);//�����ʾ�����ˮƽ��
	}		
}
/*******************************************************************************
* �������� : ������
* �������� : �ⲿ
* ������� : (x��y)������������꣬high ���߸߶ȣ�color ��������ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_RLine(unsigned char x, unsigned int y, unsigned int high, unsigned int color) 
{  
   GUI_setwindow(x,y,x,y+high-1);
   while(high--)
   {
		GUI_write_data_u16(color);// �����ʾ�������ֱ��
   }		 	
}

/*******************************************************************************
* �������� : ��ֱ�ߣ����Ի����ⷽ��ֱ�ߣ��������ߡ����ߡ�б�ߣ���
* �������� : �ⲿ
* ������� : (x1,y1)���, (x2,y2)�յ�, color��ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)  
{  
    unsigned int t,row,col; 
	int xerr=0,yerr=0,delta_x,delta_y,distance ,incx,incy; 
    delta_x = x2-x1;//������������  
    delta_y = y2-y1;  
    col=x1;  
    row=y1;
	
    if(delta_x>0)
	{
		incx=1;//���õ������� 
	} 
    else
	{  
		if(delta_x==0)
		{
			incx=0;//��ֱ��
		}   
		else
		{
			incx=-1;
			delta_x=-delta_x;
		}  
    } 
	
	
    if(delta_y>0)
	{
		incy=1;
	}  
    else 
	{  
        if(delta_y==0)
		{
			incy=0;//ˮƽ��
		}   
        else 
		{
			incy=-1;
			delta_y=-delta_y;
		}  
    }
	
    if(delta_x>delta_y) 
	{
		distance=delta_x;//ѡȡ��������������
	}  
    else 
	{
		distance=delta_y;
	}  


    for(t=0;t<=distance+1;t++)  
	{                                     //�������  
	    GUI_Point(col, row, color);
        xerr+=delta_x;  
        yerr+=delta_y;  
      	if(xerr>distance)
		{  
            xerr-=distance;  
            col+=incx;  
        }  
        if(yerr>distance)
		{  
            yerr-=distance;  
            row+=incy;  
        }  
    }  
}

/*******************************************************************************
* �������� : ��ʾ16λ���ͼƬ(ͼ��)
* �������� : �ⲿ
* ������� : (x��y)�ǿ�ʼ������꣬length��ͼƬ���ȣ�high��ͼƬ�߶ȡ�
* ���ز��� : ��
* ������� : ָ��λ������ʾͼƬ��ͼƬ���ȶ�����bmp.h�е�pic[]������
			 
*******************************************************************************/

void GUI_DisPicture(unsigned char x, unsigned int y, unsigned char length, unsigned int high ,unsigned char *pic)
{
    unsigned int tmp=0,num=0;
	GUI_setwindow(x,y,x+length-1,y+high-1);
	num=length*high*2;
	do
	{  
		//16λ���ͼƬ,ÿ�����ص�Ҫ��16��������λ����ʾ,���������ֽڱ�ʾ,ͼƬ������
		//����length*high��,����������length*high*16λ
		GUI_write_data(pic[tmp+1],pic[tmp]);
		//ÿ��д��16λ������,������Ҫд��length*high�ξ��ܰ�ͼƬȫ��д��.
					
	   tmp+=2;		//��Ϊtmpÿ������2,Ϊ����д���ݺ���ִ��length*high��,����			
	}while(tmp<num);//num�ĳ�ֵӦΪlength*high*2
}

/*******************************************************************************
* �������� : ��ʾӢ�Ļ������ַ�
* �������� : �ⲿ
* ������� : (x��y) ����ʾ���ݵĿ�ʼ���꣬c����ʾ���ַ���
             color:��ɫ,b_color:������ɫ��
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_sprintf_char(unsigned char x, unsigned int y,unsigned char c, unsigned int color,unsigned int b_color)
{  
	unsigned char  s_x ,s_y, temp ;
	c = c - 32;
	for( s_y=0 ; s_y < 16 ; s_y++ )
	{
		if(s_y+y<320)
		{
			temp = font16x8[(unsigned int)c*16+s_y] ;
			for( s_x=0 ; s_x<8 ; s_x++ )
			{
				if(x+s_x<240)
				{
					if((temp&(0x80>>(s_x))) == (0x80>>(s_x))  )
					{
						GUI_Point(x+s_x, y+s_y,color) ;
					}
					else
					{
						GUI_Point(x+s_x, y+s_y,b_color) ;
					}
				}
			}
		}
	}
}


/*******************************************************************************
* �������� : ��ָ��λ������ʾӢ���ַ���
* �������� : �ⲿ
* ������� : (x��y) �ַ����Ŀ�ʼ����(�ַ��������Ͻ�����), *s��Ӣ���ַ���ָ��
*             color : �ַ�����ɫ, b_color: ������ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_sprintf_string(unsigned char x, unsigned int y, unsigned char code *s, unsigned int color,unsigned int b_color)
{  	
    while (*s) 
    {
        GUI_sprintf_char(x, y,*s, color,b_color);
        s++;
        x=x+8;
    }
}

/*******************************************************************************
* �������� : ��ʾԤ���庺���ַ���,�����ַ������ݴ����������
* �������� : �ⲿ
* ������� : x,y ��ʼ��ʾ��λ������; w,h ��Ԥ����õĺ��ֵĿ�͸�
             s ָ����Ԥ���庺�ֵ������ָ��; color ����������ɫ ;
             b_color ���ֵı�����ɫ
* ���ز��� : ��
* ������� : ���������ȶ�����������,��������Ӧ��������ȡģ��ʽ�Ҳ���ѡ���ֽڵ���
             �ķ�ʽ.
             ȡģ��� zimoV2.1�İ汾������,��͸ߵߵ���,����ѡ�� zimoV2.2�� 
*******************************************************************************/

void GUI_sprintf_HZ(unsigned char x,unsigned int y,unsigned char w,unsigned int h,unsigned char *s ,unsigned int color,unsigned int b_color)
{ 
	unsigned char s_x=0 ,s_y=0, temp=0 , k ;  
	unsigned int n,words;

	words=sizeof(china_char)/(w*h/8);//words,�������������
	for( n=0 ; n<words ; n++ )   //n=0�����һ����n=1����ڶ����֡���
  	{
		///////// /////// ���ֵ�����ֱ����ɷָ�Ϊ h/8����,k=0ʱ��ʾ��һ����,
		///////////////// k=1ʱ��ʾ�ڶ�����......
		for(k = 0 ; k < (h/8); k++)	
		{
			//============================ ����д��w(���ֵĿ�)��
			for( s_x=0 ; s_x<w ; s_x++ ) //w�е���
			{
				if(s_x+x<240) //�жϴ���ʾ�������Ƿ���дһ�����ֵĿ�
				{
				    temp = s[n*(unsigned int)w*(unsigned int)h/8 + s_x + w*k] ;
					
					//--------------- ����д8����,��һ���ֽ�
					for( s_y=0 ; s_y<8 ; s_y++ ) //8�е���
					{
						if(y+s_y<320)	//�жϴ���ʾ�������Ƿ���дһ�����ֵĸ�
						{
							if((temp&(0x80>>(s_y))) == (0x80>>(s_y))  )
							{
								GUI_Point(x+s_x+n*w, y+s_y+8*k,color) ;
							}
							else
							{
								GUI_Point(x+s_x+n*w, y+s_y+8*k,b_color) ;
							}
						}
					}
					//-------------------
				 }
			}
			//=======================================
		}
		////////////////////////
	}
}



/*******************************************************************************
* �������� : ��ʵ�ľ���
* �������� : �ⲿ
* ������� : (sx,sy)���ϽǶ�������, (ex,ey)���½Ƕ�������, color��ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_box(unsigned char sx,unsigned int sy,unsigned char ex,unsigned int ey,unsigned int color)
{ 
	unsigned int temp;   
	GUI_setwindow(sx,sy,ex,ey); 
    sx=ex-sx+1;
	sy=ey-sy+1;

	while(sx--)
	{
	 	temp=sy;
		while(temp--)
	 	{	
			GUI_write_data_u16(color);
		}
	}
}

/*******************************************************************************
* �������� : �����ľ���
* �������� : �ⲿ
* ������� : (sx,sy)���ϽǶ�������, (ex,ey)���½Ƕ�������, color��ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_rectangle( unsigned char sx,unsigned int sy,unsigned char ex,unsigned int ey,unsigned int color)
{
	GUI_line(sx,sy,ex,sy,color);
	GUI_line(sx,sy,sx,ey,color);
	GUI_line(ex,sy,ex,ey,color);
	GUI_line(sx,ey,ex,ey,color);
}

/*******************************************************************************
* �������� : RGB��ɫ���
* �������� : �ⲿ
* ������� : R����ɫ������0-255,G����ɫ������0-255,B����ɫ������0-255
* ���ز��� : ��Ϻ����ɫ��
* ������� : ͨ������ʰɫ���� R,G,B����,������������������,�����᷵��һ����ɫ��

*******************************************************************************/

unsigned int RGB(unsigned char R,unsigned char G,unsigned char B)
{
	return((unsigned int)(R&0XF8)<<8|(unsigned int)(G&0XFC)<<3|(unsigned int)(B&0XF8)>>3);
}


/*******************************************************************************
* �������� : �Զ�������ʽ��ʾunsigned char����
* �������� : �ⲿ
* ������� : (x y) Ҫ��ʾ��λ������,dat Ҫ��ʾ����,color �ǵ����ɫ,b_color����ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_sprintf_char_to_bit(unsigned char x, unsigned int y,unsigned char dat, unsigned int color,unsigned int b_color)
{  
	unsigned char i ;
	for(i=0;i<8;i++)
	{
		if((dat&(0x80>>i))==(0x80>>i))
		{
			GUI_sprintf_nu(x,y,1,color,b_color) ;
		}
		else 
		{
			GUI_sprintf_nu(x,y,0,color,b_color) ;
		}
		x+=8 ;
	}
}

/*******************************************************************************
* �������� : ��ʮ��������ʽ��ʾunsigned char����
* �������� : �ⲿ
* ������� : (x��y)����ʾ���ݵ����Ͻ����꣬dat Ҫ��ʾ��unsigned char������
*			 color:���ֵ���ɫ,b_color:������ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_sprintf_char_to_hex(unsigned char x, unsigned int y,unsigned char dat, unsigned int color,unsigned int b_color)
{  
	GUI_sprintf_nu(x,y,dat>>4,color,b_color) ;
	x+=8 ;
	GUI_sprintf_nu(x,y,dat&0x0f,color,b_color) ;
}

/*******************************************************************************
* �������� : ��ʮ������ʽ��ʾunsigned char����
* �������� : �ⲿ
* ������� : (x��y)����ʾ���ݵ����Ͻ����꣬dat Ҫ��ʾ��unsigned char������
*			 color:���ֵ���ɫ,b_color:������ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_sprintf_char_to_dec(unsigned char x,unsigned int y,unsigned char dat,unsigned int color,unsigned int b_color)
{
	GUI_sprintf_char(x,y,dat/100+'0',color,b_color);
	GUI_sprintf_char(x+8,y,dat/10%10+'0',color,b_color);
	GUI_sprintf_char(x+16,y,dat%10+'0',color,b_color);
}

/*******************************************************************************
* �������� : ��ʾ1λ16������
* �������� : �ⲿ
* ������� : (x��y)����ʾ���ݵ����Ͻ����꣬dat Ҫ��ʾ��1λ16��������
*			 color:���ֵ���ɫ,b_color:������ɫ
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void GUI_sprintf_nu(unsigned char x, unsigned int y,unsigned char dat, unsigned int color,unsigned int b_color)
{  
    unsigned char  s_x=0 ,s_y=0, temp=0 ;
    if (dat>9)
    {
		dat=dat+7;
	} 
    dat = dat+16;
	
    for(s_y=0;s_y<16;s_y++)
    {
		if(s_y+y<320)
		{
			temp = font16x8[(unsigned int)dat*16+s_y] ;
			for( s_x=0 ; s_x<8 ; s_x++ )
			{
				if(x+s_x<240)
				{
					if((temp&(0x80>>(s_x))) == (0x80>>(s_x))  )
					{
						GUI_Point(x+s_x, y+s_y,color) ;
					}
					else
					{
						GUI_Point(x+s_x, y+s_y,b_color) ;
					}
				}
			}
		}
	}
}

/*******************************************************************************
* �������� : ��ʮ������ʽ��ʾunsigned long int����
* �������� : �ⲿ
* ������� : (x��y)����ʾ���ݵ����Ͻ����꣬dat Ҫ��ʾ��unsigned long int������
*			 color:���ֵ���ɫ,b_color:������ɫ
* ���ز��� : ��
* ������� : ������ʾ����ֵ��ΧΪ��0~4,294,967,295(ʮ����)��Ҳ����˵���øú�������
			����ཫһ��10λʮ��������ʾ��LCD������ʹ�øú���ʱ������������ʾ������
			��λ����������LCD��Ļ��ռ10���ַ��Ŀռ䣨��ʾ�Ķ�λʮ����������10λʱ��
			������������Ч��ʾ����ǰ�油�ո�
*******************************************************************************/
//(x��y)����ʾ���ݵ����Ͻ�����
void GUI_sprintf_U32_to_dec(unsigned char x, unsigned int y, unsigned long int dat, unsigned int color,unsigned int b_color)
{
	unsigned long int temp; 
	unsigned char  i;
	for(i=0;i<10;i++)
	{
		if(dat!=0)
		{
			temp=dat/10;
			GUI_sprintf_char(x-7,y,dat-temp*10+'0',color,b_color);
			dat=temp;
		}
		else/*��ǰд��10λ��Ҳ����ǰ�涼д�ո�*/
		{
			GUI_sprintf_char(x-7,y,' ',color,b_color);
		}
		x=x-8;/*���ù�������ֲ�����Զ�����*/
	}
}


/*******************************************************************************
* �������� : ����һ�����к���,���������־���
* �������� : �ⲿ
* ������� : ��
* ���ز��� : ��
* ������� : 
*******************************************************************************/
//void Avoid_warning()
//{
//	GUI_Point(0,0,RED);
//	GUI_HLine(0,0,0,RED);
//	GUI_RLine(0,0,0,RED);
//	GUI_line(0,0,0,0,RED);
//	GUI_DisPicture(0,0,0,0,pic);
//	GUI_sprintf_char(0,0,'0',RED,YELLOW);
//	GUI_sprintf_string(0,0,"0",RED,YELLOW);
//	GUI_box(0,0,0,0,RED);
//	GUI_rectangle(0,0,0,0,RED);
//	GUI_sprintf_HZ(0,0,0,0,china_char,RED,YELLOW);
//	RGB(0,0,0);
//	GUI_sprintf_nu(0,0,0,RED,YELLOW);
//	GUI_sprintf_char_to_bit(0,0,0,RED,YELLOW);
//	GUI_sprintf_char_to_dec(0,0,0,RED,YELLOW);
//	GUI_sprintf_char_to_hex(0,0,0,RED,YELLOW);
//	GUI_sprintf_U32_to_dec(240,0,0,RED,YELLOW);
//	GUI_clearscreen(WHITE);
//}


/*******************************************************************************
* �������� : ������
* �������� : 
* ������� : 
* ���ز��� : 
* ������� : 
*******************************************************************************/

void main()
{
	ILI9325_initial( );
//	Avoid_warning();	 //�������к���
	GUI_clearscreen(WHITE);   //����
	GUI_DisPicture(32,30,187,91,pic);
	GUI_sprintf_HZ(56,190,29,32,china_char,BLUE,WHITE);
	while(1);
}

