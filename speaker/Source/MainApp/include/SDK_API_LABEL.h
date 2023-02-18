#ifndef _QYY_API_LABEL_H
#define _QYY_API_LABEL_H

#include "MercuryDef.h"

#if 0
//deprecated
extern BOOL label_init(void);

extern int label_delete(const char* label);
extern int label_read_int(const char* label, int* result);
extern int label_read_data(const char*label, unsigned char*buffer, int buflen, int* resultlen);
extern int label_write_int(const char *label, int value);
extern int label_write_data(const char *label, unsigned char * data, int datalen);
#endif

#endif

