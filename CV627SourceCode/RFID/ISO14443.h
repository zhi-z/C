/*******************************************************************************
 * FileName: ISO14443.h
 *   Author: Layne Wen
 *     Date: 2017.02.22
 *  COMMENT: �汾���ڸ���
 ******************************************************************************/
#ifndef ISO14443_H
#define ISO14443_H

//------------------------------------------------------------------------------
// ISO14443-3 Command
//------------------------------------------------------------------------------
#define PICC_REQIDL			0x26		// Ѱ��������δ��������״̬�Ŀ�
#define PICC_REQALL			0x52		// Ѱ��������ȫ����
#define PICC_ANTICOLL1		0x93		// һ������ײ
#define PICC_ANTICOLL2		0x95		// ��������ײ
#define PICC_ANTICOLL3		0x97		// ��������ײ
#define PICC_HALT			0x50		// ֹͣ

//------------------------------------------------------------------------------
// ISO14443-4 Command
//------------------------------------------------------------------------------
#define PICC_RESET			0xE0		// ��λ����
#define PICC_PPSS			0xD0		// 

//------------------------------------------------------------------------------
// MFIRE�������ֶ���
//------------------------------------------------------------------------------
#define PICC_AUTHENT1A		0x60		// ��֤A��Կ
#define PICC_AUTHENT1B		0x61		// ��֤B��Կ
#define PICC_READ			0x30		// ����
#define PICC_WRITE			0xA0		// д��
#define PICC_DECREMENT		0xC0		// �ۿ�
#define PICC_INCREMENT		0xC1		// ��ֵ
#define PICC_RESTORE		0xC2		// ��Ǯ����������
#define PICC_TRANSFER		0xB0		// ���滺����������

//------------------------------------------------------------------------------
// Mifare��Կ��Դ����
//------------------------------------------------------------------------------
#define KEY_CURRENT			0x01
#define KEY_E2PROM			0x02

//------------------------------------------------------------------------------
// Type Define
//------------------------------------------------------------------------------
typedef struct
{
	unsigned char cUID[11];
	unsigned char cATQ[2];
	unsigned char cSAK;
	unsigned char cLen;
} tpd_Card;

//------------------------------------------------------------------------------
// Extern Functions Define
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ISO14443-3 Type A functions define
//------------------------------------------------------------------------------
void          PiccReset(void);
unsigned char PiccRequest(unsigned char cReq,unsigned char *cATQA);						// Ѱ��
unsigned char PiccAnticoll(unsigned char cLev, unsigned char *cUID);					// ����ײ
unsigned char PiccSelect(unsigned char cLev, unsigned char *cUID,unsigned char *cSAK);	// ѡ��һ�ſ�
unsigned char PiccHaltA(void);															// ������
unsigned char PiccAnticollSelect(unsigned char *cLen, unsigned char *cUID, unsigned char *cSAK);

//------------------------------------------------------------------------------
// ISO14443-3 Type B functions define
//------------------------------------------------------------------------------
unsigned char PiccRequestB(unsigned char cReq,unsigned char AFI,unsigned char N,unsigned char *cpATQB);
unsigned char PiccAttrib(unsigned char *PUPI, unsigned char PARAM3,unsigned char cFSDI, unsigned char *answer);
unsigned char PiccHaltB(unsigned char *PUPI);

//------------------------------------------------------------------------------
// ISO14443-4 Type A&B CPU card functions define
//------------------------------------------------------------------------------
unsigned char PiccTypeA_Rst(unsigned char *cpLen, unsigned char *cpData);

unsigned char ISO14443A_Rst(unsigned char *cpLen, unsigned char *cpRATS);

unsigned char ISO14443_COS(unsigned short *pLen, unsigned char *pAPDU);



//--------------------------------------------------------------------------------------------------
// Mifare S50/S70 card functions define
//--------------------------------------------------------------------------------------------------
unsigned char PiccAuthState(unsigned char cBlk, unsigned char cMode, unsigned char cKType, unsigned char *cKey);// ��֤��Կ

unsigned char PiccRead(unsigned char cAddr, unsigned char *cpData);	// ����
unsigned char PiccWrite(unsigned char cBlk, unsigned char *cData);	// д��
unsigned char PiccValue(unsigned char cFct,unsigned char cBlk, unsigned char *cValue);	// ��ֵ�ۿ�
unsigned char PiccRestore(unsigned char cBlk);											// ����
unsigned char PiccTransfer(unsigned char cBlk);											// ����


//------------------------------------------------------------------------------
// ID Card COS
//------------------------------------------------------------------------------
unsigned char IDCard_Reqest(unsigned short *ipLen, unsigned char *pATQB);
unsigned char IDCard_APDU(unsigned short *ipLen, unsigned char *cpAPDU);


#endif

