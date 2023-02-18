#ifndef _MERCURY_BASE64_H_
#define _MERCURY_BASE64_H_

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

int Base64_encode( unsigned char *dst, size_t dlen, size_t *olen,
                   const unsigned char *src, size_t slen );

int Base64_decode( unsigned char *dst, size_t dlen, size_t *olen,
                   const unsigned char *src, size_t slen );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


