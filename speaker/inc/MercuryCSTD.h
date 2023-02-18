/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2020
                 Author:
#########################################################################*/

#ifndef _MERCURY_CSTD_H__
#define _MERCURY_CSTD_H__

#include "MercuryDef.h"
#include "stdarg.h"  // for va_list

#ifdef __cplusplus
extern "C" {
#endif

struct tm
{
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
};

typedef int (* COMPAR)( const void *, const void * );
typedef int (*SPRINTF_FUNC) ( char *string, const char *format, ... );
typedef int (*SNPRINTF_FUNC) ( char *string,size_t size, const char *format, ... );
typedef int (*SSCANF_FUNC) (const char * string, const char * format, ...);
extern SPRINTF_FUNC MC_SPRINTF;
extern SNPRINTF_FUNC MC_SNPRINTF;
extern SSCANF_FUNC MC_SSCANF;
void* memcpy ( void *dst, const void *src, size_t count );
void* memmove ( void *dst, const void *src, size_t count );
char*  strcpy ( char *dst, const char *src );
char* strncpy ( char *dst, const char *src, size_t count );
char* strcat (char* dst, const char* src);
char* strncat (char* dst, const char* src,  size_t count);
int memcmp (const void* src1, const void* src2, size_t count);
int strcmp (const char* src1, const char* src2);
int strncmp (const char* src1, const char* src2, size_t count);
int strcoll (const char* src1, const char* src2);
size_t strxfrm (char* dst, const char* src, size_t count);
void* memchr (const void* src, int ch, size_t count);
char* strchr (const char* src, int ch);
size_t strcspn (const char* src1, const char* src2);
char* strpbrk (const char* src1, const char* src2);
char* strrchr (const char* src, int ch);
size_t strspn (const char* src1, const char* src2);
char* strstr (const char* src1, const char* src2);
char* strtok (char* src1, const char* src2);
void* memset (void* dst, int value, size_t count);
char* strerror (int errnum);
size_t strlen (const char* src);

//ARMctype.h
int isalpha (int ch);
int iscntrl (int ch);
int isgraph (int ch);
int islower (int ch);
int isprint (int ch);
int ispunct (int ch);
int isspace (int ch);
int isupper (int ch);
int isxdigit (int ch);
int tolower (int ch);
int toupper (int ch);

//ARMstdlib.h
int atoi (const char*string);
long int atol (const char*string);
long int strtol (const char*nptr, char** endptr, int base);
unsigned long int strtoul (const char* nptr, char** endptr, int base);
int rand (void);
void srand (unsigned int seed);
void qsort (void* base, size_t num, size_t size,  COMPAR par);
int  abs(int arg);
//ARMstdio.h
int snprintf(char *string, size_t size, const char *format, ...);
int vsprintf (char* string, const char* format, va_list arg);
int vsnprintf (char* str, size_t size, const char* format, va_list ap);

// math
double floor(double arg);
double pow(double arg1, double arg2);
double fabs(double arg);
double strtod(const char *nptr, char **endptr);
float strtof(const char *nptr, char **endptr);
double atof(const char *__nptr);
double sin(double arg);
double cos(double arg);
double asin(double arg);
double acos(double arg);    
double sqrt(double arg);
//wstring todo?
wchar* wstrcpy (wchar* dst, const wchar* src);
wchar* wstrncpy (wchar* dst, const wchar* src, size_t count);
wchar* wstrcat (wchar* dst, const wchar* src);
wchar* wstrncat (wchar* dst, const wchar* src, size_t count);
int wstrcmp (const wchar* src1, const wchar* src2);
int wstrncmp (const wchar* src1, const wchar* src2, size_t count);
size_t wstrlen (const wchar* src);

//Exceptions todo?
void rt_raise(int sig, int type);

#define sprintf  		MC_SPRINTF
#define sprintf_m   	 sprintf
#define snprintf  		MC_SNPRINTF
#define snprintf_m   	 snprintf
#define memcpy_m         memcpy 
#define memmove_m        memmove 
#define strcpy_m         strcpy 
#define strncpy_m        strncpy 
#define strcat_m         strcat 
#define strncat_m        strncat 
#define memcmp_m         memcmp 
#define strcmp_m         strcmp 
#define strncmp_m        strncmp 
#define strcoll_m        strcoll 
#define strxfrm_m        strxfrm 
#define memchr_m         memchr 
#define strchr_m         strchr 
#define strcspn_m        strcspn 
#define strpbrk_m         strpbrk 
#define strrchr_m        strrchr 
#define strspn_m         strspn 
#define strstr_m         strstr 
#define strtok_m         strtok 
#define memset_m         memset 
#define strerror_m       strerror 
#define strlen_m         strlen 
#define isalpha_m        isalpha 
#define iscntrl_m        iscntrl 
#define isgraph_m        isgraph 
#define islower_m        islower 
#define isprint_m        isprint 
#define ispunct_m        ispunct 
#define isspace_m        isspace 
#define isupper_m        isupper 
#define isxdigit_m       isxdigit 
#define tolower_m        tolower 
#define toupper_m        toupper 
#define atoi_m           atoi 
#define atol_m           atol 
#define strtol_m         strtol 
#define strtoul_m        strtoul 
#define rand_m           rand 
#define srand_m          srand 
#define qsort_m          qsort 
#define vsprintf_m       vsprintf 
#define vsnprintf_m      vsnprintf 
#define sscanf           MC_SSCANF
#define sscanf_m		 sscanf
// todo
#define wstrcpy_m        wstrcpy 
#define wstrncpy_m       wstrncpy 
#define wstrcat_m        wstrcat 
#define wstrncat_m       wstrncat 
#define wstrcmp_m        wstrcmp 
#define wstrncmp_m       wstrncmp 
#define wstrlen_m        wstrlen 


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_MERCURY_CSTD_H__