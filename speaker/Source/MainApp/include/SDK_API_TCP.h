/*************************************************************************
    > Project: SDK
    > File Name: SDK_API_TCP.h
	> Author: 
	> Mail: 
	> Created Time: 2019/03/22
 ************************************************************************/

#ifndef _QYY_API_TCP_H
#define _QYY_API_TCP_H

//SDK_API_ERROR.h
int sdk_tcp_connect( const char *ipaddress, int port, int tls, unsigned long *handle );
int sdk_tcp_recv( unsigned long handle, unsigned char *buffer, int toreadlen, int *readedlen, int timeout );
int sdk_tcp_send( unsigned long handle, unsigned char *buffer, int towritelen, int *writelen, int timeout );
int sdk_tcp_close( unsigned long handle );
int sdk_hosttoip( const char *hostname, char *ipaddress );

int sdk_setx509cer(const char* base64cer, int datalen);
int sdk_gprs_set_dns_server( const char *dns1, const char *dns2 );

//extern int sdk_get_wifi_gateway( void *arg );
extern int sdk_get_wifi_rssi(int *rssi);
extern int sdk_get_wifi_hostname(char *hostname);
extern int sdk_set_wifi_hostname(char *hostname);

#endif
