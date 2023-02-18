#ifndef _MQTTFREERTOS_H_
#define _MQTTFREERTOS_H_

#include "common.h"

typedef struct Mutex
{
	CRITICAL_SECTION * sem;
} Mutex;

typedef struct Thread
{
	DWORD task;
} Thread;

typedef struct Timer 
{
	ULONG xTicksToWait;
	ULONG enterTicks;
}Timer;

typedef struct Network
{
	//int my_socket;
	TCPIP_SOCKET_T my_socket;
	int (*mqttread) (struct Network*, unsigned char*, int, ULONG);
	int (*mqttwrite) (struct Network*, unsigned char*, int, ULONG);
	void (*disconnect) (struct Network*);
	int tls_flag;
}Network;

int ThreadStart(Thread* thread, void (*fn)(ULONG argc,LPVOID lpThreadParameter), void* arg);

void MutexInit(Mutex *mutex);

int MutexLock(Mutex *mutex);

int MutexUnlock(Mutex *mutex);

void TimerInit(Timer* timer);

void TimerCountdownMS(Timer* timer, ULONG timeout_ms);

void TimerCountdown(Timer* timer, ULONG timeout);

int TimerLeftMS(Timer* timer);

char TimerIsExpired(Timer* timer);

void NetworkInit(Network* n);

int NetworkConnect(Network* n, char* addr, int port);

void amoi_disconnect(Network* n);

int amoi_read(Network* n, unsigned char* buffer, int len, ULONG timeout_ms);

int amoi_write(Network* n, unsigned char* buffer, int len, ULONG timeout_ms);


#endif
