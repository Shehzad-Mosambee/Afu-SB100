#ifndef _MERCURY_TLS_H_
#define _MERCURY_TLS_H_

/* SSL Version */
#define TLSSOCK_VER_SSL30           0
#define TLSSOCK_VER_TLS10           1
#define TLSSOCK_VER_TLS11           2
#define TLSSOCK_VER_TLS12           3
#define TLSSOCK_VER_ALL             4

#define MBEDTLS_TLS_RSA_WITH_NULL_SHA                    0x02   /**< Weak! */

#define MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA             0x2F
#define MBEDTLS_TLS_RSA_WITH_RC4_128_MD5                 0x04
#define MBEDTLS_TLS_RSA_WITH_RC4_128_SHA                 0x05
#define MBEDTLS_TLS_RSA_WITH_DES_CBC_SHA                 0x09   /**< Weak! Not in TLS 1.2 */

#define TLSSOCK_AUTH_MODE_NONE 0
#define TLSSOCK_AUTH_MODE_SERVER_OPTIONAL 1
#define TLSSOCK_AUTH_MODE_SERVER_REQUIRED 2

/*********************************************
配置的错误码
***********************************************/
#define TLSSOCK_ERR_X509_FEATURE_UNAVAILABLE              -0x2080  /**< Unavailable feature, e.g. RSA hashing/encryption combination. */
#define TLSSOCK_ERR_X509_UNKNOWN_OID                      -0x2100  /**< Requested OID is unknown. */
#define TLSSOCK_ERR_X509_INVALID_FORMAT                   -0x2180  /**< The CRT/CRL/CSR format is invalid, e.g. different type expected. */
#define TLSSOCK_ERR_X509_INVALID_VERSION                  -0x2200  /**< The CRT/CRL/CSR version element is invalid. */
#define TLSSOCK_ERR_X509_INVALID_SERIAL                   -0x2280  /**< The serial tag or value is invalid. */
#define TLSSOCK_ERR_X509_INVALID_ALG                      -0x2300  /**< The algorithm tag or value is invalid. */
#define TLSSOCK_ERR_X509_INVALID_NAME                     -0x2380  /**< The name tag or value is invalid. */
#define TLSSOCK_ERR_X509_INVALID_DATE                     -0x2400  /**< The date tag or value is invalid. */
#define TLSSOCK_ERR_X509_INVALID_SIGNATURE                -0x2480  /**< The signature tag or value invalid. */
#define TLSSOCK_ERR_X509_INVALID_EXTENSIONS               -0x2500  /**< The extension tag or value is invalid. */
#define TLSSOCK_ERR_X509_UNKNOWN_VERSION                  -0x2580  /**< CRT/CRL/CSR has an unsupported version number. */
#define TLSSOCK_ERR_X509_UNKNOWN_SIG_ALG                  -0x2600  /**< Signature algorithm (oid) is unsupported. */
#define TLSSOCK_ERR_X509_SIG_MISMATCH                     -0x2680  /**< Signature algorithms do not match. (see \c ::mbedtls_x509_crt sig_oid) */
#define TLSSOCK_ERR_X509_CERT_VERIFY_FAILED               -0x2700  /**< Certificate verification failed, e.g. CRL, CA or signature check failed. */
#define TLSSOCK_ERR_X509_CERT_UNKNOWN_FORMAT              -0x2780  /**< Format not recognized as DER or PEM. */
#define TLSSOCK_ERR_X509_BAD_INPUT_DATA                   -0x2800  /**< Input invalid. */
#define TLSSOCK_ERR_X509_ALLOC_FAILED                     -0x2880  /**< Allocation of memory failed. */
#define TLSSOCK_ERR_X509_FILE_IO_ERROR                    -0x2900  /**< Read/write of file failed. */
#define TLSSOCK_ERR_X509_BUFFER_TOO_SMALL                 -0x2980  /**< Destination buffer is too small. */



typedef enum
{
    MC_CFG_TYPE_VERSION,
    MC_CFG_TYPE_AUTHMODE,
    MC_CFG_TYPE_CIPHER_SUITE,
    MC_CFG_TYPE_CA,
    MC_CFG_TYPE_CLI_CERT,
    MC_CFG_TYPE_CLI_KEY,
    MC_CFG_TYPE_IGNORE_RTC_TIME,
    MC_CFG_TYPE_HTTPS,
    MC_CFG_TYPE_HTTPS_CTXI,
    MC_CFG_TYPE_SMTPS_TYPE,
    MC_CFG_TYPE_SMTPS_CTXI,
} MC_CRT_CFG_TYPE_E;

#define DATA_OUTPUT_RINGBUF_SIZE 1024

#define IP_ADDR uint32
#define TLSSOCK_HANDLE uint32
#define int32 signed int


typedef struct _TLSSOCK_CRT_T
{
    char *crt;
    int32 len;
} TLSSOCK_CRT_T;


/**********************************
功能：创建一个TLS SOCKET句柄。
*****************************/
TLSSOCK_HANDLE TlsSockCreate(void);


/********************************
功能：配置TLS socket相关参数，目前仅支持指定SSL/TLS版本号和导入CA证书。

参数说明：
handle：TLS socket句柄。
type：指明配置类型。

MC_CFG_TYPE_VERSION用于配置TLS版本，对应param的高16位代表支持的最高版本号，低16位代表支持的最低版本号。宏定义TLSSOCK_VER_XXX是版本号定义。
MC_CFG_TYPE_AUTHMODE用于配置认证模式，对应param的值是TLSSOCK_AUTH_MODE_XXX。
MC_CFG_TYPE_CIPHER_SUITE用于配置加解密套件，param设置为0xFFFF表示使用底层支持的所有加解密套件。实际使用的是终端和服务器都支持的最高等级的加解密套件。
MC_CFG_TYPE_CA用于配置CA证书，对应param的类型是TLSSOCK_CRT_T。
MC_CFG_TYPE_CLI_CERT用于配置客户端证书，对应param的类型是TLSSOCK_CRT_T。
MC_CFG_TYPE_CLI_KEY用于配置客户端密钥，对应param的类型是TLSSOCK_CRT_T。
注意：如果需要支持双向认证模式，APP必须配置客户端证书和密钥。
其它type类型暂不支持。

param：与type匹配的参数。

返回值：成功，返回0；失败，返回 <0, -1是参数格式不对，空指针之类的错误。
**********************************************/
int32 TlsSockCfg(TLSSOCK_HANDLE handle, MC_CRT_CFG_TYPE_E type, uint32 param);


/************************************
功能：尝试与服务器连接。

param[in]handle：TLS socket句柄
param[in]ip：服务器IP，目前支持IPv4。
param[in]port：服务器端口号。

返回值：
0：正在连接服务器。具体连接结果通过TLSSOCK_CONNECT_EVENT_IND消息通知notify_task。
-1：失败。
*************************************/
int32 TlsSockConnect(TLSSOCK_HANDLE handle, IP_ADDR ip, uint16 port);

/**************************************************
功能：发送数据。

param[in] handle：TLS socket句柄。
param[in]buf：要发送的数据指针。
param[in]len：要发送的数据长度，建议len不大于1024；多于1024的数据建议分包发送。

return：大于0表示成功；小于等于0表示失败。

********************************************************/
int32 TlsSockSend(TLSSOCK_HANDLE handle, uint8* buf, uint32 len);


/**************************************************

param[in] handle：TLS socket句柄。
param[out] buf：用于存放数据的buffer指针。
param[in]len：buffer长度。

返回值：小于0表示失败；大于0表示实际读取到的数据。

说明：
1）	该函数在收到TLSSOCK_READ_EVENT_IND消息后调用。
2）	当TLS socket内部接收缓冲区是空的，这时接收到网络新数据，TLS socket才会发送TLSSOCK_READ_EVENT_IND消息给notify_task；如果接收缓存区中本身还有数据，此时接收到网络数据，不会发送TLSSOCK_READ_EVENT_IND消息。
********************************************************/
int32 TlsSockRecv(TLSSOCK_HANDLE handle, uint8* buf, uint32 len);

/************************************************************
param[in] handle：由TlsSockCreate创建的TLS socket。

**************************************************************/
int32 TlsSockClose(TLSSOCK_HANDLE handle);


#endif

