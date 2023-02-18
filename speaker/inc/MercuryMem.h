/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2020
                 Author: 
#########################################################################*/

#ifndef __MERCURY_MEM_H__
#define __MERCURY_MEM_H__

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define    LMEM_FIXED           0
#define    LMEM_ZEROINIT     0x0040

typedef struct _Mercury_HEAP_STATUS
{
    unsigned int nUsedSize;
    unsigned int nTotalSize;
} MERCURY_HEAP_STATUS;

extern HLOCAL LocalAlloc(UINT uFlags,UINT uBytes);
extern HLOCAL LocalReAlloc(void * memblock,UINT size);
extern HLOCAL LocalFree(HLOCAL hMem);
extern PVOID SecureZeroMemory(PVOID ptr,SIZE_T cnt);
extern int	GetAppHeapUsageStatus(MERCURY_HEAP_STATUS *pHeapStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif //__MERCURY_MEM_H__
