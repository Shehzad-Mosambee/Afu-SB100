/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2020
                 Author: 
#########################################################################*/

#ifndef __MERCURY_SYN_H__
#define __MERCURY_SYN_H__

#include "MercuryDef.h"
#include "MercuryThread.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _RTL_CRITICAL_SECTION {
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} CRITICAL_SECTION, *LPCRITICAL_SECTION;

/*
lpEventAttributes:Ignored. Must be NULL.
bManualReset:
           true --you must use the ResetEvent function to manually reset the state to nonsignaled
           false--the system automatically resets the state to nonsignaled after a single waiting thread has been released
InitialState:TRUE the initial state is signaled; otherwise, it is nonsignaled
lpName:
*/
extern HANDLE CreateEvent( LPSECURITY_ATTRIBUTES lpEventAttributes,
                                            BOOL bManualReset,
                                            BOOL InitialState,
                                            LPTSTR lpName);
extern HANDLE OpenEvent(DWORD dwDesiredAccess,BOOL bInheritHandle, LPCTSTR lpName);
/*
BOOL return 0  success
     return 1  faile   use GetLastError to catch the error num
*/
extern BOOL PulseEvent(HANDLE hEvent);
/*
BOOL return 0  success
     return 1  faile   use GetLastError to catch the error num
*/
extern BOOL ResetEvent(HANDLE hEvent);
/*
BOOL return 0  success
     return 1  faile   use GetLastError to catch the error num
*/
extern BOOL SetEvent(HANDLE hEvent);

extern DWORD WaitForSingleObject(HANDLE hHandle,DWORD dwMilliseconds);
/*
非空的lpCriticalSection
请将内容初始化为0
*/
extern void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
/*
lpCriticalSection必须被InitializeCriticalSection过
*/
extern void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
/*
lpCriticalSection必须被InitializeCriticalSection过
*/
extern void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
/*
lpCriticalSection必须被InitializeCriticalSection过
*/
extern void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

// sem
osiSemaphore_t *osiSemaphoreCreate(uint32_t max_count, uint32_t init_count);
void osiSemaphoreDelete(osiSemaphore_t *semaphore);
void osiSemaphoreAcquire(osiSemaphore_t *semaphore);
bool osiSemaphoreTryAcquire(osiSemaphore_t *semaphore, uint32_t timeout);
void osiSemaphoreRelease(osiSemaphore_t *semaphore);
//mutex
osiMutex_t * osiMutexCreate(void);
void  osiMutexDelete(osiMutex_t *mutex);
void  osiMutexLock(osiMutex_t *mutex);
bool  osiMutexTryLock(osiMutex_t *mutex, uint32_t timeout);
void  osiMutexUnlock(osiMutex_t *mutex);
/**
 * create a message queue
 *
 * \param msg_count maximum message count can be hold in queue
 * \param msg_size	size of each message in bytes
 * \return
 *		- message queue pointer
 *		- NULL on invalid parameter or out of memory
 */
osiMessageQueue_t *osiMessageQueueCreate(uint32_t msg_count, uint32_t msg_size);

/**
 * delete a message queue
 *
 * When \p mq is NULL, nothing will be done, just as \p free.
 *
 * \param mq	message queue pointer
 */
void osiMessageQueueDelete(osiMessageQueue_t *mq);

/**
 * put a message to message queue
 *
 * \p msg should hold content size the same as \p msg_size specified at
 * \p osiMessageQueueCreate.
 *
 * After put, the content of \p msg will be copied to message queue.
 *
 * When \p mq is full, it will be blocked until there are rooms.
 *
 * \param mq	message queue pointer
 * \param msg	mesage pointer
 * \return
 *		- true on success
 *		- false on invalid parameter
 */
bool osiMessageQueuePut(osiMessageQueue_t *mq, const void *msg);

/**
 * put a message to message queue with timeout
 *
 * This can be called in ISR. And in ISR, \p timeout must be 0.
 *
 * \param mq		message queue pointer
 * \param msg		mesage pointer
 * \param timeout	timeout in milliseconds
 * \return
 *		- true on success
 *		- false on invalid parameter or timeout
 */
bool osiMessageQueueTryPut(osiMessageQueue_t *mq, const void *msg, uint32_t timeout);

/**
 * get a message to message queue
 *
 * \p msg should be able tp hold content size of \p msg_size specified at
 * \p osiMessageQueueCreate.
 *
 * After get, the content of message will be copied to \p msg.
 *
 * When \p mq is empty, it will be blocked until there are messages.
 *
 * \param mq	message queue pointer
 * \param msg	mesage pointer
 * \return
 *		- true on success
 *		- false on invalid parameter
 */
bool osiMessageQueueGet(osiMessageQueue_t *mq, void *msg);

/**
 * get a message to message queue with timeout
 *
 * This can be called in ISR. And in ISR, \p timeout must be 0.
 *
 * \param mq		message queue pointer
 * \param msg		mesage pointer
 * \param timeout	timeout in milliseconds
 * \return
 *		- true on success
 *		- false on invalid parameter or timeout
 */
bool osiMessageQueueTryGet(osiMessageQueue_t *mq, void *msg, uint32_t timeout);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__MERCURY_SYN_H__
