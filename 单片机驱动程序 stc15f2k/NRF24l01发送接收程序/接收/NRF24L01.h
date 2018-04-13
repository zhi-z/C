/*******************************************************************************
* �ļ��� :NRF24L01.h
* ʵ�ֹ���:nrf24l01���ճ���
* �汾     ����            ����            ˵��
* V1.0     zhi-z       2018/08/05      ��ʼ�汾

* ����   : MCU: STC15f2k60s2     ����: 22.1184   MHz
           
*******************************************************************************/
#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include <STC15F2K60S2.H>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned char uint;

#define Tx_Data_Width  3  	// Ҫ���͵����ݵĿ��Ϊ6���ֽڣ�������Ҫ�޸�
#define Rx_Data_Width  3  	// Ҫ���յ����ݵĿ��Ϊ6���ֽڣ�������Ҫ�޸�

//--------------------------------------------------- NRF24L01�Ĵ���ָ��
#define WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��

//------------------------------------SPI(nRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���   
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���

sbit	CSN		=P0^2;
sbit 	MOSI	=P0^3;
sbit	IRQ		=P0^4;
sbit	CE	  =P0^5;
sbit	SCK	  =P0^6;
sbit 	MISO	=P0^7;

void init_NRF24L01(void); // ��ʼ��NRF24L01 

uchar NRF24L01_TxPacket(unsigned char * Tx_Buf);	// ��tx_buf�е����ݷ��ͳ�ȥ

unsigned char NRF24L01_RxPacket(unsigned char* Rx_buf);//�������ݲ������ݷ���rx_buf���ջ�������
	
#endif /*__NRF24L01_H__*/


