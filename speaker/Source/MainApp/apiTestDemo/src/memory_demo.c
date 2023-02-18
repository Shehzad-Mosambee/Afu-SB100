#include "MercuryMem.h"
#include "MercuryPlat.h"
#include "MercuryDef.h"

extern int	GetAppHeapUsageStatus(MERCURY_HEAP_STATUS *pHeapStatus);
void allocation_memory(void)
{
    int nBufferSize = 2 * 1024;
    char *pBuffer = NULL;
    MERCURY_HEAP_STATUS heapStatus;

    GetAppHeapUsageStatus(&heapStatus);
    MercuryTrace("befor nTotalSize = [%d] nUsedSize = [%d] nFreeSize = [%d]\r\n",
                heapStatus.nTotalSize,heapStatus.nUsedSize,(heapStatus.nTotalSize - heapStatus.nUsedSize));
    
    pBuffer = LocalAlloc(LMEM_ZEROINIT,nBufferSize);

    
    GetAppHeapUsageStatus(&heapStatus);
    MercuryTrace("after nTotalSize = [%d] nUsedSize = [%d] nFreeSize = [%d]\r\n",
                heapStatus.nTotalSize,heapStatus.nUsedSize,(heapStatus.nTotalSize - heapStatus.nUsedSize));

    LocalFree(pBuffer);
}