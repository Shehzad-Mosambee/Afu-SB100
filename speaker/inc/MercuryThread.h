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

// APP线程优先级取值范围(2-32),线程创建默认优先级8
typedef enum mercuryThreadPriority
{
	OSI_PRIORITY_BELOW_LOW = 2, 
    OSI_PRIORITY_LOW = 8,  // 线程创建默认优先级
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
CreateThread 中fdwCreate取值
fdwCreate = CREATE_SUSPENDED  ----- 线程创建后不自动运行，cbStack参数值有效
fdwCreate = STACK_SIZE_RESERVATION  ----- 线程创建后自动运行，cbStack参数值无效，使用默认栈大小。
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
cbStack:堆栈大小，配合fdwCreate使用，NULL使用默认值
lpStartAddr:入口函数
lpvThreadParam:入口函数参数
fdwCreate:CREATE_SUSPENDED   STACK_SIZE_PARAM_IS_A_RESERVATION
lpIDThread:返回线程ID
*/
extern HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpsa,DWORD cbStack,
                                             LPTHREAD_START_ROUTINE lpStartAddr,ULONG argc,LPVOID lpvThreadParam,
                                             DWORD fdwCreate,LPDWORD lpIDThread);

/*
lpsa: NULL
pUserStack:用户内存空间
cbStack:堆栈大小，配合fdwCreate使用，NULL使用默认值
lpStartAddr:入口函数
lpvThreadParam:入口函数参数
fdwCreate:CREATE_SUSPENDED   STACK_SIZE_PARAM_IS_A_RESERVATION
lpIDThread:返回线程ID
*/
extern HANDLE CreateThreadWithStack(LPSECURITY_ATTRIBUTES lpsa,LPVOID pUserStack,DWORD cbStack,
										 LPTHREAD_START_ROUTINE lpStartAddr,ULONG argc,LPVOID lpvThreadParam,
										 DWORD fdwCreate,LPDWORD lpIDThread);


/*
 dwExitCode: null
*/
extern VOID ExitThread(DWORD dwExitCode);
 /*
 返回线程ID
*/
extern HANDLE GetCurrentThread(void);
  /*
 返回线程ID
*/
extern DWORD GetCurrentThreadId(void);
  /*
返回线程优先级
*/
extern int GetThreadPriority(HANDLE hThread);


extern DWORD ResumeThread(HANDLE hThread);
/*
设置线程优先级
值只能使用 mercuryThreadPriority_t 该枚举值
*/
extern BOOL SetThreadPriority(HANDLE hThread,int nPriority);
/*
单位ms (OS 1个tick 20ms),比如小于20ms，实际20ms，25ms 需要2个tick，即40ms
*/
extern void Sleep(DWORD dwMilliseconds);
// todo
extern DWORD SuspendThread(HANDLE hThread);
extern BOOL TerminateThread(HANDLE hThread,DWORD dwExitCode);

/**************************************************************************************
从当前线程往线程threadID发送msg
msg 和 threadID 必须有效
**************************************************************************************/
extern int MercurySendMessage(MERCURY_MESSAGE_S* msg, HANDLE threadID);
/**************************************************************************************
接收线程threadID的msg
该接口为阻塞式接口，调用该接口后，
如果threadID存在msg则立即返回
如果threadID不存在msg，则挂起当前线程直至threadID收到msg。
**************************************************************************************/
extern int MercuryGetMessage(MERCURY_MESSAGE_S** msg, HANDLE threadID);
/**************************************************************************************
接收线程threadID的msg
该接口为非阻塞式接口，调用该接口后，无论线程threadID是否存在msg都立即返回
如果threadID存在msg则返回成功0 和 msg
如果threadID不存在msg，则返回-1，msg不做操作
**************************************************************************************/
extern int MercuryPeekMessage(MERCURY_MESSAGE_S** msg,HANDLE threadID);

// 引起assert 断点，调试用
extern 	void MercuryBreakPoint(void);

extern void SleepUs(DWORD us);

// 获取当前线程数量
unsigned int MercuryThreadCount(void);

/*
	获取指定数量线程状态,可以通过MercuryThreadCount获取当前线程数量
	@param pStatus 需要外部申请相应count buf传入
*/
int MercuryThreadGetAllStatus(osiThreadStatus_t *pStatus, uint32_t count);


/*
设置不进行栈溢出判断接口(特殊版本有效)
*/
int MercurySetThreadNoCheckStackOverFlow(HANDLE thread);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__MERCURY_THREAD_H__
