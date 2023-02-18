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
���õĴ�����
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
���ܣ�����һ��TLS SOCKET�����
*****************************/
TLSSOCK_HANDLE TlsSockCreate(void);


/********************************
���ܣ�����TLS socket��ز�����Ŀǰ��֧��ָ��SSL/TLS�汾�ź͵���CA֤�顣

����˵����
handle��TLS socket�����
type��ָ���������͡�

MC_CFG_TYPE_VERSION��������TLS�汾����Ӧparam�ĸ�16λ����֧�ֵ���߰汾�ţ���16λ����֧�ֵ���Ͱ汾�š��궨��TLSSOCK_VER_XXX�ǰ汾�Ŷ��塣
MC_CFG_TYPE_AUTHMODE����������֤ģʽ����Ӧparam��ֵ��TLSSOCK_AUTH_MODE_XXX��
MC_CFG_TYPE_CIPHER_SUITE�������üӽ����׼���param����Ϊ0xFFFF��ʾʹ�õײ�֧�ֵ����мӽ����׼���ʵ��ʹ�õ����ն˺ͷ�������֧�ֵ���ߵȼ��ļӽ����׼���
MC_CFG_TYPE_CA��������CA֤�飬��Ӧparam��������TLSSOCK_CRT_T��
MC_CFG_TYPE_CLI_CERT�������ÿͻ���֤�飬��Ӧparam��������TLSSOCK_CRT_T��
MC_CFG_TYPE_CLI_KEY�������ÿͻ�����Կ����Ӧparam��������TLSSOCK_CRT_T��
ע�⣺�����Ҫ֧��˫����֤ģʽ��APP�������ÿͻ���֤�����Կ��
����type�����ݲ�֧�֡�

param����typeƥ��Ĳ�����

����ֵ���ɹ�������0��ʧ�ܣ����� <0, -1�ǲ�����ʽ���ԣ���ָ��֮��Ĵ���
**********************************************/
int32 TlsSockCfg(TLSSOCK_HANDLE handle, MC_CRT_CFG_TYPE_E type, uint32 param);


/************************************
���ܣ���������������ӡ�

param[in]handle��TLS socket���
param[in]ip��������IP��Ŀǰ֧��IPv4��
param[in]port���������˿ںš�

����ֵ��
0���������ӷ��������������ӽ��ͨ��TLSSOCK_CONNECT_EVENT_IND��Ϣ֪ͨnotify_task��
-1��ʧ�ܡ�
*************************************/
int32 TlsSockConnect(TLSSOCK_HANDLE handle, IP_ADDR ip, uint16 port);

/**************************************************
���ܣ��������ݡ�

param[in] handle��TLS socket�����
param[in]buf��Ҫ���͵�����ָ�롣
param[in]len��Ҫ���͵����ݳ��ȣ�����len������1024������1024�����ݽ���ְ����͡�

return������0��ʾ�ɹ���С�ڵ���0��ʾʧ�ܡ�

********************************************************/
int32 TlsSockSend(TLSSOCK_HANDLE handle, uint8* buf, uint32 len);


/**************************************************

param[in] handle��TLS socket�����
param[out] buf�����ڴ�����ݵ�bufferָ�롣
param[in]len��buffer���ȡ�

����ֵ��С��0��ʾʧ�ܣ�����0��ʾʵ�ʶ�ȡ�������ݡ�

˵����
1��	�ú������յ�TLSSOCK_READ_EVENT_IND��Ϣ����á�
2��	��TLS socket�ڲ����ջ������ǿյģ���ʱ���յ����������ݣ�TLS socket�Żᷢ��TLSSOCK_READ_EVENT_IND��Ϣ��notify_task��������ջ������б��������ݣ���ʱ���յ��������ݣ����ᷢ��TLSSOCK_READ_EVENT_IND��Ϣ��
********************************************************/
int32 TlsSockRecv(TLSSOCK_HANDLE handle, uint8* buf, uint32 len);

/************************************************************
param[in] handle����TlsSockCreate������TLS socket��

**************************************************************/
int32 TlsSockClose(TLSSOCK_HANDLE handle);


#endif

