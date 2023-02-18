#ifndef _MERCURY_PBK_H_
#define _MERCURY_PBK_H_

#include "MercuryDef.h"

#define SIM_PBK_NAME_SIZE 50 
#define SIM_PBK_NUMBER_SIZE 10

// PBK Configuration
#define MC_PBK_SIM 0x01
#define MC_PBK_ME 0x02

#define MC_PBK_ME_MISSED 0x1
#define MC_PBK_ME_RECEIVED 0x2
#define MC_PBK_SIM_FIX_DIALLING 0x4
#define MC_PBK_SIM_LAST_DIALLING 0x8
#define MC_PBK_ME_LAST_DIALLING 0x10
#define MC_PBK_SIM_BARRED_DIALLING 0x11
#define MC_PBK_SIM_SERVICE_DIALLING 0x12
#define MC_PBK_ON 0x20
#define MC_PBK_EN 0x40
#define MC_PBK_SDN 0x80

#define MC_PBK_AUTO 0x00

#define MC_PHONE_NUM_STRING_LEN  (SIM_PBK_NUMBER_SIZE * 2 + 4)
#define MC_PBK_NAME_SIZE         SIM_PBK_NAME_SIZE

typedef struct _MC_SIM_PBK_ENTRY_INFO
{
    uint8 pNumber[MC_PHONE_NUM_STRING_LEN];
    uint8 pFullName[MC_PBK_NAME_SIZE];
    uint8 iFullNameSize;
    uint8 nNumberSize;
    uint8 nType;
    uint16 phoneIndex;
    uint8 *pRecordData;
} MC_SIM_PBK_ENTRY_INFO;

/**
* @brief ��ȡ�绰����ָ��λ�õļ�¼
*
* @param[in] storage �绰���洢����
* @param[in] startindex Ҫ��ȡ��¼�Ŀ�ʼ����
* @param[in] endindex Ҫ��ȡ��¼�Ľ�������,���ֻ��һ����¼,�����startindex
* @return 0 SUCCESS; -1 FAIL
   ֻҪ������ȷ�ú�������TRUE�� �Ƿ��ȡ�ɹ� MC_RegNotifyCallbackע���callback �����е� 
   NOTIFY_CLASS_PBK ���¼�����
*/
int PhoneBookRead(uint8 storage, uint8 starindex, uint8 endindex);

/**
* @brief ɾ��ָ��λ�õĵ绰����¼
*
* @param[in] storage �绰���洢����
* @param[in] index Ҫɾ����¼������
* @return 0 SUCCESS; -1 FAIL
   ֻҪ������ȷ�ú�������TRUE�� �õ��ĵ绰��¼��Ϣ�� MC_RegNotifyCallbackע���callback �����е�
   NOTIFY_CLASS_PBK���¼����� ID NOTIFY_ID_PBK_DELETE_OK��ʾ�ɹ�
*/

int PhonebookDelete(uint8 storage,uint32 index);

/**
* @brief �ѵ绰��¼д��ָ��λ��
*
* @param[in] storage �绰���洢����
* @param[in] entry_ptr �绰��¼����
* @param[in] index Ҫд���¼������
* @return TRUE SUCCESS; FALSE FAIL
 ֻҪ������ȷ�ú�������TRUE�� �Ƿ���ӳɹ� MC_RegNotifyCallbackע���callback �����е�
 NOTIFY_CLASS_PBK  ���¼�����NOTIFY_ID_PBK_WRITE_OK ��д��ɹ�
*/
BOOL PhonebookWrite(uint8 storage, MC_SIM_PBK_ENTRY_INFO* entry_ptr,uint32 index);


#endif

