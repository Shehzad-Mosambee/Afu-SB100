#pragma once
#include "ICommChannel.h"

typedef enum
{
	//system cmd
	MC_DOWNLOAD_APP = 0,
	MC_RUN_APP,
	MC_AUTO_RUN_APP,
	MC_READ_IMEI,
	MC_WRITE_IMEI,
	MC_READ_VER,
	MC_READ_CAL,
	MC_THROUGH_MODE,
	MC_MODEL_CONFIG,
	MC_WRITE_APPINFO,
	MC_READ_APPINFO,
	MC_SET_LOGO,
	MC_DEL_LOGO,
	MC_WRITE_CHECKCODE,
	MC_READ_CHECKCODE,
	MC_READ_MERCURYID,
	MC_READ_COMPANY,
	MC_CHECK_APP,
	MC_BACKLIGHT_CONTROL,
	MC_DIAGPORT_CONTROL,
	MC_GET_AUTO_RUN_STATUS,
	MC_PACKEG_SIZE_SET,
	MC_READ_SIM_INFO,
	MC_READ_NV,
	MC_WRITE_NV,
	MC_RESET_SYSTEM,
	MC_POWER_DOWN,
	MC_CHANNEL_SWITCH,

	MC_WRITE_PUB_KEY,
	MC_READ_PUB_KEY,
	MC_WRITE_SIGN_DATA,
	MC_SEC_ENABLE,
	MC_SET_OTP,
	MC_GET_SEC_STATE,

	//fileSystem cmd
	MC_FILESYSTE_INIT,
	MC_FILESYSTE_INIT_PLUS,
	MC_FILESYSTE_FORMAT,
	MC_FILESYSTE_PUSH_FILE,
	MC_FILESYSTE_PULL_FILE,
	MC_FILESYSTE_DIR_PATH,
	MC_FILESYSTE_CREATE_DIR,
	MC_FILESYSTE_DELETE_DIR,
	MC_FILESYSTE_DELETE_FILE,
	MC_FILESYSTE_CONFIG,
	//newly added	
	MC_LOGOFLASH_DATAWRITE,
	MC_CMD_FOTA_INFO,
	MC_CMD_DEL_FOTA_INFO,
	MC_CMD_CLEAR_CALIFLAG,
	MC_CMD_DOWNLOADAPP_CHECKSET,
	MC_DOWNLOAD_APP_EXT,
	MC_LOGOFLASH_DATAREAD,
	MC_RUNTIMENV_ERASE,
	MC_READ_MODELNAME,
	MC_CMD_INVAILD
}MC_FASTBOOT_CMD_E;



typedef struct
{
	UINT32						magicNumber;
	UINT8						signData[256];
	UINT32						length;
	MC_FASTBOOT_CMD_E			cmd;
}MC_FASTBOOT_CMD_HEADER;

typedef struct
{
	UINT32						result;
	UINT32						len;
}MC_FASTBOOT_RSP_HEADER;

typedef struct
{
	UINT32						addr;
	UINT32						len;
}MC_DOWNLOAD_IMG_BLOCK;

typedef struct
{
	UINT8						info[256];
	UINT32						len;
}MC_PUSH_IMG_INFO;

typedef struct
{
	UINT32						itemID;
	UINT32						len;
}MC_NV_EDITE_INFO;
void fb_queue_writeSign(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, char *type, unsigned int size);
void fb_queue_download(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size);
void fb_queue_push(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *path, const char *data, unsigned int size);
void fb_queue_command(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size);
void fb_queue_fileSys(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size);
int fb_execute_queue(ICommChannel* m_pProChan);
void fb_queue_pull(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *path, const char *data, unsigned int size);
void fb_queue_nvEdite(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *info, const char *data, unsigned int size);
void fb_queue_img(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size);
void fb_queue_readLogoPart(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size, const char *path);