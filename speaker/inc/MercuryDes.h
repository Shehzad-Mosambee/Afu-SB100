
#ifndef _MERCURY_DES_H_
#define _MERCURY_DES_H_

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DES_ENCRYPT     1 /**< DES encryption. */
#define DES_DECRYPT     0 /**< DES decryption. */

#define    DES_KEY_SIZE   8

typedef void* DES_CTX;
typedef void* DES3_CTX;

int Des_init(DES_CTX *ctx);

int Des_free(DES_CTX ctx);

int Des3_init(DES3_CTX *ctx);

int Des3_free(DES3_CTX ctx);

void Des_key_set_parity(unsigned char key[DES_KEY_SIZE]);

int Des_key_check_key_parity(const unsigned char key[DES_KEY_SIZE]);

int Des_key_check_weak( const unsigned char key[DES_KEY_SIZE]);

int Des_setkey_enc(DES_CTX ctx, const unsigned char key[DES_KEY_SIZE]);

int Des_setkey_dec(DES_CTX ctx, const unsigned char key[DES_KEY_SIZE]);

int Des3_set2key_enc(DES3_CTX ctx, const unsigned char key[DES_KEY_SIZE * 2]);

int Des3_set2key_dec(DES3_CTX ctx, const unsigned char key[DES_KEY_SIZE * 2]);

int Des3_set3key_enc(DES3_CTX ctx, const unsigned char key[DES_KEY_SIZE * 3]);

int Des3_set3key_dec(DES3_CTX ctx, const unsigned char key[DES_KEY_SIZE * 3]);

int Des_crypt_ecb(DES_CTX ctx, const unsigned char input[8], unsigned char output[8]);

int Des_crypt_cbc(DES_CTX ctx, int mode, size_t length, unsigned char iv[8], const unsigned char *input, unsigned char *output);

int Des3_crypt_ecb(DES3_CTX ctx, const unsigned char input[8], unsigned char output[8]);

int Des3_crypt_cbc(DES3_CTX ctx, int mode, size_t length, unsigned char iv[8], const unsigned char *input, unsigned char *output);

void Des_decrypt(unsigned int SK[32], const unsigned char key[DES_KEY_SIZE]);

int Des_crypt_ecb_data(DES_CTX ctx, size_t length, unsigned char *input, unsigned char *output);

int Des3_crypt_ecb_data(DES3_CTX ctx, size_t length, unsigned char *input, unsigned char *output);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_MERCURY_DES_H_

