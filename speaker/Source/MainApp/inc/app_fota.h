#ifndef _APP_FOTA_H_
#define _APP_FOTA_H_

#define APP_VERSION                "1.0.0" 
#define FW_BUILD_VERSION            1

#define SERVER_HOST                 "152.136.213.55"
#define SERVER_PORT                 10501
#define CHECK_UPDATE                "/speaker/check-update"

extern volatile int ota_thread_finished;

typedef int (*http_callback)(unsigned char*);


#endif