#ifndef _APP_COFIG_H_
#define _APP_COFIG_H_

#include "MercuryDef.h"

typedef struct _AppConfigStruct
{
    BOOL        bValid;
    BOOL        bExistExFlash;
    int         nCapacity;
    BOOL        bExistWifi;
    int         nVolume;
    int         nAudioType;
    // char        sDevSN[32];
    char        sAppVersion[32];
    int         nBuildVersion;
    /* data */
} AppConfigStruct, *LPAppConfigStruct;

BOOL appInfoLoad(void);
void getAppInfo(LPAppConfigStruct pAppInfo);
BOOL appInfoSave(void);
BOOL appInfoUpdate(LPAppConfigStruct pAppInfo);

// flash
BOOL app_IsExistExFlash(void);
int app_GetExFlashCapacity(void);
BOOL app_SetExFlash(BOOL isExist, int nCapacity);

// wifi
BOOL app_IsExistWifi(void);
BOOL app_SetExistWifi(BOOL isExist);

// audio
int app_GetVolume(void);
void app_SetVolume(int nVolume);
int app_GetAudioType(void);
BOOL app_SetAudioType(int nAudioType);

// device version
char *app_GetAppVersion(void);
BOOL app_SetAppVersion(char *sAppVersion);
int app_GetBuildVersion(void);
BOOL app_SetBuildVersion(int nBuildVersion);



#endif