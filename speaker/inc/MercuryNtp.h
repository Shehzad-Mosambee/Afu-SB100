#ifndef __MERCURY_SEC_H__
#define __MERCURY_SEC_H__
#include "MercuryDef.h"

typedef enum 
{
	QL_NTP_SUCCESS,
	QL_NTP_GENERIC_FAILURE,
}QL_NTP_ERROR_CODE;

void MercuryNtpSync(void);
QL_NTP_ERROR_CODE MercuryNtpSetCid(int pdp);

QL_NTP_ERROR_CODE MercuryNtpSetServer(char *ntp_svr);


#endif

