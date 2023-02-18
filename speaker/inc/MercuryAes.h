
#ifndef _MERCURY_AES_H_
#define _MERCURY_AES_H_

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AES_ENCRYPT     1 /**< AES encryption. */
#define AES_DECRYPT     0 /**< AES decryption. */

typedef void* AES_CTX;


int Aes_init(AES_CTX *ctx);

int Aes_free(AES_CTX ctx);

int Aes_setkey_enc(AES_CTX ctx, const unsigned char *key, unsigned int keybits);

int Aes_setkey_dec(AES_CTX ctx, const unsigned char *key, unsigned int keybits);

int Aes_crypt_ecb(AES_CTX ctx, int mode, const unsigned char input[16], unsigned char output[16]);

int Aes_crypt_cbc(AES_CTX ctx, int mode, size_t length, unsigned char iv[16], const unsigned char *input, unsigned char *output);

int Aes_crypt_cfb128(AES_CTX ctx, int mode, size_t length, size_t *iv_off, unsigned char iv[16], const unsigned char *input, unsigned char *output);

int Aes_crypt_cfb8(AES_CTX ctx, int mode, size_t length, unsigned char iv[16], const unsigned char *input, unsigned char *output );

int Aes_crypt_ctr(AES_CTX ctx, size_t length, size_t *nc_off, unsigned char nonce_counter[16], unsigned char stream_block[16], const unsigned char *input, unsigned char *output);

void Aes_encrypt(AES_CTX ctx, const unsigned char input[16], unsigned char output[16]);

void Aes_decrypt(AES_CTX ctx, const unsigned char input[16], unsigned char output[16]);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_MERCURY_AES_H_

