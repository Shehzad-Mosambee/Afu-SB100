#ifndef _CONNECT_MQTT_SOUNDBOX_SERVER_H_
#define _CONNECT_MQTT_SOUNDBOX_SERVER_H_
#include "MercuryDef.h"


#define MQTT_RECONNECT  -1

// #define MQTT_ADDRESS_PROD			"180.179.104.218"
// #define MQTT_PORT_PROD 				8883
#define MQTT_ADDRESS_PROD			"notifypro.in"
#define MQTT_PORT_PROD 				8883

#define MQTT_ADDRESS_UAT			"uat.notifypro.in"
#define MQTT_PORT_UAT  				8883

#define PRODUCT_KEY             "a1TrHYpY5LD"

#define DEV_CLINET_ID           10

// int msg_count;

void mqtt_soundbox_thread(ULONG argc, LPVOID lpThreadParameter);
void set_soundbox_network_success_event(void);

void mqtt_soundbox_close_thread();

#endif  //  _CONNECT_MQTT_SOUNDBOX_SERVER_H_