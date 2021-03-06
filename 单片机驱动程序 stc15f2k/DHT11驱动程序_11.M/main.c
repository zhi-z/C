/*******************************************************************************
* 文件名 : main.C

* 版本     作者            日期            说明
* V1.1      zhi-z       2014/08/03       初始版本

* 描述   : MCU:stc15f2k60s2      晶振:22.1184   MHz
           
******************************************************************************/
# include "DHT11.h"
#include "uart.h"

void main()
{
	lcd_init();
	display_12864(2,1, "温度：");
	display_12864(3,1, "湿度：");
	while(1)
	{  
		DHT11_DHT11_Read();
		Delay20ms();
		Delay20ms();
		Delay20ms();
		Delay20ms();		  //延时不能少
		Delay20ms();
		Delay20ms();
		display();
	}
}

