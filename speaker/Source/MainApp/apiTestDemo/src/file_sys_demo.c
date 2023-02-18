#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryFile.h"
#include "MercuryNv.h"

#include "SDK_API_FS.h"

unsigned char l_testNvBuff[4096] = {0};

void print_nv_space(int nvLen) {
	uint32 DeviceFreeSpace, DeviceUsedSpace, DeviceTotalSpace;
    GetDeviceFreeSpace(&DeviceFreeSpace);
    GetDeviceUsedSpace(&DeviceUsedSpace);
    DeviceTotalSpace = DeviceFreeSpace + DeviceUsedSpace;
    MercuryTrace("Added NV item length = %d, DeviceFreeSpace = [%d], DeviceUsedSpace = [%d], DeviceTotalSpace = [%d]\r\n",
                nvLen, DeviceFreeSpace, DeviceUsedSpace, DeviceTotalSpace);
}

int write_nv_print_space(int nItemId, int nvLen) {
    int nRv = NV_Write(nItemId, nvLen, l_testNvBuff);
    MercuryTrace("NV_Write return value = [%d]\r\n", nRv);
    print_nv_space(nvLen);
    return nRv;
}

void test_nv_item_space(void) {
    int nRv;
    print_nv_space(0);
    for (int i = 1; i < sizeof(l_testNvBuff) / 16; i++) {
        nRv = write_nv_print_space(i, i*16);
        if (nRv != 0) {
            break;
        }
    }
}

void test_nv_clear_all(void) {
    for (size_t i = 0; i < 500; i++)
    {
        NV_Delete(i);
    }
    MercuryTrace("NV_Delete ok\r\n");
}

void test_nv_item_space_detail(void) {
    print_nv_space(0);

    write_nv_print_space(1, 1);

    write_nv_print_space(2, 395);
    write_nv_print_space(3, 396);
    write_nv_print_space(4, 397);

    write_nv_print_space(5, 491);
    write_nv_print_space(6, 492);
    write_nv_print_space(7, 493);

    write_nv_print_space(8, 991);
    write_nv_print_space(9, 992);
    write_nv_print_space(10, 993);

    write_nv_print_space(11, 1491);
    write_nv_print_space(12, 1492);
    write_nv_print_space(13, 1493);
}

void test_GetDeviceSpace(void)
{
    // test_nv_clear_all();
    // test_nv_item_space();

    test_nv_clear_all();

    test_nv_item_space_detail();

	// uint32 DeviceFreeSpace,DeviceUsedSpace,DeviceTotalSpace;
    // GetDeviceFreeSpace(&DeviceFreeSpace);
    // GetDeviceUsedSpace(&DeviceUsedSpace);
    // DeviceTotalSpace = DeviceFreeSpace + DeviceUsedSpace;
    // MercuryTrace("DeviceFreeSpace = [%d] DeviceUsedSpace = [%d] DeviceTotalSpace = [%d]\r\n",
    //             DeviceFreeSpace,DeviceUsedSpace,DeviceTotalSpace);
}

void test_GetDeviceSpace_Point(void)
{
    DEVICE_FS_USAGE deviceUsage;
    sdk_get_flash_usage(&deviceUsage,"E:");
    MercuryTrace("DeviceFreeSpace_Point = [%d] DeviceUsedSpace_Point = [%d] DeviceTotalSpace_Point = [%d]\r\n",
            deviceUsage.DeviceFreeSpace,deviceUsage.DeviceUsedSpace,deviceUsage.DeviceTotalSpace);
}