#include "MercuryDef.h"
#include "MercuryCSTD.h"
#include "MercuryPlat.h"
#include "MercuryNv.h"
#include "MercuryCom.h"

#include "vendor.h"
#include "nv.h"
#include "speakerapp.h"

// APP usb write
int app_UsbWrite(uint8 *pSendBuff, uint32 nSendBuffLen)
{
	uint32 bytesWrite = 0;
	VirComWrite(PORT_ID, pSendBuff, nSendBuffLen, &bytesWrite);
    return bytesWrite;
}

// 获取设备SN
int app_at_get_SN(char *data, int len)
{
    char snData[32] = {0};
    char SN[64] = {0};
	NV_Read(NV_SN, 32, snData);

    if (snData != NULL) {
        snprintf(SN, sizeof(SN), "SN:%s\r\n\n", snData);
        app_UsbWrite(SN, strlen(SN));
    }

	return 0;
}

// 获取设备SIM卡IMSI、ICCID
int app_at_get_simInfo(char *data, int len)
{
    SIM_IMSI_T imsi = {0};
    SIM_ICCID_T iccid = {0};

    char imsiBf[64] = {0}, iccidBf[64] = {0};

    GetSimInfo(SIM_IMSI_E,&imsi,NULL);
    GetSimInfo(SIM_CCID_E,NULL,&iccid);

	MercuryTrace("imsi = %s iccid = %s\r\n", imsi.imsi_val, iccid.id_num);
    
	if (imsi.imsi_val != NULL) {
        snprintf(imsiBf, sizeof(imsiBf), "IMSI:%s\r\n",imsi.imsi_val);
        app_UsbWrite(imsiBf,strlen(imsiBf));
    }

    if (iccid.id_num != NULL) {
        snprintf(iccidBf, sizeof(iccidBf), "ICCID:%s\r\n\n",iccid.id_num);
        app_UsbWrite(iccidBf,strlen(iccidBf));
    }

    return 0;
}

// 获取模块IMEI
int app_at_get_imei(char *data, int len)
{
    BYTE *imei = NULL;
    char imeiBf[32] = {0};

    imei = GetImei();

    if (NULL != imei) {
        snprintf(imeiBf, sizeof(imeiBf), "IMEI:%s\r\n\n", imei);
        app_UsbWrite(imeiBf,strlen(imeiBf));
    }

    return 0;
}

// 获取底包版本
int app_at_get_bottomBag_version(char *data, int len)
{
    BYTE *mercury_version = NULL;
    BYTE versionBf[512] = {0};

    mercury_version = GetMercuryVersion();

    if (mercury_version != NULL) {
        snprintf(versionBf, sizeof(versionBf), "%s\r\n\n", mercury_version);
        app_UsbWrite(versionBf, strlen(versionBf));
    }

    return 0;
}

// 获取APP应用版本
int app_at_get_appVersion(char *data, int len)
{
    char appVersion[64] = {0};
    snprintf(appVersion, sizeof(appVersion), "APP_VERSION:%s\r\nAPP_BUILD:%d\r\n\n", APP_VERSION, FW_BUILD_VERSION);
    app_UsbWrite(appVersion,strlen(appVersion));

    return 0;
}

// 获取产品型号
int app_at_get_dev_model(char *data, int len)
{
	AppInfoStruct app_info = {0};
    char product_model[32] = {0};
	
	NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info);
    
    snprintf(product_model, sizeof(product_model), "PRODUCT_MODEL:%s\r\n\n", app_info.model);
    app_UsbWrite(product_model,strlen(product_model));

    return 0;
}

// 获取项目编号
int app_at_get_item_num(char *data, int len)
{
	AppInfoStruct app_info = {0};
    char item_no[32] = {0};
	
	NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info);
    
    snprintf(item_no, sizeof(item_no), "PROJ_ID:%s\r\n\n", app_info.item_no);
    app_UsbWrite(item_no,strlen(item_no));

    return 0;
}
// 获取电池电量
int app_at_get_battery_level(char *data, int len)
{
    int battery_level = app_get_battery_percent();
    char battery[32] = {0};
    snprintf(battery, sizeof(battery), "BATTERY_LEVEL:%d\r\n\n", battery_level);
    app_UsbWrite(battery,strlen(battery));

    return 0;
}

void vendor_usb_recv_thread( ULONG argc, LPVOID lpThreadParameter );
void app_at_thread(void)
{
    vendor_usb_recv_thread(0, NULL);
}