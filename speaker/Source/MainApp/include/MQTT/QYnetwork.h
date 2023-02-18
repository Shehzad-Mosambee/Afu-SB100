#ifndef _QYNETWORK_H
#define _QYNETWORK_H

#include "MercuryDef.h"

typedef struct
{
	BOOL DNS_flag;
	TCPIP_HOST_HANDLE DNS_value;
}STRUCT_DNS_RESULT;

extern HANDLE connect_event_handle;

extern HANDLE dns_event_handle;
extern STRUCT_DNS_RESULT DNS_result;

int network_init( void );
int connect( int sockfd, const char *ipaddress, int port );
int recv( int sockfd, void *buf, int count,int *readedlen, int timeout );
int send( int sockfd, const void *buf, int count, int *writedlen, int timeout );
int close( int sockfd );
int hosttoip( const char *hostname, char *ipaddress );

#endif
