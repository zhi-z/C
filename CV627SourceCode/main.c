#include "main.h"
#include "stm32f10x_Init.h"
#include "PCD.h"
#include "string.h"
#include "ISO14443.h"
#include "History.h"
#include "usart.h"


//------------------------------------------------------------------------------
// Functions Define
//------------------------------------------------------------------------------
tpd_Card g_sCard;						// ��Ƶ��
unsigned char  g_cBuffer[256];			// ͨ�Ż���
unsigned short g_iLength;

unsigned char  g_cATS[32];
unsigned char  g_cATSLen;

const unsigned char g_cM1KEY[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
int snr = 17;    // ������17


//******************************************************************************
//    PARAM: void
//   RETURN: void
//  COMMENT: ������
//  		
//******************************************************************************
int main(void)
{
	unsigned char cStatus;
	
	// ��ʼ��ϵͳ
	InitializeSystem();
	
	// ��ʼ���豸
	InitDevice();
	
	uart_init(115200);
	
	printf("init..........\r\n\r\n");
	
	do
	{
		printf("running..........\r\n\r\n");
		//��Ƭ��λ
		PiccReset();
		
		// ISO14443AѰ�� �жϿ�����
		// ѡ��������ײ��
		// Ѱ��
		cStatus = PiccRequest(PICC_REQALL, g_sCard.cATQ);   // PICC_REQALL:Ѱ��������ȫ����,g_sCard.cATQ:���տ�Ƭ���ʹ���
		if (cStatus != ERR_SUCCESS)
		{
			cStatus = PiccRequest(PICC_REQALL, g_sCard.cATQ);
		}
	
		if (cStatus == ERR_SUCCESS)
		{
			// ѡ��������ײ
			cStatus = PiccAnticollSelect(&g_sCard.cLen, g_sCard.cUID, &g_sCard.cSAK);
			// ������
			cStatus = PiccHaltA();		// �������� 00
			
			cStatus = PiccRequest(PICC_REQIDL, g_sCard.cATQ);	// �������� FF �޿�
			
			cStatus = PiccRequest(PICC_REQALL, g_sCard.cATQ);	// �������� 00 
			if (cStatus == ERR_SUCCESS)
			{
				cStatus = PiccAnticollSelect(&g_sCard.cLen, g_sCard.cUID, &g_sCard.cSAK);
			}
			LED_GRE(ON);
		}
		else
		{
			LED_GRE(OFF);
		}

		// ISO14443-4 Type A ��������
		if (cStatus == ERR_SUCCESS)
		{
			if (g_sCard.cSAK & 0x20)
			{
				// ISO14443-4 Type A CPU ��������
					// ��λ����
					// ѡӦ��
					// ȡ�����
				cStatus = ISO14443A_Rst(&g_cATSLen, g_cATS);
				if (cStatus == ERR_SUCCESS)
				{
//					memcpy(g_cBuffer, "\x00\xA4\x00\x00\x02\x3F\x00", 7);
//					g_iLength = 0x07;
//					cStatus = ISO14443_COS(&g_iLength, g_cBuffer);
					
					memcpy(g_cBuffer, "\x00\x84\x00\x00\x08", 5);
					g_iLength = 0x05;
					cStatus = ISO14443_COS(&g_iLength, g_cBuffer);
					LED_YEL(ON);
				}
				else
				{
					LED_YEL(OFF);
				}
			}
			else
			{
				// Mifare�� ��������
					// ��֤����
					// ��ȡ���ݿ�1
					// д�����ݿ�1	���
					// ��ȡ���ݿ�1
					// д�����ݿ�1	дֵ
			
				cStatus = PiccAuthState(15*4+3, 0x60, 0x00, (unsigned char *)g_cM1KEY);
				printf("piccAuthState: 0x%x\n", cStatus);
				if (cStatus == ERR_SUCCESS)
				{
					// ��ȡ0��������1������
					cStatus = PiccRead(15*4+1, g_cBuffer);
					printf("read 18 sector!\n");
					printf("value = : %s [%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x]\r\n\r\n",
							g_cBuffer,
							g_cBuffer[0],
							g_cBuffer[1],
							g_cBuffer[2],
							g_cBuffer[3],
							g_cBuffer[4],
							g_cBuffer[5],
							g_cBuffer[6],
							g_cBuffer[7],
							g_cBuffer[8],
							g_cBuffer[9],
							g_cBuffer[10],
							g_cBuffer[11],
							g_cBuffer[12],
							g_cBuffer[13],
							g_cBuffer[14],
							g_cBuffer[15]
					);
				}

//				if (cStatus == ERR_SUCCESS)
//				{
//					memset(g_cBuffer, 0xA5, 16);
//					// д0��������1������
//					cStatus = PiccWrite(16*4+1, "123456aaaaabbbbb");
//					
//				}

//				if (cStatus == ERR_SUCCESS)
//				{
//					cStatus = PiccRead(16*6+1, g_cBuffer);
//					printf("read 18 sector!\n");
//					printf("value = :%s\n",g_cBuffer);
//				}

//				if (cStatus == ERR_SUCCESS)
//				{
//					memset(g_cBuffer, 0x00, 16);
//					cStatus = PiccWrite(16*4+1, "123456aaaaabbbbb");
//				}
				
				
			}
		}
		else
		{
			LED_YEL(OFF);
			LED_BLU(OFF);
		}
		
		// ISO14443-4 Type B ��������
		
		cStatus = IDCard_Reqest(&g_iLength, g_cATS);
		
		if (cStatus == ERR_SUCCESS)
		{
			g_iLength = sizeof("\x00\x36\x00\x00\x08") - 1;
			memcpy(g_cBuffer, "\x00\x36\x00\x00\x08", g_iLength);
			cStatus = IDCard_APDU(&g_iLength, g_cBuffer);
			
			if (cStatus == ERR_SUCCESS)
			{
				LED_GRE(ON);
				printf("read finish!\n");
				printf("������ֵ��:%s\n",g_cBuffer);
			}
		}
		else
		{
			LED_GRE(OFF);
		}


	} while (1);
}
//******************************************************************************
//	 	IN: void
//	RETURN: void
// Comment: 
//
//******************************************************************************
void InitDevice(void)
{
	unsigned char cStatus;
	LED_RED(ON);
	LED_GRE(ON);
	LED_YEL(ON);
	LED_BLU(ON);
	BEEP(ON);
	Delay100uS(2000);		// ��ʱ200ms
	LED_RED(OFF);
	LED_GRE(OFF);
	LED_YEL(OFF);
	LED_BLU(OFF);
	BEEP(OFF);
	
	// LED_RED:	��Ƶ����������
	// LED_GRE:	�п�ָʾ�ƣ�������ΪM1��
	// LED_YEL:	Type A CPU Card
	// LED_BLU:	Type B CPU Card
	


	RC_SEL0(OFF);
	RC_SEL1(ON);			// ������ƵоƬΪSPIģʽ
	
	cStatus = PcdReset();   // ��λ����ʼ��RC663
	if (cStatus == ERR_SUCCESS)
	{
		PcdConfig(PCD_TYPE_14443A);
		LED_RED(ON);		// ��Ƶ��ʼ���ɹ���RED LED ������
	}

	return;
}

//******************************************************************************
//	 	IN: unsigned int iTimes
//	RETURN: void
// Comment: ʹ��Timer4���ȴ���ʱ,��λʱ��100us
//
//******************************************************************************
void Delay100uS(unsigned int iTimes)
{	
	Timer4_Init(TIME_UNIT_100US);
	
	Timer4_Wait(iTimes);
	
	Timer4_Stop();
	return;
}

void Timer4_Init(unsigned char cMode)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	uint16_t PrescalerValue;
	uint16_t PeriodValue;
	
	switch (cMode)
	{
		case TIME_UNIT_100US:
			PrescalerValue = 0;				// ϵͳʱ��1��Ƶ
			PeriodValue    = SystemCoreClock/10000;
			break;
		
		case TIME_UNIT_1MS:
			PrescalerValue = 1;				// ϵͳʱ��2��Ƶ
			PeriodValue    = SystemCoreClock/2000;
			break;
		
		case TIME_UNIT_200MS:
			PrescalerValue = 575;			// ϵͳʱ��576��Ƶ
			PeriodValue    = SystemCoreClock/2880;
			break;
		
		default:
			break;
	}

	TIM_TimeBaseStructure.TIM_Period = PeriodValue;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_SetCounter(TIM4, 0x0000);
	TIM_ITConfig(TIM4, TIM_IT_Update , ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	return;
}
void Timer4_Wait(unsigned short sTime)
{
	do
	{
		sTime--;
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		while(TIM_GetITStatus(TIM4, TIM_IT_Update) == RESET);
		
	} while (sTime);
	return;
}
void Timer4_Stop(void)
{
	TIM_Cmd(TIM4, DISABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update , DISABLE);
	return;
}


