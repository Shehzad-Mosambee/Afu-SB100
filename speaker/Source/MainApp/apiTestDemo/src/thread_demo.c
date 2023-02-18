#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryThread.h"
#include "MercuryMem.h"

void testThreadStatus() {
    unsigned int nThreadCount = MercuryThreadCount();
    osiThreadStatus_t * pThreadStatus = (osiThreadStatus_t *)LocalAlloc(LMEM_ZEROINIT, nThreadCount * sizeof(osiThreadStatus_t));
    MercuryThreadGetAllStatus(pThreadStatus, nThreadCount);

    // HANDLE hCurThread = GetCurrentThread();
	// MercuryTrace("GetCurrentThreadId dwCurId = [0x%08X]\r\n", hCurThread);
	HANDLE hThread = GetCurrentThread();
	MercuryTrace("GetCurrentThread hThread = [0x%08X]\r\n", hThread);

	for (int i = 0; i < nThreadCount; i++)
	{
		// osiThreadStatus_t * pTemp = (osiThreadStatus_t *)(((void *)pThreadStatus) + sizeof(osiThreadStatus_t) * i);
		osiThreadStatus_t * pTemp = pThreadStatus + i;
		MercuryTrace("name = [%s]\r\n",pTemp->name);
		MercuryTrace("thread_number = [%d]\r\n",pTemp->thread_number);
		MercuryTrace("stack_base = [%x]\r\n",pTemp->stack_base);
		MercuryTrace("stack_alloc_size = [%d]\r\n",pTemp->stack_alloc_size);
		MercuryTrace("stack_min_remained = [%d]\r\n",pTemp->stack_min_remained);
        if ((unsigned int)hThread == (unsigned int)(pTemp->handler)) {
            MercuryTrace("================== Find the thread info \r\n");
            MercuryTrace("GetCurrentThread hThread = [0x%08X]\r\n", hThread);
            MercuryTrace("MercuryThreadGetAllStatus handler = [0x%08X]\r\n", pTemp->handler);
        }
	}
	
	LocalFree(pThreadStatus);
}

void test_thread(void)
{
	while (1)
	{
		MercuryTrace("Test Thread - run app...\r\n");
		Sleep(10 *1024);
	}
}

void test_thread_stack_alloc(void) {
	MERCURY_HEAP_STATUS pHeapStatus;
	// 1. Get the heap usage
	GetAppHeapUsageStatus(&pHeapStatus);
	MercuryTrace("befor nTotalSize = [%d] nUsedSize = [%d] nFreeSize = [%d]\r\n",pHeapStatus.nTotalSize,pHeapStatus.nUsedSize,(pHeapStatus.nTotalSize - pHeapStatus.nUsedSize));

	// 2. malloc buffer from the heap
	char *buffer = LocalAlloc(LMEM_ZEROINIT, 32 * 1024);

	// 3. create thread with the above buffer as the thread stack
	CreateThreadWithStack(NULL,(LPVOID)buffer ,32*1024, test_thread, 0, NULL,STACK_SIZE_RESERVATION, NULL);

	// 4. Get the heap usage again
	GetAppHeapUsageStatus(&pHeapStatus);
	MercuryTrace("after nTotalSize = [%d] nUsedSize = [%d] nFreeSize = [%d]\r\n",pHeapStatus.nTotalSize,pHeapStatus.nUsedSize,(pHeapStatus.nTotalSize - pHeapStatus.nUsedSize));
}
