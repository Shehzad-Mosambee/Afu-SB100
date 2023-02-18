/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author:
#########################################################################*/

#ifndef __MERCURY_DEF_H__
#define __MERCURY_DEF_H__


#ifdef __cplusplus
extern "C" {
#endif

#ifndef INT32
typedef signed long INT32;
#endif

#ifndef LONG
typedef signed long LONG;
#endif

#ifndef HANDLE
typedef unsigned int HANDLE;
#endif

#ifndef LPOVERLAPPED
typedef void* LPOVERLAPPED ;
#endif

#ifndef PLONG
typedef signed long  *PLONG;
#endif

#ifndef uint8
typedef unsigned char       uint8;
#endif

#ifndef uint16
typedef unsigned short      uint16;
#endif

#ifndef uint32
typedef unsigned int        uint32;
#endif

#ifndef uint8_t
typedef unsigned char       uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short      uint16_t;
#endif

#ifndef uint32_t
typedef unsigned int    uint32_t;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef BOOLEAN
typedef int BOOLEAN;
#endif

#ifndef bool
	typedef unsigned char bool;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef FLOAT
typedef float FLOAT;
#endif

#ifndef PFLOAT
typedef FLOAT *PFLOAT;
#endif

#ifndef PBOOL
typedef BOOL  *PBOOL;
#endif

#ifndef LPBOOL
typedef BOOL  *LPBOOL;
#endif

#ifndef PBYTE
typedef BYTE  *PBYTE;
#endif

#ifndef LPBYTE
typedef BYTE  *LPBYTE;
#endif

#ifndef PINT
typedef int  *PINT;
#endif

#ifndef LPINT
typedef int  *LPINT;
#endif

#ifndef PWORD
typedef WORD  *PWORD;
#endif

#ifndef LPWORD
typedef WORD  *LPWORD;
#endif

#ifndef LPLONG
typedef long  *LPLONG;
#endif

#ifndef PDWORD
typedef DWORD  *PDWORD;
#endif

#ifndef LPDWORD
typedef DWORD  *LPDWORD;
#endif

#ifndef LPVOID
typedef  void*   LPVOID;
#endif

#ifndef LPCVOID
typedef const void  *LPCVOID;
#endif

#ifndef INT
typedef int INT;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef PUINT
typedef unsigned int *PUINT;
#endif

#ifndef LPTSTR
typedef const char* LPTSTR;
#endif

#ifndef LPSTR
typedef char* LPSTR;
#endif

#ifndef size_t
typedef unsigned int                size_t;
#endif


#ifndef LPCTSTR
typedef const unsigned short* LPCTSTR;
#endif

#ifndef HLOCAL
typedef void* HLOCAL;
#endif

#ifndef PVOID
typedef void* PVOID;
#endif

#ifndef VOID
typedef void VOID;
#endif


#ifndef SIZE_T
//typedef size_t SIZE_T;
typedef unsigned int SIZE_T;

#endif

#ifndef HWND
typedef void* HWND;
#endif

#ifndef ULONG_PTR
typedef unsigned long* ULONG_PTR;
#endif

#ifndef ULONG
typedef  unsigned long  ULONG;
#endif

typedef void* TIMER_PTR;

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTE;

typedef  SECURITY_ATTRIBUTE* LPSECURITY_ATTRIBUTES;

typedef	long long time_t;
typedef	long long int64_t;

/**
 * opaque data structure for mutex
 */
typedef struct osiMutex osiMutex_t;

/**
 * opaque data structure for semaphore
 */
typedef struct osiSemaphore osiSemaphore_t;

typedef struct osiMessageQueue osiMessageQueue_t;



#ifndef NULL
#define NULL                         0
#endif

#define TRUE     1
#define FALSE    0

#define true     1
#define false    0

// temp std inc
#define strlen_m strlen
#define memset_m memset
#define atoi_m 	atoi

typedef WORD wchar;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
