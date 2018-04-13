/*******************************************************************************
* �ļ��� :NRF24L01.c
* ʵ�ֹ���:nrf24l01���ճ���
* �汾     ����            ����            ˵��
* V1.0     zhi-z      2018/08/05      ��ʼ�汾

* ����   : MCU: STC15f2k60s2     ����: 22.1184   MHz
           
*******************************************************************************/

#include "NRF24L01.h"

//-------------------------- ״̬��־
uint 	bdata sta;   // ���ڱ���״̬�Ĵ�����ֵ
sbit	Rx_DR	=sta^6;    //����״̬
sbit	Tx_DS	=sta^5;    //����״̬
sbit	Max_RT	=sta^4;

//------------------------------------------------------ ���͵����ݵ���������
#define Tx_Address_Width    5   // Ҫ���͵ĵ�ַ�Ŀ��Ϊ5�ֽڣ�Ҫ�ڽ���ģ���Ӧ
#define Rx_Address_Width    5   // Ҫ���յĵ�ַ�Ŀ��Ϊ5�ֽڣ�Ҫ�ڷ���ģ���Ӧ
uint const Tx_Address[Tx_Address_Width]= {0x34,0x43,0x10,0x10,0x01};//���ص�ַ��5�ֽ�
uint const Rx_ADDRESS[Rx_Address_Width]= {0x34,0x43,0x10,0x10,0x01};//���յ�ַ��5�ֽ�


/*******************************************************************************
* �������� : ��ʱ��������ʱ����Ϊ 1 ʱ����Լ��ʱ10us
* �������� : �ڲ�
* ������� : ��ʱ����
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void Delay_us(unsigned char n)
{
	unsigned char i;
	
	while(n--)
	{
		_nop_();
		_nop_();
		_nop_();
		i = 52;
		while (--i);
	}
}

/*******************************************************************************
* �������� : SPI��д������������ĺ���
* �������� : �ڲ�
* ������� : Ҫд�������
* ���ز��� : ����������
* ������� : 

	������ĺ�������� GPIO ģ�� SPI �Ĺ��ܡ�������ֽڣ�MOSI���� MSB ѭ�������
ͬʱ�������ֽڣ�MISO���� LSB ѭ�����롣�����ض��룬�½��������

*******************************************************************************/

uint SPI_ReadWrite(uint dat)
{
	uint bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) //һ���ֽ�����ѭ��8��
   	{
		MOSI = (dat & 0x80);    //��dat�ĸ�λ���MOSI
		dat  = (dat << 1);      //dat�������ƣ����λ�����������θ�λ����ռ�����λ
		SCK = 1;                // SCK�øߣ�׼������
		dat |= MISO;       	   // ��ȡMISO���ݴ��뵽dat�����λ
		SCK = 0;               // ��ʼ��������
   	}
    return(dat);           		//���ض�����ֵ
}

/*******************************************************************************
* �������� : NRF24L01���Ĵ���
* �������� : �ڲ�
* ������� : dat: ָ��+�Ĵ�����ַ
* ���ز��� : �Ĵ�����ֵ
* ������� : �β�Ϊ��

	��ȡ�Ĵ���ֵ�ĺ���������˼·����ͨ�� READ_REG ����β�Ϊ��0x00+�Ĵ�����ַ����
�ѼĴ����е�ֵ����������ֵ����

*******************************************************************************/

uchar Read_Reg(uchar dat)
{
	uchar reg_val;
	
	CSN = 0;                // CSN low, initialize SPI communication...
	SPI_ReadWrite(dat);            // Select register to read from..
	reg_val = SPI_ReadWrite(0);    // ..then read registervalue
	CSN = 1;                // CSN high, terminate SPI communication
	
	return(reg_val);        // return register value
}

/*******************************************************************************
* �������� : NRF24L01����д�Ĵ���
* �������� : �ڲ�
* ������� : dat: ָ��+�Ĵ�����ַ     value�� Ҫд���ֵ
* ���ز��� : ��
* ������� : 

	���øú���ʱʹ��д�Ĵ���ָ�WRITE_REG������ dat = WRITE_REG + ��Ҫд������
�ļĴ����ĵ�ַ�����ɽ�valueд�뵽��Ҫд��ļĴ���

*******************************************************************************/

uint ReadWrite_Reg(uchar dat, uchar value)
{
	uint status;
	
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_ReadWrite(dat);      // select register
	SPI_ReadWrite(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	
	return(status);            // return nRF24L01 status uchar
}

/*******************************************************************************
* �������� : ��NRF24L01��FIFO�����������ݶ�ȡ��ŵ��û��Ļ�������
* �������� : �ڲ�
* ������� : dat: ָ��+�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars���������ݵĸ���
* ���ز��� : 
* ������� : 

	���ջ��������ʺ�������Ҫ������ȡNRF24L01��FIFO�������е����ݡ�����˼·����ͨ��
READ_REG ��������ݴ�NRF24L01��FIFO��������RD_RX_PLOAD���ж������浽�û��Ļ�����
�����飩

*******************************************************************************/

uint Read_From_FIFO_Buf(uchar dat, uchar *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;                    		// Set CSN low, init SPI tranaction
	status = SPI_ReadWrite(dat);       		// Select register to write to and read status uchar
	
	for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
	{
		pBuf[uchar_ctr] = SPI_ReadWrite(0);    // 
	}
		
	CSN = 1;                           
	
	return(status);                    // return nRF24L01 status uchar
}

/*******************************************************************************
* �������� : ���û�������������д�뵽NRF24L01��FIFO������
* �������� : �ڲ�
* ������� : dat: ָ��+�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��value���������ݵĸ���
* ���ز��� : 
* ������� : 

	��Ҫ�������û������������飩������ŵ���д�뵽NRF24L01��FIFO�������С�����˼
·����ͨ��WRITE_REG ��������ݴ浽���� FIFO��WR_TX_PLOAD����ȥ

*******************************************************************************/

uint Write_To_FIFO_Buf(uchar dat, uchar *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;            //SPIʹ��       
	status = SPI_ReadWrite(dat);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
	{
		SPI_ReadWrite(*pBuf++);
	}
		
	CSN = 1;           //�ر�SPI
	return(status);    // 
}

/*******************************************************************************
* �������� : ����Ϊ���ݽ���ģʽ��Ҫʹ�ý���ģʽʱ��Ҫ���øú����� 
* �������� : �ⲿ
* ������� : ��
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void SetRX_Mode(void)
{
	CE=0;
	ReadWrite_Reg(WRITE_REG + CONFIG, 0x0f);  // IRQ�շ�����ж���Ӧ��16λCRC	��������
	CE = 1; 
	Delay_us(130);
}

/*******************************************************************************
* �������� : ���շ���ģ������ݣ��������ݷ���rx_buf�û����ջ�������
* �������� : �ⲿ
* ������� : �û��Ļ������ĵ�ַ
* ���ز��� : 
* ������� : 
*******************************************************************************/

unsigned char NRF24L01_RxPacket(unsigned char* Rx_Buf)
{
    unsigned char revale=0;
	
	SetRX_Mode();//����Ϊ����ģʽ
	
	sta=Read_Reg(STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
	if(Rx_DR)				// �ж��Ƿ���յ�����
	{
	    CE = 0; 			//SPIʹ��
		Read_From_FIFO_Buf(RD_RX_PLOAD,Rx_Buf,Tx_Data_Width);// read receive payload from RX_FIFO buffer
		revale =1;			//��ȡ������ɱ�־
	}
	ReadWrite_Reg(WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	return revale;
}

/*******************************************************************************
* �������� : ��tx_buf�û��������е����ݷ��͸�����ģ��
* �������� : �ⲿ
* ������� : �û��Ļ������ĵ�ַ
* ���ز��� : ���͵Ľ��
* ������� : 
*******************************************************************************/

uchar NRF24L01_TxPacket(unsigned char * Tx_Buf)
{
	unsigned char revale=0;
	CE=0;			//StandBy Iģʽ	
	Write_To_FIFO_Buf(WRITE_REG + RX_ADDR_P0, Tx_Address, Tx_Address_Width); // װ�ؽ��ն˵�ַ
	Write_To_FIFO_Buf(WR_TX_PLOAD, Tx_Buf, Tx_Data_Width); 			 // װ������	
	ReadWrite_Reg   (WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	CE=1;		 //�ø�CE���������ݷ���
	Delay_us(10);
	
	ReadWrite_Reg(WRITE_REG+STATUS,0XFF); //�����־λ�Ĵ���
	sta=Read_Reg(STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
	if(!Tx_DS)
	{
		revale = 1;
	}
	return revale; //���ط��ͽ��
}

/*******************************************************************************
* �������� : NRF24L01��ʼ��
* �������� : �ⲿ
* ������� : ��
* ���ز��� : ��
* ������� : 
*******************************************************************************/

void init_NRF24L01(void)
{
    Delay_us(100);
	
 	CE=0;    // chip enable
 	CSN=1;   // Spi disable 
 	SCK=0;   // Spi clock line init high
 	
	Write_To_FIFO_Buf(WRITE_REG + TX_ADDR,    Tx_Address, Tx_Address_Width);//д���ص�ַ
	Write_To_FIFO_Buf(WRITE_REG + RX_ADDR_P0, Rx_ADDRESS, Rx_Address_Width);//д���ն˵�ַ
	ReadWrite_Reg(WRITE_REG + EN_AA, 0x01); //Ƶ��0�Զ�	ACKӦ������	
	ReadWrite_Reg(WRITE_REG + EN_RXADDR, 0x01); //������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	ReadWrite_Reg(WRITE_REG + RF_CH, 0);  //�����ŵ�����Ϊ2.4GHZ���շ�����һ��
	ReadWrite_Reg(WRITE_REG + RX_PW_P0, Rx_Data_Width);//���ý������ݳ��ȣ���������ΪRX_PLOAD_WIDTH���ֽ�
	ReadWrite_Reg(WRITE_REG + RF_SETUP, 0x07); //���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB	
	ReadWrite_Reg(WRITE_REG + CONFIG, 0x0e); //IRQ�շ�����ж���Ӧ��16λCRC��������
}

