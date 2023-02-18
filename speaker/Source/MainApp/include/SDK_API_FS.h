#ifndef _SDK_API_FS_H_
#define _SDK_API_FS_H_

#include "MercuryDef.h"

typedef struct
{
    uint32 DeviceFreeSpace;
    uint32 DeviceUsedSpace;
    uint32 DeviceTotalSpace;
} DEVICE_FS_USAGE;

/**
 * @brief Obtain flash usage
 * 
 * @param deviceSpace [out] The device stores usage information
 * @param driverName [in] Device Driver Name
 */
void sdk_get_flash_usage(DEVICE_FS_USAGE *deviceSpace,char *driverName);

#endif