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
* 	???? SIM ????????????
* 
* Parameters:
* 	sim_status	  	[out] 	???? SIM ?????????????? MC_SIM_STATUS 
* Return:
* 	0				??????
*	other value		??????
*
*****************************************************************/
extern int MercurySimGetStatus(MC_SIM_STATUS *sim_status);   

/*****************************************************************
* Description:
* 	?? SIM ?????????? PIN(MC_SIM_STATUS_WAIT_PIN)?????? PIN???????????? 
* 
* Parameters:
* 	pt_info	  			[in] 	PIN ?????? MC_SIM_VERIFY_PIN_INFO?? 
* Return:
* 	0				??????
*	other value		??????
*
*****************************************************************/
extern int MercurySimVerifyPin(MC_SIM_VERIFY_PIN_INFO  *pin_info);

/*****************************************************************
* Description:
* 	???? SIM ???? PIN ??
* 
* Parameters:
* 	pt_info	  			[in] 	???????? PIN ?????? PIN???? MC_SIM_CHANGE_PIN_INFO??  
* Return:
* 	0				??????
*	other value		??????
*
*****************************************************************/
int MercurySimChangePin(MC_SIM_CHANGE_PIN_INFO  *pin_info);

/*****************************************************************
* Function: ql_sim_unblock_pin
*
* Description:
* 	?????????????? PIN ??????SIM ???????????? PUK ????
*	???? PUK ???????? PIN ????????????
* 
* Parameters: 
* 	pt_info	  			[in] 	???? PUK ???????? PIN ?????? MC_SIM_UNBLOCK_PIN_INFO??  
* Return:
* 	0				??????
*	other value		??????
*
*****************************************************************/
int MercurySimUnblockPin(MC_SIM_UNBLOCK_PIN_INFO  *pin_info);

/*****************************************************************
* Description:
* 	???? SIM ?? PIN ???????? 
* 
* Parameters: 
* 	pin_info			  	[in] 	???? PIN ?????? MC_SIM_VERIFY_PIN_INFO?? 
* Return:
* 	0				??????
*	other values		??????
*
*****************************************************************/
extern int MercurySimEnablePin(MC_SIM_VERIFY_PIN_INFO  *pin_info);

/*****************************************************************
* Description:
* 	???? SIM ?? PIN ??????
* 
* Parameters:
* 	pin_info	  			[in] 	???? PIN ?????? MC_SIM_VERIFY_PIN_INFO?? 
* Return:
* 	0				??????
*	other values		??????
*
*****************************************************************/
extern int MercurySimDisablePin(MC_SIM_VERIFY_PIN_INFO	*pin_info);


/*****************************************************************
* Function: ql_sim_get_phonenumber
*
* Description:
* 	???? SIM ???????????? 
* 
* Parameters:
* 	num	  		[out] 	SIM ????????????????????  
* 	len	  	[in] 	num ????????????  
* Return:
* 	0				??????
*	other values		??????
*
*****************************************************************/
int MercurySimGetPhoneNumber(char *num,uint8 len);	

/*****************************************************************
* Function: MercurySimSetForceDectect
*
* Description:
* 	??????????????????????????????
* 
* Parameters:
* 	enable  	[in] 	TRUE: ????????????sim???? FALSE: ????
* Return: void
* Note: ????????????????????????sim hotplug??????????????????
*		????????????????:
*       MercurySimGetPhoneNumber(TRUE);
* 		FlightModeSet(TRUE);
* 		FlightModeSet(FALSE);
*****************************************************************/
void MercurySimSetForceDectect(BOOLEAN enable);

#endif //__MERCURY_SIM_H__
