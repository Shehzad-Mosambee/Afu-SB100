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
* 	获取 SIM 卡状态信息。
* 
* Parameters:
* 	sim_status	  	[out] 	返回 SIM 卡类型信息，见 MC_SIM_STATUS 
* Return:
* 	0				成功。
*	other value		失败。
*
*****************************************************************/
extern int MercurySimGetStatus(MC_SIM_STATUS *sim_status);   

/*****************************************************************
* Description:
* 	当 SIM 状态为请求 PIN(MC_SIM_STATUS_WAIT_PIN)，输入 PIN，进行验证。 
* 
* Parameters:
* 	pt_info	  			[in] 	PIN 码，见 MC_SIM_VERIFY_PIN_INFO。 
* Return:
* 	0				成功。
*	other value		失败。
*
*****************************************************************/
extern int MercurySimVerifyPin(MC_SIM_VERIFY_PIN_INFO  *pin_info);

/*****************************************************************
* Description:
* 	更改 SIM 卡的 PIN 码
* 
* Parameters:
* 	pt_info	  			[in] 	输入旧的 PIN 和新的 PIN，见 MC_SIM_CHANGE_PIN_INFO。  
* Return:
* 	0				成功。
*	other value		失败。
*
*****************************************************************/
int MercurySimChangePin(MC_SIM_CHANGE_PIN_INFO  *pin_info);

/*****************************************************************
* Function: ql_sim_unblock_pin
*
* Description:
* 	当多次错误输入 PIN 码后，SIM 卡状态为请求 PUK 时，
*	输入 PUK 码和新的 PIN 码进行解锁。
* 
* Parameters: 
* 	pt_info	  			[in] 	输入 PUK 码和新的 PIN 码，见 MC_SIM_UNBLOCK_PIN_INFO。  
* Return:
* 	0				成功。
*	other value		失败。
*
*****************************************************************/
int MercurySimUnblockPin(MC_SIM_UNBLOCK_PIN_INFO  *pin_info);

/*****************************************************************
* Description:
* 	启用 SIM 卡 PIN 码验证。 
* 
* Parameters: 
* 	pin_info			  	[in] 	输入 PIN 码，见 MC_SIM_VERIFY_PIN_INFO。 
* Return:
* 	0				成功。
*	other values		失败。
*
*****************************************************************/
extern int MercurySimEnablePin(MC_SIM_VERIFY_PIN_INFO  *pin_info);

/*****************************************************************
* Description:
* 	关闭 SIM 卡 PIN 码验证
* 
* Parameters:
* 	pin_info	  			[in] 	输入 PIN 码，见 MC_SIM_VERIFY_PIN_INFO。 
* Return:
* 	0				成功。
*	other values		失败。
*
*****************************************************************/
extern int MercurySimDisablePin(MC_SIM_VERIFY_PIN_INFO	*pin_info);


/*****************************************************************
* Function: ql_sim_get_phonenumber
*
* Description:
* 	获取 SIM 卡本机号码。 
* 
* Parameters:
* 	num	  		[out] 	SIM 卡本机号码，字符串。  
* 	len	  	[in] 	num 数组的长度。  
* Return:
* 	0				成功。
*	other values		失败。
*
*****************************************************************/
int MercurySimGetPhoneNumber(char *num,uint8 len);	

/*****************************************************************
* Function: MercurySimSetForceDectect
*
* Description:
* 	设置退出飞行模式时是否强制检卡
* 
* Parameters:
* 	enable  	[in] 	TRUE: 开启强制检测sim卡； FALSE: 关闭
* Return: void
* Note: 针对有些客户硬件上不支持sim hotplug，但需要开机后插卡
*		能检卡。处理流程:
*       MercurySimGetPhoneNumber(TRUE);
* 		FlightModeSet(TRUE);
* 		FlightModeSet(FALSE);
*****************************************************************/
void MercurySimSetForceDectect(BOOLEAN enable);

#endif //__MERCURY_SIM_H__
