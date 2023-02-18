#ifndef _WIFI_H_
#define _WIFI_H_

#define WIFI_AP         0
#define WIFI_AIRKISS    1

typedef struct 
{
	char ssid[64];
	char pwd[32];
}AppWifiInfoStruct;

void ap_and_airkiss_wifi_connect(int wifiType);
void start_connect_network(void);

#endif