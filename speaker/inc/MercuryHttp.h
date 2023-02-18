#ifndef _MERCURY_HTTP_H
#define _MERCURY_HTTP_H
#include "MercuryDef.h"


#define MC_HTTPS_CA_CRT 0
#define MC_HTTPS_CLIENT_CRT 1
#define MC_HTTPS_CLIENT_PRIVATE_KEY 2

typedef void* HTTP_CONTEX;


/*
Http 初始化
return : NULL faile, HTTP_CONTEX success
*/
HTTP_CONTEX HttpInit(void);

/*
Http 去初始化
param[in] http_handle
return : MC_FAILE faile, MC_SUCCESS success
*/
int HttpDeinit(HTTP_CONTEX http_handle);

/*
Http 参数配置
param[in] http_handle , MC_HttpInit param out
param[in] ParamItem: parameter list "CID", "URL","UA","REDIR","BREAK","BREAKEND","TIMEOUT","CONTENT"
          "USERNAME", "PASSWORD"
param[in] ParamValue: string , http parameter value, 
return : MC_FAILE faile, MC_SUCCESS success
*/
int HttpConfig(HTTP_CONTEX http_handle, char * ParamItem, char * ParamValue);


/*
Http 设置证书
param[in] http_handle
param[in] CertType,证书类型，0 set CA certificate, 1 client certificate, 2 private key
param[in] CertData 证书内容
param[in] datasize: 数据长度
return : MC_FAILE faile, MC_SUCCESS success
*/
int HttpSetCrt(HTTP_CONTEX http_handle, uint8 CertType, char * CertData, uint16 datasize);

/*
Input http data
param[in] http_handle
param[in] HttpData http data
param[in] datasize: 数据长度
return : MC_FAILE faile, MC_SUCCESS success
*/
int HttpInputData(HTTP_CONTEX http_handle, char * HttpData, uint16 datasize);


/*
read http data
param[in] http_handle
param[in] startaddress: start read address
param[in] readsize: want to read data size
return : MC_FAILE faile, MC_SUCCESS success, 实际读取的数据通过callback 发送给APP
*/
int HttpRead(HTTP_CONTEX http_handle, uint16 startaddress, uint16 readsize);


/*
http action
param[in] http_handle
param[in] actionmode: 0 GET, 1 POST, 2 HEAD, 3 DELETE
return : MC_FAILE faile, MC_SUCCESS success
*/
int HttpAction(HTTP_CONTEX http_handle, uint8 actionmode);


/*
Input http data， 功能同MC_HttpInputData, 但是参数的是startaddress， readsize的类型不同
MC_HttpInputData， 参数是uint16 ,导致发送大于64K的数据出错
param[in] http_handle
param[in] HttpData http data
param[in] datasize: 数据长度
return : MC_FAILE faile, MC_SUCCESS success
*/

int HttpInputData_Ex(HTTP_CONTEX http_handle, char * HttpData, uint32 datasize);


/*
read http data, 功能同MC_HttpRead, 但是参数的是startaddress， readsize的类型不同
MC_HttpRead， 参数是uint16 ,导致无法读取大于64K的数据
param[in] http_handle
param[in] startaddress: start read address
param[in] readsize: want to read data size
return : MC_FAILE faile, MC_SUCCESS success, 实际读取的数据通过callback 发送给APP
*/
int HttpRead_Ex(HTTP_CONTEX http_handle, uint32 startaddress, uint32 readsize);


#endif
