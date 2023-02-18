#ifndef _MERCURY_SHA_H_
#define _MERCURY_SHA_H_

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint32 total[2];          /*!< The number of Bytes processed.  */
    uint32 state[8];          /*!< The intermediate digest state.  */
    unsigned char buffer[64];   /*!< The data block being processed. */
    int is224;                  /*!< Determines which function to use.*/
}
SHA256_CTX;

typedef struct
{
    uint32 state[5];
    uint32 count[2];
    unsigned char buffer[64];
} SHA1_CTX;


int Sha1_Init( SHA1_CTX *ctx );
int Sha1_Free( SHA1_CTX *ctx );
int Sha1_Starts( SHA1_CTX *ctx );
int Sha1_Update( SHA1_CTX *ctx, const unsigned char *input, size_t ilen );
int Sha1_Finish( SHA1_CTX *ctx, unsigned char output[20] );
int Sha1_Clone( SHA1_CTX *dst, const SHA1_CTX *src );
int Sha1(const uint8 *input, size_t ilen, uint8 output[20]);


int Sha256_Init( SHA256_CTX *ctx );
int Sha256_Free( SHA256_CTX *ctx );
int Sha256_Starts( SHA256_CTX *ctx , int is224 );
int Sha256_Update( SHA256_CTX *ctx, const unsigned char *input, size_t ilen );
int Sha256_Finish( SHA256_CTX *ctx, unsigned char output[20] );
int Sha256_Clone( SHA256_CTX *dst, const SHA256_CTX *src );
int Sha256(const uint8 *input, size_t ilen, uint8 output[20], int is224);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
