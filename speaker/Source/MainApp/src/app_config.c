#include "MercuryPlat.h"
#include "MercuryDef.h"
#include "MercuryNv.h"
#include "MercuryCSTD.h"

#include "nv.h"

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
extern void getAppInfo(LPAppConfigStruct pAppInfo);

AppConfigStruct g_appInfo = {
    TRUE,           // bValid 表示NV保存的AppInfo是否有效 
    FALSE,          // bExistExFlash
    0,              // nCapacity  
    FALSE,          // bExistWifi
    5,              // nVolume
    1,              // nAudioType
    "1.0.0",        // sAppVersion
    1               // nBuildVersion
};

void getAppInfo(LPAppConfigStruct pAppInfo) {
    memcpy(pAppInfo, &g_appInfo, sizeof(AppConfigStruct));
}

// BOOL appInfoLoad(void) {
//     AppConfigStruct appCfg;
//     int nRV = NV_AppInfoRead(&appCfg, sizeof(AppConfigStruct));
//     MercuryTrace("appInfoLoad NV_AppInfoRead nRV = %d, appCfg.bValid = %d \n", nRV, appCfg.bValid);
//     if ((nRV != 0) || (appCfg.bValid)) {
//         return appInfoSave();
//     }
//     return (NV_AppInfoRead(&g_appInfo, sizeof(AppConfigStruct)) == 0);
// }

BOOL appInfoLoad(void) {
    AppConfigStruct appCfg;
    int nRV = NV_Read(NV_APP_INFO_ITEM, sizeof(AppConfigStruct), &appCfg);
    MercuryTrace("appInfoLoad NV_Read nRV = %d, appCfg.bValid = %d appCfg.nAudioType = %d \n", nRV, appCfg.bValid, appCfg.nAudioType);
    if ((nRV != 0) || !(appCfg.bValid)) {
        return appInfoSave();
    }
    return (NV_Read(NV_APP_INFO_ITEM, sizeof(AppConfigStruct), &g_appInfo) == 0);
}

// BOOL appInfoSave(void) {
//     return (NV_AppInfoWrite(&g_appInfo, sizeof(AppConfigStruct)) == 0);
// }

BOOL appInfoSave(void) {
    NV_Delete(NV_APP_INFO_ITEM);
    return (NV_Write(NV_APP_INFO_ITEM, sizeof(AppConfigStruct), &g_appInfo) == 0);
}

BOOL appInfoUpdate(LPAppConfigStruct pAppInfo) {
    memcpy(&g_appInfo, pAppInfo, sizeof(AppConfigStruct));
    return appInfoSave();
}

BOOL app_IsExistExFlash(void) {
    return g_appInfo.bExistExFlash;
}

int app_GetExFlashCapacity(void) {
    return g_appInfo.bExistExFlash ? g_appInfo.nCapacity : 0;
}

BOOL app_SetExFlash(BOOL isExist, int nCapacity) {
    g_appInfo.bExistExFlash = isExist;
    g_appInfo.nCapacity = nCapacity;
    return appInfoSave();
}

BOOL app_IsExistWifi(void) {
    return g_appInfo.bExistWifi;
}

BOOL app_SetExistWifi(BOOL isExist) {
    g_appInfo.bExistWifi = isExist;
    return appInfoSave();
}

int app_GetVolume(void) {
    return g_appInfo.nVolume;
}

// BOOL app_SetVolume(int nVolume) {
//     g_appInfo.nVolume = nVolume;
//     return appInfoSave();
// }

void app_SetVolume(int nVolume) {
    g_appInfo.nVolume = nVolume;
}

int app_GetAudioType(void) {
    return g_appInfo.nAudioType;
}

BOOL app_SetAudioType(int nAudioType) {
    g_appInfo.nAudioType = nAudioType;
    return appInfoSave();
}

char *app_GetAppVersion(void)
{
    return g_appInfo.sAppVersion;
}

BOOL app_SetAppVersion(char *sAppVersion) {
    strcpy(g_appInfo.sAppVersion,sAppVersion);
    return appInfoSave();
}

int app_GetBuildVersion(void)
{
    return g_appInfo.nBuildVersion;
}

BOOL app_SetBuildVersion(int nBuildVersion) {
    g_appInfo.nBuildVersion = nBuildVersion;
    return appInfoSave();
}