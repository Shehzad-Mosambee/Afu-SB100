/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author:
#########################################################################*/

#ifndef __MERCURY_SIM_H__
#define __MERCURY_SIM_H__
#include "MercuryDef.h"

typedef struct{
	int type;      		//0: read binary; 1: read record; 2: get response
	int result;			//0: success; other value: fail
	uint8 pstring[1];	//store data in string format for read binary/record and get response, the length is variable.
}MC_SIM_CRSM_RESP_T;

typedef struct{
	BOOLEAN enable;				//0: disable; 1:enable note: if disable, the plugin_level and detect_gpio are ignore
	BOOLEAN plugin_level;		//detect sim plugin level, 0: low; 1: high 
	uint8 detect_gpio;			//detect gpio
}MC_SIM_HOTPLUG_CFG_T;

#define MC_SIM_PIN_LEN_MAX  16   //Maximum length of PIN data
typedef struct 
{
    uint8 pin_value[MC_SIM_PIN_LEN_MAX];  /*  Value of the PIN */
}MC_SIM_VERIFY_PIN_INFO;  

typedef struct 
{
    uint8_t	old_pin_value[MC_SIM_PIN_LEN_MAX];  /**< Value of the old PIN as a sequence of ASCII characters. */
    uint8_t new_pin_value[MC_SIM_PIN_LEN_MAX];  /**< Value of the new PIN as a sequence of ASCII characters. */
}MC_SIM_CHANGE_PIN_INFO; 

typedef struct 
{
    uint8_t puk_value[MC_SIM_PIN_LEN_MAX];      /**< Value of the PUK as a sequence of ASCII characters. */
    uint8_t new_pin_value[MC_SIM_PIN_LEN_MAX];  /**< Value of the new PIN as a sequence of ASCII characters. */
}MC_SIM_UNBLOCK_PIN_INFO;  

typedef enum 
{
	MC_SIM_STATUS_NOT_INIT,
	MC_SIM_STATUS_NOT_INSERTED,
	MC_SIM_STATUS_READY,
   	MC_SIM_STATUS_WAIT_PIN,
	MC_SIM_STATUS_WAIT_PUK,
   	MC_SIM_STATUS_UNKNOWN
}MC_SIM_STATUS;    /**< Card state. */

extern int MercurySimSetHotplug(MC_SIM_HOTPLUG_CFG_T *cfg);
extern int MercurySimGetHotplug(MC_SIM_HOTPLUG_CFG_T *cfg);
extern int MercurySimCrsmReadBinary(uint32 nEFID, uint16 offset, uint8 length, uint32 nEFPath);
extern int MercurySimCrsmReadRecord(uint32 nEFID, uint16 offset, uint8 length, uint32 nEFPath);
extern int MercurySimCrsmGetResponse(uint32 nEFID, uint16 offset, uint8 length, uint32 nEFPath);


/*****************************************************************
* Description:
* 	��ȡ SIM ��״̬��Ϣ��
* 
* Parameters:
* 	sim_status	  	[out] 	���� SIM ��������Ϣ���� MC_SIM_STATUS 
* Return:
* 	0				�ɹ���
*	other value		ʧ�ܡ�
*
*****************************************************************/
extern int MercurySimGetStatus(MC_SIM_STATUS *sim_status);   

/*****************************************************************
* Description:
* 	�� SIM ״̬Ϊ���� PIN(MC_SIM_STATUS_WAIT_PIN)������ PIN��������֤�� 
* 
* Parameters:
* 	pt_info	  			[in] 	PIN �룬�� MC_SIM_VERIFY_PIN_INFO�� 
* Return:
* 	0				�ɹ���
*	other value		ʧ�ܡ�
*
*****************************************************************/
extern int MercurySimVerifyPin(MC_SIM_VERIFY_PIN_INFO  *pin_info);

/*****************************************************************
* Description:
* 	���� SIM ���� PIN ��
* 
* Parameters:
* 	pt_info	  			[in] 	����ɵ� PIN ���µ� PIN���� MC_SIM_CHANGE_PIN_INFO��  
* Return:
* 	0				�ɹ���
*	other value		ʧ�ܡ�
*
*****************************************************************/
int MercurySimChangePin(MC_SIM_CHANGE_PIN_INFO  *pin_info);

/*****************************************************************
* Function: ql_sim_unblock_pin
*
* Description:
* 	����δ������� PIN ���SIM ��״̬Ϊ���� PUK ʱ��
*	���� PUK ����µ� PIN ����н�����
* 
* Parameters: 
* 	pt_info	  			[in] 	���� PUK ����µ� PIN �룬�� MC_SIM_UNBLOCK_PIN_INFO��  
* Return:
* 	0				�ɹ���
*	other value		ʧ�ܡ�
*
*****************************************************************/
int MercurySimUnblockPin(MC_SIM_UNBLOCK_PIN_INFO  *pin_info);

/*****************************************************************
* Description:
* 	���� SIM �� PIN ����֤�� 
* 
* Parameters: 
* 	pin_info			  	[in] 	���� PIN �룬�� MC_SIM_VERIFY_PIN_INFO�� 
* Return:
* 	0				�ɹ���
*	other values		ʧ�ܡ�
*
*****************************************************************/
extern int MercurySimEnablePin(MC_SIM_VERIFY_PIN_INFO  *pin_info);

/*****************************************************************
* Description:
* 	�ر� SIM �� PIN ����֤
* 
* Parameters:
* 	pin_info	  			[in] 	���� PIN �룬�� MC_SIM_VERIFY_PIN_INFO�� 
* Return:
* 	0				�ɹ���
*	other values		ʧ�ܡ�
*
*****************************************************************/
extern int MercurySimDisablePin(MC_SIM_VERIFY_PIN_INFO	*pin_info);


/*****************************************************************
* Function: ql_sim_get_phonenumber
*
* Description:
* 	��ȡ SIM ���������롣 
* 
* Parameters:
* 	num	  		[out] 	SIM ���������룬�ַ�����  
* 	len	  	[in] 	num ����ĳ��ȡ�  
* Return:
* 	0				�ɹ���
*	other values		ʧ�ܡ�
*
*****************************************************************/
int MercurySimGetPhoneNumber(char *num,uint8 len);	

/*****************************************************************
* Function: MercurySimSetForceDectect
*
* Description:
* 	�����˳�����ģʽʱ�Ƿ�ǿ�Ƽ쿨
* 
* Parameters:
* 	enable  	[in] 	TRUE: ����ǿ�Ƽ��sim���� FALSE: �ر�
* Return: void
* Note: �����Щ�ͻ�Ӳ���ϲ�֧��sim hotplug������Ҫ������忨
*		�ܼ쿨����������:
*       MercurySimGetPhoneNumber(TRUE);
* 		FlightModeSet(TRUE);
* 		FlightModeSet(FALSE);
*****************************************************************/
void MercurySimSetForceDectect(BOOLEAN enable);

#endif //__MERCURY_SIM_H__
