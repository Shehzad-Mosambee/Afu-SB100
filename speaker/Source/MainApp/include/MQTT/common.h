
#ifndef _COMMON_H_
#define _COMMON_H_
#define MQTT_DEBUG 1
//#define  MQTT_TASK 1
#include "MercuryDef.h"
#include "MercuryAudio.h"
#include "MercuryBarScan.h"
#include "MercuryGraphics.h"
#include "MercuryMem.h"
#include "MercuryNet.h"
#include "MercuryNv.h"
#include "MercuryPowerManager.h"
#include "MercurySyn.h"
#include "MercuryThread.h"
#include "MercuryTimer.h"
//#include "MercuryTts.h"
#include "MercuryKeypad.h"
#include "MercuryBattery.h"
#include "MercuryNLed.h"
#include "MercuryFile.h"
#include "MercuryPlat.h"
#include "MercuryCom.h"
#include "MercurySpi.h"
#include "MercuryGpio.h"
#include "MercuryMobile.h"
#include "MercuryVKey.h"
#include "MercuryFota.h"
#include "MercuryPWM.h"
#include "MercuryI2C.h"
#include "MercuryCSTD.h"


//mqtt
#include <MQTTConnect.h>
#include <MQTTPacket.h>
#include <MQTTFormat.h>
#include <MQTTPublish.h>
#include <MQTTSubscribe.h>
#include <MQTTUnSubscribe.h>
#include <StackTrace.h>
//#include <transport.h>
//#include <tcpnetwork.h>
//#include <network.h>

//http
//#include "http.h"

#include "MQTTfreeRTOS.h"
#include "MQTTClient.h"
#include "QYnetwork.h"
#define uart_printf MercuryTrace

#endif
