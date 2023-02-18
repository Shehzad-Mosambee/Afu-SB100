
#ifndef _MERCURY_RSA_H_
#define _MERCURY_RSA_H_


#include "MercuryDef.h"

/*
 * RSA constants
 */
#define RSA_MODE_PUBLIC     0
#define RSA_MODE_PRIVATE    1

#define RSA_PKCS_V15        0
#define RSA_PKCS_V21        1

#define KEY_LEN 128
#define PT_LEN  24
#define RSA_PT  "\xAA\xBB\xCC\x03\x02\x01\x00\xFF\xFF\xFF\xFF\xFF" \
                "\x11\x22\x33\x0A\x0B\x0C\xCC\xDD\xDD\xDD\xDD\xDD"



typedef enum {
    RSA_HASH_NONE = 0,
    RSA_HASH_MD2,
    RSA_HASH_MD4,
    RSA_HASH_MD5,
    RSA_HASH_SHA1,
    RSA_HASH_SHA224,
    RSA_HASH_SHA256,
    RSA_HASH_SHA384,
    RSA_HASH_SHA512,
    RSA_HASH_RIPEMD160,
} RSA_HASH_TYPE;


typedef enum{
    RSA_KEY_TYPE_PUB = 0,
    RSA_KEY_TYPE_PRI,
    RSA_KEY_MAX,
}RSA_KEY_TYPE_E;


typedef struct{
    RSA_KEY_TYPE_E type;
    uint16 len;
    uint8 *pRsa_N;
    uint8 *pRsa_E;
    uint8 *pRsa_D;
    uint8 *pRsa_P;
    uint8 *pRsa_Q;
    uint8 *pRsa_DP;
    uint8 *pRsa_DQ;
    uint8 *pRsa_QP;
}RSA_KEY_T;


typedef void* RSA_CTX;
typedef void* RNG_CTX;

/***********************************************
RSA 使用初始化
param[in] padding 类型RSA_PKCS_V15，RSA_PKCS_V21
param[in] hash_id 初始化值0
return: NULL fail, RSA_CTX pointer
*******************************************/
int mercury_rsa_init( RSA_CTX *CTX , int padding, RSA_HASH_TYPE hash_id );
int mercury_rsa_gen_key( RSA_CTX ctx, int nbits, int exponent );
int mercury_rsa_check_pubkey( RSA_CTX ctx );
int mercury_rsa_check_privkey( RSA_CTX ctx );
int mercury_rsa_public( RSA_CTX ctx,unsigned char *input,unsigned char *output);
int mercury_rsa_private( RSA_CTX ctx,unsigned char *input,unsigned char *output);
int mercury_rsa_pkcs1_encrypt( RSA_CTX ctx,int mode, int ilen, uint8 *input, uint8 *output);
int mercury_rsa_pkcs1_decrypt( RSA_CTX ctx,int mode, int *olen, uint8 *input,uint8 *output, uint8 outputsize);
int mercury_rsa_pkcs1_sign(RSA_CTX ctx, int mode, RSA_HASH_TYPE hash_id, int hashlen, const uint8 *hash, uint8 *sig);
int mercury_rsa_pkcs1_verify( RSA_CTX ctx,int mode, RSA_HASH_TYPE hash_id,int hashlen, uint8 *hash, uint8 *sig );
void mercury_rsa_free( RSA_CTX ctx );

/***********************************************
 * 使用该函数   不需要调用 mercury_rsa_init， mercury_rsa_check_pubkey， mercury_rsa_check_privkey 函数内部已经实现
 * 用户只需传入 Rsakey_t， 解析后的各个参数值，即可使用,不使用了需要调用 mercury_rsa_free
 * return    0 成功   -1  失败
*******************************************/
int mercury_rsa_creat(RSA_CTX *ctx , int padding, RSA_HASH_TYPE hash_id, RSA_KEY_T Rsakey_t);

/***********************************************
随机数相关函数
*******************************************/
int mercury_rng_init(RNG_CTX *CTX);
int mercury_rng_nonce_Init(RNG_CTX CTX , unsigned char *nonce, size_t nonceSz);
int mercury_rng_gen(RNG_CTX CTX , unsigned char *output, size_t sz);


/***********************************************
 * mercury_rsa_pkcs1_encrypt_ex
 * 改接口填充数据不做校验，增加，客户自行处理
************************************************/
int mercury_rsa_pkcs1_encrypt_ex( RSA_CTX ctx,int mode, int ilen, uint8 *input, uint8 *output);
/***********************************************
 * mercury_rsa_pkcs1_decrypt_ex
 * 改接口填充数据不做校验，增加，客户自行处理
************************************************/
int mercury_rsa_pkcs1_decrypt_ex( RSA_CTX ctx,int mode, int *olen, uint8 *input, uint8 *output, uint8 output_size);


#endif

