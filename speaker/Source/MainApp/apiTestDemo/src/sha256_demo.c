#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercurySha.h"
#include "MercuryMem.h"
#include "MercuryThread.h"
#include <string.h>

#include "speakerapp.h"

void sha256_test(void)
{
    int ret;
    unsigned char sha256sum[20];
    unsigned char sha256Buff[64];
    char sha256_test_buf[] = {"12345678"};
    SHA256_CTX ctx;

    Sha256_Init(&ctx);

    ret = Sha256_Starts(&ctx, FALSE);
    MercuryTrace("Sha256_Starts ret = [%d]\r\n",ret);

    ret = Sha256_Update(&ctx, sha256_test_buf,sizeof(sha256_test_buf));
    MercuryTrace("Sha256_Update ret = [%d]\r\n",ret);

    ret = Sha256_Finish(&ctx, sha256sum);
    MercuryTrace("Sha256_Finish ret = [%d]\r\n",ret);

    ret = Sha256(sha256sum,sizeof(sha256sum),sha256Buff,FALSE);
    Dbg_dump(sha256sum,strlen(sha256sum));
    MercuryTrace("Sha256 ret = [%d] sha256Buff = [%s]\r\n",ret,sha256Buff);
}

void sha_test(void)
{
    // sha1 test
    MercuryTrace("sha1 test\r\n");
    sha1_test();

    // sha256 test
    MercuryTrace("sha256 test\r\n");
    sha256_test();
}