#ifndef _MERCURY_MD5_H_
#define _MERCURY_MD5_H_

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint32 total[2];          /*!< number of bytes processed  */
    uint32 state[4];          /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */
}MD5_CTX;


int MD5_Init( MD5_CTX *ctx );
int MD5_Free( MD5_CTX *ctx );
int MD5_Starts( MD5_CTX *ctx );
int MD5_Update( MD5_CTX *ctx, const unsigned char *input, size_t ilen );
int MD5_Finish( MD5_CTX *ctx, unsigned char output[20] );
int MD5_Clone( MD5_CTX *dst, const MD5_CTX *src );
int MD5(const uint8 *input, size_t ilen, uint8 output[20]);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

