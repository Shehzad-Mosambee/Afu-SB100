/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2020
                 Author: 
#########################################################################*/

#ifndef __MERCURY_THREAD_H__
#define __MERCURY_THREAD_H__

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

// APP�߳����ȼ�ȡֵ��Χ(2-32),�̴߳���Ĭ�����ȼ�8
typedef enum mercuryThreadPriority
{
	OSI_PRIORITY_BELOW_LOW = 2, 
    OSI_PRIORITY_LOW = 8,  // �̴߳���Ĭ�����ȼ�
    OSI_PRIORITY_BELOW_NORMAL = 16,
    OSI_PRIORITY_NORMAL = 24,  
    OSI_PRIORITY_ABOVE_NORMAL = 30,
    OSI_PRIORITY_HIGH = 31,
    OSI_PRIORITY_REALTIME = 32, 
} mercuryThreadPriority_t;


/**
 * thread state
 */
typedef enum
{
    OSI_THREAD_STATE_RUNNING,   ///< running
    OSI_THREAD_STATE_READY,     ///< ready, and there are higher priority task in running
    OSI_THREAD_STATE_BLOCKED,   ///< waiting temporal or external events
    OSI_THREAD_STATE_SUSPENDED, ///< suspend explicitly
    OSI_THREAD_STATE_DELETED,   ///< deleted but not release resources
    OSI_THREAD_STATE_INVALID,   ///< placeholder for invalid
} osiThreadState_t;

/**
 * thread status
 */
typedef struct osiThreadStatus
{
    void *handler;               ///< implementation dependent handler
    const char *name;            ///< thread name
    unsigned thread_number;      ///< numeric number
    osiThreadState_t state;      ///< state
    unsigned curr_priority;      ///< current priority, may be inherited
    unsigned base_priority;      ///< base priority
    void *stack_base;            ///< stack base address (lowest address)
    unsigned stack_alloc_size;   ///< satck allocation size
    unsigned stack_min_remained; ///< minimal unused stack size
} osiThreadStatus_t;

/*
CreateThread ��fdwCreateȡֵ
fdwCreate = CREATE_SUSPENDED  ----- �̴߳������Զ����У�cbStack����ֵ��Ч
fdwCreate = STACK_SIZE_RESERVATION  ----- �̴߳������Զ����У�cbStack����ֵ��Ч��ʹ��Ĭ��ջ��С��
*/
#define  CREATE_SUSPENDED             4
#define STACK_SIZE_RESERVATION  0x1000

typedef VOID (*LPTHREAD_START_ROUTINE) (ULONG argc,LPVOID lpThreadParameter);


/// Task context static descriptor
typedef struct
{
    /// A pointer to a task body.
    /// The task body corresponds to a function implementing the task.
    /// When the task enters in ACTIVE state for the first time, the task body is
    /// invoked with the address of the dynamic context as a parameter.
    void  (*TaskBody)(void *);
    /// A pointer to an exit function. The exit function, when it exists,
    /// is called when the task is stopped. The exit function, when called,
    /// is given the address of the dynamic context.
    void  (*TaskExit)(void *);
    /// A pointer to a string that contains the name of the task.
    const char *Name;
    /// A stack size expressed in number of double words, to be allocated for the task.
    uint16   StackSize;
    /// The priority [0..255] to be given to the task.
    uint8    Priority;
} mercury_sxr_TaskDesc_t;

typedef struct _Mercury_TASK_HANDLE
{
    mercury_sxr_TaskDesc_t desc; // It must be the first one. So, sxr_TaskDesc_t* and TASK_HANDLE* are the same
    uint32 nErrCode;
    uint8 nTaskId;
    uint8 nMailBoxId;
}MERCURY_TASK_HANDLE;


typedef struct
{
    unsigned short MessageID;
    unsigned int dataLen;
    void* pdata;
}MERCURY_MESSAGE_S;

/*
lpsa: NULL
cbStack:��ջ��С�����fdwCreateʹ�ã�NULLʹ��Ĭ��ֵ
lpStartAddr:��ں���
lpvThreadParam:��ں�������
fdwCreate:CREATE_SUSPENDED   STACK_SIZE_PARAM_IS_A_RESERVATION
lpIDThread:�����߳�ID
*/
extern HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpsa,DWORD cbStack,
                                             LPTHREAD_START_ROUTINE lpStartAddr,ULONG argc,LPVOID lpvThreadParam,
                                             DWORD fdwCreate,LPDWORD lpIDThread);

/*
lpsa: NULL
pUserStack:�û��ڴ�ռ�
cbStack:��ջ��С�����fdwCreateʹ�ã�NULLʹ��Ĭ��ֵ
lpStartAddr:��ں���
lpvThreadParam:��ں�������
fdwCreate:CREATE_SUSPENDED   STACK_SIZE_PARAM_IS_A_RESERVATION
lpIDThread:�����߳�ID
*/
extern HANDLE CreateThreadWithStack(LPSECURITY_ATTRIBUTES lpsa,LPVOID pUserStack,DWORD cbStack,
										 LPTHREAD_START_ROUTINE lpStartAddr,ULONG argc,LPVOID lpvThreadParam,
										 DWORD fdwCreate,LPDWORD lpIDThread);


/*
 dwExitCode: null
*/
extern VOID ExitThread(DWORD dwExitCode);
 /*
 �����߳�ID
*/
extern HANDLE GetCurrentThread(void);
  /*
 �����߳�ID
*/
extern DWORD GetCurrentThreadId(void);
  /*
�����߳����ȼ�
*/
extern int GetThreadPriority(HANDLE hThread);


extern DWORD ResumeThread(HANDLE hThread);
/*
�����߳����ȼ�
ֵֻ��ʹ�� mercuryThreadPriority_t ��ö��ֵ
*/
extern BOOL SetThreadPriority(HANDLE hThread,int nPriority);
/*
��λms (OS 1��tick 20ms),����С��20ms��ʵ��20ms��25ms ��Ҫ2��tick����40ms
*/
extern void Sleep(DWORD dwMilliseconds);
// todo
extern DWORD SuspendThread(HANDLE hThread);
extern BOOL TerminateThread(HANDLE hThread,DWORD dwExitCode);

/**************************************************************************************
�ӵ�ǰ�߳����߳�threadID����msg
msg �� threadID ������Ч
**************************************************************************************/
extern int MercurySendMessage(MERCURY_MESSAGE_S* msg, HANDLE threadID);
/**************************************************************************************
�����߳�threadID��msg
�ýӿ�Ϊ����ʽ�ӿڣ����øýӿں�
���threadID����msg����������
���threadID������msg�������ǰ�߳�ֱ��threadID�յ�msg��
**************************************************************************************/
extern int MercuryGetMessage(MERCURY_MESSAGE_S** msg, HANDLE threadID);
/**************************************************************************************
�����߳�threadID��msg
�ýӿ�Ϊ������ʽ�ӿڣ����øýӿں������߳�threadID�Ƿ����msg����������
���threadID����msg�򷵻سɹ�0 �� msg
���threadID������msg���򷵻�-1��msg��������
**************************************************************************************/
extern int MercuryPeekMessage(MERCURY_MESSAGE_S** msg,HANDLE threadID);

// ����assert �ϵ㣬������
extern 	void MercuryBreakPoint(void);

extern void SleepUs(DWORD us);

// ��ȡ��ǰ�߳�����
unsigned int MercuryThreadCount(void);

/*
	��ȡָ�������߳�״̬,����ͨ��MercuryThreadCount��ȡ��ǰ�߳�����
	@param pStatus ��Ҫ�ⲿ������Ӧcount buf����
*/
int MercuryThreadGetAllStatus(osiThreadStatus_t *pStatus, uint32_t count);


/*
���ò�����ջ����жϽӿ�(����汾��Ч)
*/
int MercurySetThreadNoCheckStackOverFlow(HANDLE thread);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__MERCURY_THREAD_H__
