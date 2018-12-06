/*******************************************************************************
 * FileName: Status.h
 *   Target: 
 *  Comment: �������壬�豸״ָ̬ʾ��
 *
 * Revision: V1.00
 *   Author: Layne Wen
 *     Date: 1.9.2010
 ******************************************************************************/
#ifndef STATUS_H
#define STATUS_H

//#define TRUE			1
//#define FALSE			0

#define YES				1
#define NO				0

#define HIGH			1
#define LOW				0

#define ON				1
#define OFF				0


//------------------------------------------------------------------------------
// ����״̬
//------------------------------------------------------------------------------
#define ERR_SUCCESS					0x00		// �ɹ�

#define ERR_FAILURE					0xE0		// ʧ��
#define ERR_LENGTH					0xE1		// ���ȴ���	ERR_LENGTH
#define ERR_PARA					0xE2		// ��������	ERR_PARAMETER
#define ERR_TIMEOUT					0xE3		// ��ʱ����
#define ERR_EXECUTE					0xE4		// ִ�д���	EXECUTE
#define ERR_CRC						0xE5		// CRC����
#define ERR_CMD						0xE6		// �������
#define ERR_VERIFY					0xE7		// У�����
#define ERR_ADDRESS					0xE8		// ��ַ����
#define ERR_SN_MIS					0xE9		// ��Ų�ƥ��
#define ERR_COMM					0xEF		// ͨ�Ŵ���	ERR_COMMUNICATION




//------------------------------------------------------------------------------
// ����״̬
//------------------------------------------------------------------------------
#define ALM_ALARM					0xA0		// ����
#define ALM_NOSAVE					0xA1		// û�б���
#define ALM_REPEAT					0xA3		// �ظ�ִ��		ALM = Alarm


//------------------------------------------------------------------------------
// Modbus Exception Code
//------------------------------------------------------------------------------
#define EXCEP_FUNCTION				0x01		// Function code not supported
#define EXCEP_ADDRESS				0x02		// Starting Address == OK  AND Starting Address + Quantity of Inputs == OK
#define EXCEP_QUANTITY				0x03		// 0x0001 < Quantity of Registers < 0x007D
#define EXCEP_DISCRETE				0x04		// ReadDiscreteOutputs == OK


#ifndef NULL
#define NULL						((void *)(0x0000))
#endif


#endif
