#ifndef _QYY_API_MQTT_H_
#define _QYY_API_MQTT_H_

#include "common.h"

#define MQTT_MAX_NUMBER 4
#define READ_BUF_LEN 1024
#define SEND_BUF_LEN (3 * 1024)

typedef void (*MQTTClient_connectionLost) (void* context, const char *cause);
typedef void (*MSG_ARRIVED)(MessageData *data);

typedef struct MQTTClient_HANDLE{
	MQTTClient mqtt_client;
	Network mqtt_network;
    MQTTClient_connectionLost Callback;
}MQTTClient_HANDLE;


MQTTClient *handle_to_client(TCPIP_SOCKET_T handle);

int sdk_MQTT_connect(const char* ipaddress, int port , int tls, int timeout, 
                 const char* clientId, const char* usrname, const char* password, 
                 int keep_alive_interval, MQTTClient_connectionLost lostCallback, 
                 unsigned long* handle, BOOL will, unsigned char* will_topic, 
                 unsigned char *will_message, int ping_timeout, int cleansession );

int sdk_MQTT_publish( unsigned long handle, const char *topic, unsigned long qos, void *payload, int payloadlen );

int sdk_MQTT_subcribe(unsigned long handle, const char* topic, unsigned long qos,
                  MSG_ARRIVED messageCallback);

int sdk_MQTT_unsubcribe(unsigned long handle, const char* topic);

int sdk_MQTT_close(unsigned long handle);

int sdk_MQTT_yield(unsigned long handle, int seconds);

int sdk_change_mqtt_alive_interval(unsigned long handle, unsigned int AliveInterval);

#endif
