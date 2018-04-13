


#include"lcd12864.h"


//********************************
void lcd_delay1ms(uint z)//��ʱ����
{
	unsigned int i , j ;
	for (i = z ; i > 0 ; i--)
		for (j = 1700 ; j > 0 ; j--) ;
}

/****************д�����************/
void write_com(byte com)
{
	lcd_rs = 0;
	lcd_rw = 0;
	lcd_e = 0;
	port = com;
	lcd_delay1ms(5);
  lcd_e = 1;
	lcd_delay1ms(5);
	lcd_e = 0;
}

/**************д���ݺ���***************/
void write_dat(byte dat)
{
	lcd_rs = 1;
	lcd_rw = 0;
	lcd_e = 0;
  port = dat;
	lcd_delay1ms(5);
	lcd_e = 1;
	lcd_delay1ms(5);
	lcd_e = 0;

}
/**************lcd�ĳ�ʼ������************/
void lcd_init()
{
   lcd_12864_psb = 1;
	 write_com(0x30);//����ָ�����
	 lcd_delay1ms(5);
	 write_com(0x0c);//��ʾ�����ع��
	 lcd_delay1ms(5);
	 write_com(0x01);//���lcd����ʾ����
	 lcd_delay1ms(5);
}
/**************��ʾλ��*********************/

void place(byte x,byte y)
{
	byte psb;
	if(x == 1)
	{
		x = 0x80;
	}

	if(x == 2)
	{
		x = 0x90;
	}

	if(x == 3)
	{
		x = 0x88;
	}

	if(x == 4)
	{
		x = 0x98;
	}
	
	psb = x + y;		   //��ʾ�ڼ���
    write_com(psb);

}

/**************��ʾ*********************/
void display_12864(byte x,byte y, byte * p)
{
	place(x,y);
	while( *p != '\0')
	{
		write_dat(*p);
		p++;
	}
}
