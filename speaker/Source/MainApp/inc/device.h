#ifndef _DEVICE_H_
#define _DEVICE_H_

#define E_RST_UNKNOWN 0    //!< Reset reason can not be determined
#define E_RST_POWERON 1       //!< Reset due to power-on event
#define E_RST_EXT     2       //!< Reset by external pin (not applicable for ESP8266)
#define E_RST_SW      3      //!< Software reset via esp_restart
#define E_RST_APPLY	4
#define E_RST_CONFIG	5
#define E_RST_REBOOT	6
#define E_RST_CHKUSB	7

#define AL36V200 "AL36V200"
#define AL36V203 "AL36V203"
#define AL35V300 "AL35V300"
#define AL35V303 "AL35V303"
#define MERCURY_VERSION_AL36V200    0
#define MERCURY_VERSION_AL36V203    1
#define MERCURY_VERSION_AL35V300    2
#define MERCURY_VERSION_AL35V303    3
#define MERCURY_VERSION_UNKNOWN    -1

extern int g_mercury_version;
void set_restart_reason(int reset_reason);
void app_keypad_init(void);
void check_usb(void);

#endif

