#ifndef _APP_AT_H_
#define _APP_AT_H_

// 获取设备SN
int app_at_get_SN(char *data, int len);
// 获取设备SIM卡IMSI、ICCID
int app_at_get_simInfo(char *data, int len);
// 获取模块IMEI
int app_at_get_imei(char *data, int len);
// 获取底包版本
int app_at_get_bottomBag_version(char *data, int len);
// 获取APP应用版本
int app_at_get_appVersion(char *data, int len);
// 获取产品型号
int app_at_get_dev_model(char *data, int len);
// 获取项目编号
int app_at_get_item_num(char *data, int len);
//获取电池电量
int app_at_get_battery_level(char *data, int len);

// AT thread
void app_at_thread(void);

#endif