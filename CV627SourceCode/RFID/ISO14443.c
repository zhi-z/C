/*******************************************************************************
 * CopyRight(c)2010-2017, BJ-TOP Electronics Co.,Ltd. All rights reserved
 * FileName: ISO14443.c
 *   Author: Layne Wen
 *     Date: 2017.02.22
 *  COMMENT: �汾���ڸ���
 ******************************************************************************/
#include "ISO14443.h"
#include "String.h"
#include "PCD.h"


extern tpd_Card    g_sCard;
extern tpd_pcdpara g_sPcdPara;
extern tpd_pcdtrsv g_sPcdTrsv;

//******************************************************************************
//	 	IN: void
//	RETURN: void
// Comment: ��Ƭ��λ
//
//******************************************************************************
void PiccReset(void)
{
	PcdAntenna(OFF);
	Delay100uS(50);
	PcdAntenna(ON);
	Delay100uS(50);
	return;
}
//******************************************************************************
//	 	IN: unsigned char cReq				// 0x26=PICC_REQIDL; 0x52=PICC_REQALL;
//	   OUT: unsigned char *cATQA			// Answer To Request
//	RETURN: unsigned char
// Comment: Ѱ��
//
//******************************************************************************
unsigned char PiccRequest(unsigned char cReq, unsigned char *cATQA)
{
	unsigned char cStatus;
	
	PcdConfig(PCD_TYPE_14443A);
	
	PcdSetTmo(0);

	SetBitMask(PHHAL_HW_RC663_REG_DRVMODE, 0x08);		// Enable Transmitter PIN
	WriteRawRC(PHHAL_HW_RC663_REG_TXCRCCON,0x18);		// Disable TX CRC
	WriteRawRC(PHHAL_HW_RC663_REG_RXCRCCON,0x18);		// Disable RX CRC
	WriteRawRC(PHHAL_HW_RC663_REG_TXDATANUM,0x0F);		// Sent last data is 7 bit; Need to reset.
	WriteRawRC(PHHAL_HW_RC663_REG_RXBITCTRL,0x00);
	
	SetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
	ClearBitMask(PHHAL_HW_RC663_REG_STATUS, 0x20);

	g_sPcdTrsv.cData[0] = cReq;
	g_sPcdTrsv.iLength = 1;
	// #PHHAL_HW_RC663_CMD_TRANSMIT:�����ݴ�FIFO���������䵽����
	// Like #PHHAL_HW_RC663_CMD_TRANSMIT but automatically activates the receiver after transmission is finished.
	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE); 
	if (cStatus == ERR_SUCCESS)
	{
		if (g_sPcdTrsv.iLength == 2)
		{
			memcpy(cATQA, g_sPcdTrsv.cData, 2);
		}
		else
		{
			cStatus = MI_BITCOUNTERR;
		}
	}
	return cStatus;
}
//******************************************************************************
//	 	IN: unsigned char cLev				// PICC_ANTICOLL1; standard select code
//											   PICC_ANTICOLL2; cascaded level 1
//											   PICC_ANTICOLL3; cascaded level 2
//	   OUT: unsigned char *cUID				// 4 bytes serial number, and 1 byte BCC.
//	RETURN: unsigned char
// Comment: ����ײ
//
//******************************************************************************
unsigned char PiccAnticoll(unsigned char cLev, unsigned char *cUID)
{
	unsigned char cStatus;

	PcdSetTmo(0);

	WriteRawRC(PHHAL_HW_RC663_REG_TXCRCCON, 0x18);
	WriteRawRC(PHHAL_HW_RC663_REG_RXCRCCON, 0x18);
	WriteRawRC(PHHAL_HW_RC663_REG_TXDATANUM, 0x08);
	WriteRawRC(PHHAL_HW_RC663_REG_RXBITCTRL, 0x00);
	

	g_sPcdTrsv.iLength = 0x02;
	g_sPcdTrsv.cData[0] = cLev;
	g_sPcdTrsv.cData[1] = 0x20;

	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
	
	if (cStatus == ERR_SUCCESS)
	{
		memcpy(cUID, g_sPcdTrsv.cData, g_sPcdTrsv.iLength);
	}

	return cStatus;
}
//******************************************************************************
//	 	IN: unsigned char cLev
//			unsigned char *cUID				// 4 bytes serial number
//	   OUT: unsigned char *cSAK				// 1 byte select acknowledge
//	RETURN: unsigned char
// Comment: 
//
//******************************************************************************
unsigned char PiccSelect(unsigned char cLev,unsigned char *cUID,unsigned char *cSAK)
{
	unsigned char cStatus;
	
	PcdSetTmo(4);

	WriteRawRC(PHHAL_HW_RC663_REG_TXCRCCON,0x19);
	WriteRawRC(PHHAL_HW_RC663_REG_RXCRCCON,0x19);
	WriteRawRC(PHHAL_HW_RC663_REG_RXBITCTRL,0x00);
	
	g_sPcdTrsv.iLength = 0x07;
	g_sPcdTrsv.cData[0] = cLev;
	g_sPcdTrsv.cData[1] = 0x70;
	memcpy(&g_sPcdTrsv.cData[2], cUID, 5);

	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
	if (cStatus == ERR_SUCCESS)
	{
		if (g_sPcdTrsv.iLength != 0x01)
		{
			cStatus = MI_BITCOUNTERR;
		}
		*cSAK = g_sPcdTrsv.cData[0];
	}
	return cStatus;
}
//******************************************************************************
//	 	IN: void
//	RETURN: unsigned char
// Comment: 
//
//******************************************************************************
unsigned char PiccHaltA(void)
{
	unsigned char cStatus;

	g_sPcdTrsv.cData[0] = PICC_HALT;
	g_sPcdTrsv.cData[1] = 0x00;
	g_sPcdTrsv.iLength  = 0x02;
	cStatus = PcdTransceive(PCD_CMD_TRANSMIT);
	if (cStatus == MI_NOTAGERR)
	{
		cStatus = ERR_SUCCESS;
	}
	//PcdIdle();

	return cStatus;
}
//******************************************************************************
//	 	IN: unsigned char *cLen
//	   OUT: unsigned char *cUID
//			unsigned char *cSAK
//	RETURN: void
// Comment: 
//
//******************************************************************************
unsigned char PiccAnticollSelect(unsigned char *cLen, unsigned char *cUID, unsigned char *cSAK)
{
	unsigned char cStatus;
	
	// һ������ײ����
	cStatus = PiccAnticoll(PICC_ANTICOLL1, cUID);  // ��ȡ��cUID

	if (cStatus == ERR_SUCCESS)
	{
		// ѡ��
		cStatus = PiccSelect(PICC_ANTICOLL1, cUID, cSAK);
	}

	if (cStatus == ERR_SUCCESS)
	{
		*cLen = 0x04;
		// ��������ײ����
		if (*cSAK & 0x04)
		{
			memcpy(cUID, &cUID[1], 3);

			cStatus = PiccAnticoll(PICC_ANTICOLL2, &cUID[3]);
			if (cStatus == ERR_SUCCESS)
			{
				cStatus = PiccSelect(PICC_ANTICOLL2, &cUID[3], cSAK);
			}

			if (cStatus == ERR_SUCCESS)
			{
				*cLen = 0x07;
				// ��������ײ����
				if (*cSAK & 0x04)
				{
					memcpy(&cUID[3], &cUID[4], 3);

					cStatus = PiccAnticoll(PICC_ANTICOLL3, &cUID[6]);
					if (cStatus == ERR_SUCCESS)
					{
						cStatus = PiccSelect(PICC_ANTICOLL3, &cUID[6], cSAK);
					}
					if (cStatus == ERR_SUCCESS)
					{
						*cLen = 0x0A;
					}
				}
			}
		}
	}
	return cStatus;
}


//******************************************************************************
// FUNCTION: PiccTypeA_Rst
//       IN: unsigned char cPara
//           unsigned char *cpLen
//      OUT: unsigned char *cpData
//   RETURN: char cStatus
//  COMMENT: TypeA��Ƭ��λ����
//******************************************************************************
unsigned char PiccTypeA_Rst(unsigned char *cpLen, unsigned char *cpData)
{
	unsigned char cStatus;
	PcdSetTmo(5);
	
	g_sPcdTrsv.cData[0] = PICC_RESET;
	g_sPcdTrsv.cData[1] = g_sPcdPara.cFSD;
	g_sPcdTrsv.iLength = 2;

	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
	if (cStatus == ERR_SUCCESS)
	{
		memcpy(cpData, g_sPcdTrsv.cData, g_sPcdTrsv.iLength);
		*cpLen = g_sPcdTrsv.iLength;
	}
	return cStatus;
}

//******************************************************************************
// FUNCTION: FSCIת��Ϊ Max INF Length
//    PARAM: unsigned char cFSCI
//   RETURN: void
//******************************************************************************
void PiccCaluINF(unsigned char cFSCI)
{
	unsigned char cCount = 0x00;
	if (g_sPcdPara.cCID == 0x08)
	{
		cCount++;
	}
	if (g_sPcdPara.cNAD == 0x04)
	{
		cCount++;
	}
	switch (cFSCI)
	{
		case 0:				// 16 - PCB(1) - EDC(2) = 13
		case 1:				// 24
		case 2:				// 32
		case 3:				// 40
		case 4:				// 48
			g_sPcdPara.cMaxINF = cFSCI * 8 + 13 - cCount;
			break;
		case 5:				// 64 - PCB(1) - EDC(2) = 61
			g_sPcdPara.cMaxINF = 61 - cCount;
			break;
		case 6:				// 96 - PCB(1) - EDC(2) = 93
			g_sPcdPara.cMaxINF = 93 - cCount;
			break;
		case 7:				// 128 - PCB(1) - EDC(2) = 125
			g_sPcdPara.cMaxINF = 125 - cCount;
			break;
		default:			// 256 - PCB(1) - EDC(2) = 253
			g_sPcdPara.cMaxINF = 253 - cCount;
			break;
	}
	return;
}
//******************************************************************************
//       IN: unsigned char cTimeOut
//           unsigned char cWTX
//   RETURN: void
//  COMMENT: 
//******************************************************************************
void PiccWaitWTX(unsigned char cTimeOut, unsigned char cWTX)
{
	if (cWTX == 0 || cWTX > 59)
	{
		PcdSetTmo(cTimeOut);
	}
	else
	{
		//WriteRawRC(JREG_TRELOADLO, 0x04 * cWTX + 1);
	}
	return;
}
//******************************************************************************
//	 	IN: unsigned char *cpLen
//	   OUT: unsigned char *cpRATS
//	RETURN: unsigned char
// Comment: 
//
//******************************************************************************
unsigned char ISO14443A_Rst(unsigned char *cpLen, unsigned char *cpRATS)
{
	unsigned char cStatus;
	unsigned char i;

	// ��ʼ��Ĭ�ϲ���
	g_sPcdPara.cFWI = 0x0A;				// ����Ĭ��
	g_sPcdPara.cFSD = 0x40;				// ����Ĭ�϶���������
	g_sPcdPara.cCID = 0x00;
	g_sPcdPara.cNAD = 0x00;
	g_sPcdPara.cPCB = 0x00;

	cStatus = PiccTypeA_Rst(cpLen, cpRATS);
	if (cStatus == ERR_SUCCESS && cpRATS[0] > 0x01)
	{
		i = 2;
			
		if (cpRATS[1] & 0x10)		//TA1
		{
			//cTemp = cpRATS[i] & (cpRATS[i] >> 4) & 0x07;
			i++;
		}
		if (cpRATS[1] & 0x20)		//TB1
		{
			g_sPcdPara.cFWI = cpRATS[i] >> 4;
			i++;
		}
		if (cpRATS[1] & 0x40)		//TC1
		{
			if (cpRATS[i] & 0x02)
			{
				g_sPcdPara.cCID = 0x08;
			}
			if (cpRATS[i] & 0x01)
			{
				g_sPcdPara.cNAD = 0x04;
			}
		}
		PiccCaluINF(cpRATS[1] & 0x0F);
	}
	return cStatus;
}

#define PCB_I_BLOCK			0x02
#define PCB_R_BLOCK			0xA2
#define PCB_S_BLOCK			0xC2

//******************************************************************************
//	IN OUT: unsigned short *cpLen
//			unsigned char *cpAPDU
//	RETURN: unsigned char
// Comment: 
//
//******************************************************************************
unsigned char ISO14443_COS(unsigned short *cpLen, unsigned char *cpAPDU)
{
	unsigned char cStatus;
	unsigned char cErr, cCurIndex;
	unsigned char cSendLenNow = 0x00;
	unsigned char cRecvLenNow = 0x00;
	unsigned int  iSendIndex  = 0x00;
	unsigned int  iRecvIndex  = 0x00;
	
	PcdSetTmo(g_sPcdPara.cFWI);
	g_sPcdTrsv.cData[0] = 0xA2;			// ����R(ACK)��ֵ���ɽ���I-Blockװ������
	do
	{
		if ((g_sPcdTrsv.cData[0] & 0xF2) == 0xA2)			// �յ�R(ACK)Ӧ��   ����I(n)����
		{
			cCurIndex = 0x00;
			if ((iSendIndex + g_sPcdPara.cMaxINF) < *cpLen)
			{
				// �������ݳ��ȴ��ڿ�Ƭ�����������ְ����͡�
				g_sPcdTrsv.cData[cCurIndex++] = PCB_I_BLOCK | 0x10 | g_sPcdPara.cCID | g_sPcdPara.cNAD | g_sPcdPara.cPCB;			// I-Block, Chaining bit, CID, NAD, Block Number
				if (g_sPcdPara.cCID == 0x08)
				{
					g_sPcdTrsv.cData[cCurIndex++] = 0x00;
				}
				if (g_sPcdPara.cNAD == 0x04)
				{
					g_sPcdTrsv.cData[cCurIndex++] = 0x00;
				}
				cSendLenNow = g_sPcdPara.cMaxINF;
			}
			else
			{				
				// �������ݳ���С�ڿ�Ƭ�����������������͡�
				g_sPcdTrsv.cData[cCurIndex++] = PCB_I_BLOCK | g_sPcdPara.cCID | g_sPcdPara.cNAD | g_sPcdPara.cPCB;
				if (g_sPcdPara.cCID == 0x08)
				{
					g_sPcdTrsv.cData[cCurIndex++] = 0x00;
				}
				if (g_sPcdPara.cNAD == 0x04)
				{
					g_sPcdTrsv.cData[cCurIndex++] = 0x00;
				}
				cSendLenNow = (unsigned char)(*cpLen - iSendIndex);
			}
			
			memcpy(&g_sPcdTrsv.cData[cCurIndex], &cpAPDU[iSendIndex], cSendLenNow);
			g_sPcdTrsv.iLength = cCurIndex + cSendLenNow;

			// ���²���
			g_sPcdPara.cPCB ^= 0x01;			// ����PCB Block Number
			iSendIndex += cSendLenNow;			// ���·����������

		}
		else if ((g_sPcdTrsv.cData[0] & 0xF0) == 0x10)		// �յ�I(1)����   ����R(ACK)Ӧ��
		{
			// �������ݣ������cpAPDU
			cCurIndex = 1;
			if (g_sPcdTrsv.cData[0] & 0x08)
			{
				cCurIndex++;
			}
			if (g_sPcdTrsv.cData[0] & 0x04)
			{
				cCurIndex++;
			}
			cRecvLenNow = g_sPcdTrsv.iLength - cCurIndex;
			memcpy(&cpAPDU[iRecvIndex], &g_sPcdTrsv.cData[cCurIndex], cRecvLenNow);
			iRecvIndex += cRecvLenNow;
			
			// ���� R(ACK)
			cCurIndex = 1;
			g_sPcdTrsv.cData[0] = PCB_R_BLOCK | g_sPcdPara.cCID | g_sPcdPara.cPCB;
			if (g_sPcdPara.cCID)
			{
				g_sPcdTrsv.cData[cCurIndex++] = 0x00;
			}
			g_sPcdTrsv.iLength = cCurIndex;
			
			// ���²���
			g_sPcdPara.cPCB ^= 0x01;			// ����PCB Block Number

		}
		else if ((g_sPcdTrsv.cData[0] & 0xF2) == 0x02)		// �յ�I(0)����   ����
		{
			// �������ݣ������cpAPDU
			cCurIndex = 1;
			if (g_sPcdTrsv.cData[0] & 0x08)
			{
				cCurIndex++;
			}
			if (g_sPcdTrsv.cData[0] & 0x04)
			{
				cCurIndex++;
			}
			cRecvLenNow = g_sPcdTrsv.iLength - cCurIndex;
			memcpy(&cpAPDU[iRecvIndex], &g_sPcdTrsv.cData[cCurIndex], cRecvLenNow);
			
			*cpLen = iRecvIndex + cRecvLenNow;
			
			// ���ݽ�����ɣ�����ѭ��
			break;
			
		}
		else if ((g_sPcdTrsv.cData[0] & 0xF2) == 0xF2)		// �յ�S(WTX)   ����S(WTX)
		{
			cCurIndex = 1;
			if (g_sPcdTrsv.cData[0] & 0x08)
			{
				cCurIndex = 2;
			}
			// �ȴ���ʱ
			PiccWaitWTX(g_sPcdPara.cFWI, g_sPcdTrsv.cData[cCurIndex]);

		}
		else
		{
			cStatus = ERR_FAILURE;
			break;						// δ֪PCB�������ѭ��
		}
		
		// ���ݿ��շ�����
//		for (cErr=0; cErr<3; cErr++)
//		{
			cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);			// �շ�����������ѭ��
			
			if (cStatus == ERR_SUCCESS)
			{
				break;
			}
//		}
		
	} while (cStatus == ERR_SUCCESS);

	return cStatus;
}

/*******************************************************************************
 *  MODULE: ISO14443-3 Type B Function
 *  STRUCT: PiccRequestB			// ISO14443 TypeB
 *          PiccAttrib
 *          PiccHltb
 * COMMENT: 
*******************************************************************************/
//******************************************************************************
// FUNCTION: PiccRequestB
//       IN: unsigned char cReq             = 0   REQB
//                                          = 1   WUPB
//           unsigned char AFI
//           unsigned char N                = 0~7 Number of slots
//      OUT: unsigned char *cpATQB			// ������Ҫ16�ֽڵĻ���
//   RETURN: char
//  COMMENT: Type B Ѱ��
//******************************************************************************
unsigned char PiccRequestB(unsigned char cReq,unsigned char AFI,unsigned char N,unsigned char *cpATQB)
{
	unsigned char cStatus;
	
	PcdConfig(PCD_TYPE_14443B);

	PcdSetTmo(g_sPcdPara.cFWI);
	g_sPcdTrsv.cData[0] = 0x05;							// APf code
	g_sPcdTrsv.cData[1] = AFI;							// AFI
	g_sPcdTrsv.cData[2] = ((cReq<<3)&0x08)|(N&0x07);	// PARAM
	g_sPcdTrsv.iLength  = 0x03;
	
	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);

	if (cStatus == ERR_SUCCESS && g_sPcdTrsv.iLength == 0x0C)
	{
		memcpy(cpATQB, g_sPcdTrsv.cData, g_sPcdTrsv.iLength);
		//PcdSetTmo(cpATQB[11]>>4);			// set FWT
	}
	else
	{
		cStatus = ERR_FAILURE;
	}
	
	return cStatus;
}
//******************************************************************************
// FUNCTION: PiccAttrib
//       IN: unsigned char PARAM3
//           unsigned char cFSDI
//      OUT: unsigned char *PUPI
//           unsigned char *answer			// 1�ֽ�
//   RETURN: void
//  COMMENT: Type B ����ײ
//******************************************************************************
unsigned char PiccAttrib(unsigned char *PUPI, unsigned char PARAM3,unsigned char cFSDI, unsigned char *answer)
{
	unsigned char cStatus;
	
	PcdSetTmo(g_sPcdPara.cFWI);

	g_sPcdTrsv.cData[0] = 0x1D;
	memcpy(&g_sPcdTrsv.cData[1], PUPI, 4);
	g_sPcdTrsv.cData[5] = 0x00;						// EOF/SOF required, default TR0/TR1
	g_sPcdTrsv.cData[6] = cFSDI;					// Max frame 64 
	g_sPcdTrsv.cData[7] = PARAM3;					// Param3, ISO/IEC 14443-4 compliant?
	g_sPcdTrsv.cData[8] = 0x00;						// CID
	g_sPcdTrsv.iLength  = 0x09;

	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
	*answer = g_sPcdTrsv.cData[0];

	return cStatus;
}
//******************************************************************************
// FUNCTION: PiccHaltB
//       IN: unsigned char *PUPI
//      OUT: void
//   RETURN: void
//  COMMENT:
//******************************************************************************
unsigned char PiccHaltB(unsigned char *PUPI)
{
	unsigned char cStatus;

	g_sPcdTrsv.cData[0] = 0x50;
	memcpy(&g_sPcdTrsv.cData[1], PUPI, 4);
	g_sPcdTrsv.iLength  = 0x05;

	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);

	return cStatus;
}

//******************************************************************************
// FUNCTION: ISO14443B_Rst
//       IN: unsigned char cReq			// bReq meaning 0x00 = REQB; 0x01 =  WUPB;
//      OUT: unsigned char *pLen
//           unsigned char *pATQB
//   RETURN: void
//  COMMENT:
//******************************************************************************
unsigned char ISO14443B_Rst(unsigned char cReq, unsigned char *pLen, unsigned char *pATQB)
{
	unsigned char cStatus;
	unsigned char cFSCI;
	//unsigned char cTemp;
	
	g_sPcdPara.cFWI = 0x05;				// ����Ĭ��
	g_sPcdPara.cFSD = 0x40;				// ����Ĭ�϶���������
	g_sPcdPara.cCID = 0x00;
	g_sPcdPara.cNAD = 0x00;
	g_sPcdPara.cPCB = 0x00;
	
	cStatus = PiccRequestB(cReq, 0, 0, pATQB);
	if (cStatus == ERR_SUCCESS)
	{
		cFSCI = pATQB[10] >> 4;
		g_sPcdPara.cFWI = pATQB[11] >> 4;
		if (pATQB[11] & 0x01)
		{
			g_sPcdPara.cCID = 0x08;
		}
		if (pATQB[11] & 0x02)
		{
			g_sPcdPara.cNAD = 0x04;
		}
		PiccCaluINF(cFSCI);

		//cStatus = PiccAttrib(&pATQB[1], pATQB[10] & 0x0F, cFSCI, &pATQB[12]);
		cStatus = PiccAttrib(&pATQB[1], 0x01, cFSCI, &pATQB[12]);
		if(cStatus == ERR_SUCCESS)
		{
			*pLen = 12;
		}
	}
	return cStatus;
}
//******************************************************************************
//* MODULE: Mifare Card Support Function
//******************************************************************************

//******************************************************************************
//	 	IN: unsigned char cBlk
//			unsigned char cMode				// PICC_AUTHENT1A; PICC_AUTHENT1B
//			unsigned char cKType
//			unsigned char *cKey
//	RETURN: void
// Comment: 
//
//******************************************************************************
unsigned char PiccAuthState(unsigned char cBlk, unsigned char cMode, unsigned char cKType, unsigned char *cKey)
{
	unsigned char cStatus;
	PcdSetTmo(5);
	
	memcpy(&g_sPcdTrsv.cData, cKey, 6);
	g_sPcdTrsv.iLength = 6;
	cStatus = PcdTransceive(PCD_CMD_LOADKEY);
	
	if (cStatus == ERR_SUCCESS)
	{
		if (cMode & 0x80)
		{
			g_sPcdTrsv.cData[0] = PICC_AUTHENT1B;
		}
		else
		{
			g_sPcdTrsv.cData[0] = PICC_AUTHENT1A;
		}
		g_sPcdTrsv.cData[1] = cBlk;
		memcpy(&g_sPcdTrsv.cData[2], g_sCard.cUID, 4);
		g_sPcdTrsv.iLength = 6;

		cStatus = PcdTransceive(PCD_CMD_MFAUTHENT);
	}

	if(cStatus == ERR_SUCCESS)
	{
		//cStatus = ReadRawRC(JREG_STATUS2);		// WAIT ADD

		if(ReadRawRC(PHHAL_HW_RC663_REG_STATUS) & 0x20)
		{
			cStatus = ERR_SUCCESS;
		}
		else
		{
			cStatus = MI_AUTHERR;
		}
	}
	return cStatus;
}
//******************************************************************************
//       IN: unsigned char cAddr			// cAddr = 0--63	Mifare S50
//           								// cAddr = 0--255	Mifare S70
//           								// cAddr = 0--15	UltraLight
//      OUT: unsigned char *cpData			// ��ȷ���س��ȶ���16�ֽ�
//   RETURN: unsigned char cStatus
//  COMMENT:
//******************************************************************************
unsigned char PiccRead(unsigned char cAddr, unsigned char *cpData)
{
	unsigned char cStatus;
	PcdSetTmo(5);

	WriteRawRC(PHHAL_HW_RC663_REG_TXCRCCON, 0x19);
	WriteRawRC(PHHAL_HW_RC663_REG_RXCRCCON, 0x19);
	WriteRawRC(PHHAL_HW_RC663_REG_TXDATANUM,0x08);
	WriteRawRC(PHHAL_HW_RC663_REG_RXBITCTRL,0x00);

	g_sPcdTrsv.cData[1] = cAddr;				// ����������
	g_sPcdTrsv.cData[0] = PICC_READ;
	g_sPcdTrsv.iLength  = 2;
	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
	if (cStatus == ERR_SUCCESS)
	{
		if (g_sPcdTrsv.iLength != 0x10)
		{
			cStatus = MI_BYTECOUNTERR;
		}
		else
		{
			memcpy(cpData, g_sPcdTrsv.cData, 0x10);  // 0x10 == 16
		}
	}
	return cStatus;
}
//******************************************************************************
//       IN: unsigned char cBlock
//      OUT: unsigned char *cpData
//   RETURN: unsigned char
//  Comment:
//******************************************************************************
unsigned char PiccWrite(unsigned char cBlock, unsigned char *cpData)
{
	unsigned char cStatus;

	PcdSetTmo(5);

	WriteRawRC(PHHAL_HW_RC663_REG_RXCRCCON, 0x18);
	WriteRawRC(PHHAL_HW_RC663_REG_TXCRCCON, 0x19);
	
	g_sPcdTrsv.cData[0] = PICC_WRITE;
	g_sPcdTrsv.cData[1] = cBlock;
	g_sPcdTrsv.iLength  = 2;
	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);

	if (cStatus == MI_SERNRERR)
	{
		g_sPcdTrsv.cData[0] &= 0x0F;
		if (g_sPcdTrsv.cData[0] == 0x0A)
		{
			cStatus = ERR_SUCCESS;
		}
		else
		{
			cStatus = MI_WRITEERR;
		}
	}

	if (cStatus == ERR_SUCCESS)
	{
		PcdSetTmo(5);
		memcpy(g_sPcdTrsv.cData, cpData, 0x10); // 0x10 == 16
		g_sPcdTrsv.iLength = 0x10;
		cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
		if (cStatus == MI_SERNRERR)
		{
			g_sPcdTrsv.cData[0] &= 0x0F;
			if (g_sPcdTrsv.cData[0] == 0x0A)
			{
				cStatus = ERR_SUCCESS;
			}
			else
			{
				cStatus = MI_WRITEERR;
			}
		}
	}
	return cStatus;
}

//******************************************************************************
//       IN: unsigned char cFunc       // 0xC1 = Increment Value;
//                                     // 0xC0 = Decrement Value;
//           unsigned char cBlock
//           unsigned char *cpValue
//      OUT: void
//   RETURN: unsigned char cStatus
//  COMMENT: Ǯ�����ܺ���
//******************************************************************************
unsigned char PiccValue(unsigned char cFunc, unsigned char cBlock,unsigned char *cpValue)
{
	unsigned char cStatus;
	PcdSetTmo(5);
	
	WriteRawRC(PHHAL_HW_RC663_REG_RXCRCCON, 0x18);
	WriteRawRC(PHHAL_HW_RC663_REG_TXCRCCON, 0x19);

	g_sPcdTrsv.cData[0] = cFunc;
	g_sPcdTrsv.cData[1] = cBlock;
	g_sPcdTrsv.iLength  = 2;
	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);

	if (cStatus == MI_SERNRERR)
	{
		g_sPcdTrsv.cData[0] &= 0x0F;
		if (g_sPcdTrsv.iLength == 0x01 && g_sPcdTrsv.cData[0] == 0x0A)
		{
			PcdSetTmo(5);
			memcpy(g_sPcdTrsv.cData, cpValue, 0x04);
			g_sPcdTrsv.iLength = 0x04;
			cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
	
			if (cStatus == ERR_SUCCESS || cStatus == MI_NOTAGERR)
			{
				cStatus = PiccTransfer(cBlock);
			}
		}
	}
	return cStatus;
}
//******************************************************************************
//	 	IN: unsigned char cBlock
//	RETURN: unsigned char
// Comment: 
//
//******************************************************************************
unsigned char PiccRestore(unsigned char cBlock)
{
	unsigned char cStatus;

	PcdSetTmo(5);
	
	WriteRawRC(PHHAL_HW_RC663_REG_RXCRCCON, 0x18);
	WriteRawRC(PHHAL_HW_RC663_REG_TXCRCCON, 0x19);
	
	g_sPcdTrsv.cData[0] = PICC_RESTORE;
	g_sPcdTrsv.cData[1] = cBlock;
	g_sPcdTrsv.iLength  = 2;

	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
	if (cStatus == MI_SERNRERR)
	{
		g_sPcdTrsv.cData[0] &= 0x0F;
		if (g_sPcdTrsv.iLength == 0x01 && g_sPcdTrsv.cData[0] == 0x0A)
		{
			PcdSetTmo(5);
			g_sPcdTrsv.cData[0] = 0x00;
			g_sPcdTrsv.cData[1] = 0x00;
			g_sPcdTrsv.cData[2] = 0x00;
			g_sPcdTrsv.cData[3] = 0x00;
			g_sPcdTrsv.iLength  = 0x04;

			cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
			if (cStatus == MI_NOTAGERR)
			{
				cStatus = ERR_SUCCESS;
			}
		}
	}
	return cStatus;
}
//******************************************************************************
//	 	IN: unsigned char cBlock
//	RETURN: unsigned char
// Comment: ִ��Ǯ�����ܲ���
//
//******************************************************************************
unsigned char PiccTransfer(unsigned char cBlock)
{
	unsigned char cStatus;

	PcdSetTmo(5);
	
	WriteRawRC(PHHAL_HW_RC663_REG_RXCRCCON, 0x18);
	WriteRawRC(PHHAL_HW_RC663_REG_TXCRCCON, 0x19);

	g_sPcdTrsv.cData[0] = PICC_TRANSFER;
	g_sPcdTrsv.cData[1] = cBlock;
	g_sPcdTrsv.iLength  = 2;

	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
	if (cStatus == MI_SERNRERR)
	{
		g_sPcdTrsv.cData[0] &= 0x0F;
		if (g_sPcdTrsv.iLength == 0x01 && g_sPcdTrsv.cData[0] == 0x0A)
		{
			cStatus = ERR_SUCCESS;
		}
	}
	return cStatus;
}

//******************************************************************************
//	   OUT: unsigned char *cpLen
//			unsigned char *pATQB
//	RETURN: void
// Comment: ���֤ Ѱ������
//			
//******************************************************************************
unsigned char IDCard_Reqest(unsigned short *ipLen, unsigned char *pATQB)
{
	unsigned char cStatus;
	//unsigned char cFSCI;
	//unsigned char cTemp;
	
	g_sPcdPara.cFWI = 0x04;				// ����Ĭ��
	g_sPcdPara.cFSD = 0x40;				// ����Ĭ�϶���������
	g_sPcdPara.cCID = 0x08;
	g_sPcdPara.cNAD = 0x00;
	g_sPcdPara.cPCB = 0x00;
	
	cStatus = PiccRequestB(0, 0, 0, pATQB);
	if (cStatus == ERR_SUCCESS)
	{
//		cFSCI = pATQB[10] >> 4;
		g_sPcdPara.cFWI = pATQB[11] >> 4;
//		if (pATQB[11] & 0x01)
//		{
//			g_sPcdPara.cCID = 0x08;
//		}
//		if (pATQB[11] & 0x02)
//		{
//			g_sPcdPara.cNAD = 0x04;
//		}
//		PiccCaluINF(cFSCI);

		cStatus = PiccAttrib(&pATQB[1], 0x01, 0x08, &pATQB[12]);
		if(cStatus == ERR_SUCCESS)
		{
			*ipLen = 12;
		}
	}
	return cStatus;

}
//******************************************************************************
//	IN OUT: unsigned char *cpLen		// ����
//			unsigned char *cpAPDU		// Ӧ�����ݵ�Ԫ
//	RETURN: unsigned char
// Comment: ���֤ APDU���书��
//			
//******************************************************************************
unsigned char IDCard_APDU(unsigned short *ipLen, unsigned char *cpAPDU)
{
	unsigned char cStatus;
	
	g_sPcdTrsv.iLength = *ipLen;
	memcpy(g_sPcdTrsv.cData, cpAPDU, g_sPcdTrsv.iLength);

	cStatus = PcdTransceive(PCD_CMD_TRANSCEIVE);
	
	if (cStatus == ERR_SUCCESS)
	{
		memcpy(cpAPDU, g_sPcdTrsv.cData, g_sPcdTrsv.iLength);
		*ipLen = g_sPcdTrsv.iLength;
	}
	return cStatus;
}


