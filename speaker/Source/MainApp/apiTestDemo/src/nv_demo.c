#include "MercuryNv.h"
#include "MercuryPlat.h"

#include "nv_demo.h"

static void _nv_read(void)
{
    int num;
    if (0 == NV_Read(NV_TEST,sizeof(int),&num))
        MercuryTrace("NV Read num = %d\r\n",num);
    else
        MercuryTrace("NV Read error\r\n");
}

static void _nv_write(void)
{
    int num = 123;
    if (0 == NV_Write(NV_TEST,sizeof(int),&num))
        MercuryTrace("NV Write num = %d\r\n",num);
    else
        MercuryTrace("NV Write error\r\n");
}

static void _nv_delete(void)
{
    if (0 == NV_Delete(NV_TEST))
        MercuryTrace("NV Delete success\r\n");
    else
        MercuryTrace("NV Delete fail\r\n");
}

void nv_test(void)
{
    // nv delete
    _nv_delete();

    // nv write
    _nv_write();

    // nv read
    _nv_read();
}