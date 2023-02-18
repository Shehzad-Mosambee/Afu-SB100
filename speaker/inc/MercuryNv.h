/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author: 
#########################################################################*/

#ifndef __MERCURY_NV_H__
#define __MERCURY_NV_H__
#include "MercuryDef.h"

//nv item id form 0 to 499
#define NV_ITEM_NUM_MAX   500
#define NV_APPINFO_LEN          640

typedef struct
{
    uint32 startaddr;
    uint32 size;
}NV_CONGIG_T;

extern int NV_Init(void);
extern int NV_Deinit(void);
extern int NV_Read(WORD ItemID,WORD cchSize, BYTE *pBuf);
extern int NV_Write(WORD ItemID, WORD cchSize, BYTE *pBuf);
extern int NV_Delete(WORD ItemID);
extern int NV_AppInfoRead(void* pAppInfo, DWORD len);
extern int NV_AppInfoWrite(void* pAppInfo, DWORD len);
extern int NV_GetItemidDataLen(WORD ItemID);


/*************************************************************************************
 * 函数名：MC_NV_Config
 * 函数功能：这个函数很重要，主函数里面必须放在第一个调用(适用于AG25平台)

 * 返回  
 *************************************************************************************/
extern BOOLEAN NV_Config(NV_CONGIG_T *pConfig);


/***************************************
set sysnv,
param[in] nv_item: support
    "ap_trace_en" : "0.disable; 1.enable"
    "diag_device" : "1.diag device uart; 2.diag device usb serial";
    "usbmode" :
        "0.Charger only; 1.RDA (8910 ROM) serial; "
        "2.Eight serials; 3.RNDIS and serials; "
        "4.SPRD U2S Diag; 5.ECM and serials; "
	    "6.only user serials; 7.RNDIS and user serials "
	    "8.ECM and user serials";
    "nat_cfg":
        "0 disable; 16bit config nat cid; 101 =0000000100100101 sim 0 cid(1,3,6 enable)(2,4,5,7,8 disable) sim1 cid(1 enable)(2,3,4,5,6,7,8 disable)";
param[in] value 
*****************************************/
int SYSNV_Set(char *nv_item, uint8_t value);


/***************************************
set sysnv,
param[in] nv_item
param[out] value 
*****************************************/
int SYSNV_Get(char *nv_item, uint8_t *value);

//reset all sysnv
void SYSNV_Clear(void);

#endif //__MERCURY_NV_H__

