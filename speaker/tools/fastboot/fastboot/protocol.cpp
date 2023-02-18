#include "stdafx.h"
#include "protocol.h"
#include "fastboot.h"
#include "sha2.h"
#include "rsa.h"

extern RDA_KEY_PAIR_T keyPair;
extern DLL_SIGN_FUNC dll_sign_func;
extern DLL_VERITY_FUNC dll_verify_func;

extern BOOL g_bISRSA;

int timeOut = 5 * 1000;

#define HDLC_FLAG               0x7E
#define HDLC_ESCAPE             0x7D
#define HDLC_ESCAPE_MASK        0x20

#define BUFF_LENGTH				(1024)
#define RANDCOUNT				40
static char mError[128];

unsigned char databuf[BUFF_LENGTH*4] = { 0 };
unsigned char tmp[BUFF_LENGTH*4] = { 0 };
unsigned char readBuf[BUFF_LENGTH * 4] = { 0 };

#define ISRSA


#define KEY_LEN 256

#define RSA_N   "d96c1340612be438a23013059129"\
				"6f7e3c6440d29b1403120a2295386f"\
				"0fbf190ff1a3e7c5008864ae4404bc"\
				"7a105d3ebdaedc0664f06759006653"\
				"fd91907381e0f3966903ee4f3c648c"\
				"76ed8d48b37bc754e81e649e04728d"\
				"f2430c8ef5aeddcb9523e87cf0df4c"\
				"65f25faeb3936744cbd761ab9e3748"\
				"f2d58a45d5c6098a042a5a93ad3506"\
				"761c546cbe1e6f16abc425848bbe2e"\
				"73bf63b08e588f6791ef78551a8f32"\
				"c5fda5d06672b8b194d99e79cf23b5"\
				"e392715774aeb550c7b7d40257a2d6"\
				"10d9fcfef74c3ca78a92671c732a3b"\
				"e891045e2ff0b3681554ed5902d7c5"\
				"561c60e1a420150d03f1844646b72f"\
				"cae8571e66afcab5197e7306ff2b25"\
				"9841"


#define RSA_E   "10001"

#define RSA_D   "7c55c7b49b5d244816c7f338cdc8aa"\
				"4fc06d7c7c73debfdac31cffc4a40e"\
				"9733d7e051e12ad1a12e2252177b49"\
				"3aae7840b50c7243ab0f0ee0a1a40a"\
				"7001f16c269e66376bde0e45f52236"\
				"a8a6911ac3003cd7f8f9853bf9de9e"\
				"da2506640d3a1d400ea6d73c50163a"\
				"7e9ea523607a3fe76a261aa481ed1f"\
				"a16a4f816e9ce9e56615119d1c3234"\
				"21194365d031de1cf55e633f2d9617"\
				"1254988ef4036289c005d4441d228c"\
				"f4b25bc7d80667408858ade1db67e3"\
				"5765ce5da7e0775a6774f7102db160"\
				"9548183fd298d12a385d5263c836e7"\
				"bd55ac35009bd150522428b34beba8"\
				"a9bc9828981853076260ac486e74fe"\
				"bb0515b4513ad44a3310c67ca8fc09"\
				"79"

#define RSA_P   "ec9c2b929172213791b976067080"\
				"871f900432d767e3a5cf8ea86d4403"\
				"5292a38b94ef1da2b71ef3a85f6612"\
				"fc3dfeeb11d2ad84184969af54a12b"\
				"5a35bb39569739f177f4ccd09592a0"\
				"8cfc8813538cc29c140b129bf6f7bf"\
				"5b5b2b530610285c63bdcaac810b54"\
				"8c89a2108b8e1179784b9940da9ed4"\
				"167c691cda96717dfb"



#define RSA_Q   "eb3d5d177f8824ba7b9a701c504a"\
				"fc85e7b28e41cae3985f33c8ae0fe1"\
				"05c33653c673d7b3bc58d1fd57aeb3"\
				"b24ec1999fbb1d6485e91a140da103"\
				"7cae2fb11343b156370ffc6f74557e"\
				"154705643cb5ad524e68028a33bbc8"\
				"567f55d55b53603baeaeda5480c595"\
				"70d3386f90d1ad1232fe62f89ef491"\
				"437074ed50f16099f3"



#define RSA_DP  "b1a56b5d87102e7a5f51e4985f1d"\
				"50d654b5a24a7238cc4432b2a81ef9"\
				"73a49f5fec393f81ec8533a185c6c8"\
				"98541fb1f456be6a4025d6508a74db"\
				"6cc68cc4370c21b7f73d780d5f4f90"\
				"756a20ec89bd545c04fc6f4364408c"\
				"8e95bbc880f3dea9c74537071f7817"\
				"49d10839eddd83a734b765dff88469"\
				"6e5c883adeaf3794c3"


#define RSA_DQ  "36d40f9d897906ee7c1eabe0bd8a2c"\
				"bf5d188df5a1bf586560217708766f"\
				"2c67be30f411237053d765d0090513"\
				"1b2e1e8b4f4673868ea5286560d0fa"\
				"3f731837a0407612d16409ea387ef1"\
				"a0a30e10e45b6862a3f56b96d13ec9"\
				"8256542b3c0ca2fc7cfea0517f3bec"\
				"971e814a64c48d085ba85fc054ca82"\
				"c8744857e45a9aa7"

#define RSA_QP  "877ee9b80f23f36ce5782b9961e5"\
				"73f0d648d961a2518dad9c4e47d8eb"\
				"60310f8af8dfed4f0943a8303c1184"\
				"2be173c114baa723e06c3add8bfad6"\
				"b3059f7a056e8d7c1162bc134781d7"\
				"d2be4820bec39714bd8cb17c688623"\
				"5d977a843a89b3b15b95bcb240cd5e"\
				"77765e4e3f2fc03e92729569fa647e"\
				"e222a679972aa60b2f"

static UINT8 GetHexCode(UINT8 Binary)
{
	if ((Binary >= 0) && (Binary <= 9))
	{
		return Binary + '0';
	}
	else if ((Binary >= 0xA) && (Binary <= 0xF))
	{
		return Binary - 10 + 'A';
	}
}
static int Binary2Hex(UINT8 *Binary, UINT32 BinaryLen, UINT8 *Hex, UINT32 *HexLen)
{
	int i = 0, iHexLen = 0;

	for (i = 0; i < BinaryLen; i++)
	{
		Hex[iHexLen++] = GetHexCode(Binary[i] >> 4);
		Hex[iHexLen++] = GetHexCode(Binary[i] & 0xF);
	}

	*HexLen = iHexLen;

	return 0;
}


char *fb_get_error(void)
{
	return mError;
}

UINT32 vds_CRC32(const UINT8 *pvData, INT32 iLen)
{
	UINT32 iVal = 0;
	const UINT8 *p = pvData;

	while (--iLen >= 0)
	{
		iVal = vds_crc32_table[(iVal ^ *p++) & 0xff] ^ (iVal >> 8);
	}

	return iVal;
}


static void Bin2Hex(BYTE *Binary, DWORD BinLen, BYTE *Hex, DWORD *HexLen)
{
	const BYTE HexLUT[] = { '0', '1', '2', '3', '4',  '5',  '6',  '7',  '8',  '9',  'A',  'B',  'C',  'D',  'E',  'F' };
	DWORD j = 0;

	for (DWORD i = 0; i < BinLen; i++)
	{
		Hex[j++] = HexLUT[Binary[i] >> 4];
		Hex[j++] = HexLUT[Binary[i] & 0xF];
	}

	*HexLen = j;
}

static UINT32 escape(UINT8 *sourceBuf, UINT8 *destBuf, UINT32 iLen)
{
	UINT8 *src_ptr = sourceBuf;
	UINT32 curr, RawDataLen = 0;

	for (curr = 0; curr < iLen; curr++)
	{
		switch (*(src_ptr + curr))
		{
		case HDLC_FLAG:
		case HDLC_ESCAPE:
			destBuf[RawDataLen++] = HDLC_ESCAPE;
			destBuf[RawDataLen++] = *(src_ptr + curr) ^ HDLC_ESCAPE_MASK;
			break;
		default:
			destBuf[RawDataLen++] = *(src_ptr + curr);
			break;
		}
	}
	return RawDataLen;

}
static UINT32 deEscape(UINT8 *sourceBuf, UINT8 *destBuf, UINT32 iLen)
{
	unsigned int curr = 0;
	unsigned int dest_len = 0;

	for (curr = 0; curr < iLen; curr++)
	{
		switch (*(sourceBuf + curr))
		{
		case HDLC_ESCAPE:
			curr++;
			*(destBuf + dest_len) = *(sourceBuf + curr) ^ HDLC_ESCAPE_MASK;
			break;
		default:
			*(destBuf + dest_len) = *(sourceBuf + curr);
			break;
		}

		dest_len = dest_len + 1;
	}

	return dest_len;
}
#if 1
static int dataSign(const void *data, unsigned int size, BYTE* signData)
{
	
	if (g_bISRSA)
	{
		BYTE Sha2Hex[128] = { 0 };
		DWORD ReadWriteBytes = 0;
		UINT8 sha2OutBuf[32] = { 0 };
		BYTE ShaHexEncrpted[512] = { 0 };
		rsa_context rsa;
		sha2_context ctx;

		sha2_starts(&ctx, false);
		sha2_update(&ctx, (BYTE*)data, size);
		sha2_finish(&ctx, sha2OutBuf);

		//printf("\r\n");
		Bin2Hex(sha2OutBuf, 32, Sha2Hex, &ReadWriteBytes);
		//printf("%s\r\n", Sha2Hex);


		memset(&rsa, 0, sizeof(rsa_context));
		rsa.len = KEY_LEN;
		mpi_read_string(&rsa.N, 16, RSA_N);
		mpi_read_string(&rsa.E, 16, RSA_E);
		mpi_read_string(&rsa.D, 16, RSA_D);
		mpi_read_string(&rsa.P, 16, RSA_P);
		mpi_read_string(&rsa.Q, 16, RSA_Q);
		mpi_read_string(&rsa.DP, 16, RSA_DP);
		mpi_read_string(&rsa.DQ, 16, RSA_DQ);
		mpi_read_string(&rsa.QP, 16, RSA_QP);

		if (rsa_check_pubkey(&rsa) != 0 ||
			rsa_check_privkey(&rsa) != 0)
		{
			printf("RSA Key Check Failed\r\n");
			return -1;
		}

		if (rsa_pkcs1_encrypt(&rsa, RSA_PRIVATE, ReadWriteBytes, Sha2Hex, ShaHexEncrpted))
		{
			printf("RSA Encrypt Failed\r\n");
			return -1;
		}
		memcpy(signData, ShaHexEncrpted, KEY_LEN);
	}
	else
	{
		memset(signData, 0, 256);
		RDA_SIGNATRUE signature = { 0 };
		dll_sign_func(&keyPair.enc_key, (char *)data, size, &signature);
		memcpy(signData, &signature, sizeof(RDA_SIGNATRUE));
	}
		
	return 0;

}
#endif
BOOL saveImg(const char *fn, UINT8* data, int fileSize)
{

	FILE * fp = NULL;
	
	fp = fopen(fn, "wb");
	if (fp == NULL)
	{
		printf("File open error!\r\n");
		return FALSE;
	}
	fwrite(data, 1, fileSize, fp);
	
	fclose(fp);
	return TRUE;
}
static int check_response(ICommChannel* m_pProChan, unsigned int *size,
	BOOL bShowInfo, char *response, BOOL printfBin)
{
	MC_FASTBOOT_RSP_HEADER rData = { 0 };
	UINT8 iData = 0;
	UINT32 iLen = 0, oLen = 0;
	DWORD t = 0;
	//memcpy(readBuf, tmp, iLen);

	memset(readBuf, 0, BUFF_LENGTH*4);
	memset(tmp, 0, BUFF_LENGTH*4);
	t = GetTickCount();
	do {
		if (GetTickCount() - t > timeOut)
		{
			sprintf(mError, "Time out");
			return -1;
		}
		iLen = ReadDataFromPort(&iData, sizeof(UINT8), m_pProChan);

	} while (iData != 0x7E);
	iLen = 0;
	t = GetTickCount();
	do {

		if (GetTickCount() - t > (3*1000))
		{
			sprintf(mError, "Time out");
			return -1;
		}
		if (ReadDataFromPort(&iData, sizeof(char), m_pProChan))
		{
			tmp[iLen] = iData;
			iLen += 1;
		}
		else
		{
			iData = 0;
		}

	} while (iData != 0x7E);
	iLen--;

	oLen = deEscape(tmp, readBuf, iLen);

	memcpy(&rData, readBuf, sizeof(MC_FASTBOOT_RSP_HEADER));
	if (rData.result == 0)
	{
		sprintf(mError, "remote: %s", readBuf + sizeof(MC_FASTBOOT_RSP_HEADER));
		return -1;
	}
	else
	{
		if (rData.len !=0 && bShowInfo)
		{
			if (memcmp(readBuf + sizeof(MC_FASTBOOT_RSP_HEADER), "finish", strlen("finish")) != 0)
			{
				if (printfBin)
				{
					UINT8 Binary[1024] = {0};
					//UINT8 Hex[1280] = {0};
					//UINT32 HexLen = 0;
					memcpy(Binary, readBuf + sizeof(MC_FASTBOOT_RSP_HEADER), rData.len);
					printf("\r\n");
					//Binary2Hex(Binary, rData.len, Hex, &HexLen);
					for (int i=0;i<rData.len;i++)
					{
						printf("%02X", Binary[i]);
					}
					printf("\r\n\r\n");
				}
				else
				{
					printf("\r\n%s\r\n\r\n", readBuf + sizeof(MC_FASTBOOT_RSP_HEADER));
				}
				
			}

			
		}
		if (size != NULL)
		{
			*size = rData.len;
		}
		if (response != NULL)
		{
			memcpy(response, readBuf + sizeof(MC_FASTBOOT_RSP_HEADER), rData.len);
		}
		
	}

	return 0;
}
static int _file_command_send(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, char *response)
{
	BYTE signData[256] = { 0 };
	int oLen = 0;
	UINT32 crc = 0;
	MC_FASTBOOT_CMD_HEADER sData = { 0 };
	UINT8 bufTmp[BUFF_LENGTH*2] = { 0 };
	MC_PUSH_IMG_INFO imgInfo = { 0 };//文件系统相关命令 信息。包含路径，大小等
//	BYTE signData[256] = { 0 };
	char res[256] = { 0 };

	if (size > (BUFF_LENGTH*2 - sizeof(MC_FASTBOOT_CMD_HEADER) - sizeof(MC_PUSH_IMG_INFO)))
	{
		sprintf(mError, "data too long. size (%d)", size);
		return -1;
	}

	if (cmd == MC_FILESYSTE_INIT || cmd == MC_FILESYSTE_INIT_PLUS)
	{
		timeOut = 50 * 1000;
	}
	
	sData.magicNumber = 0x5a5aa5a5;
	sData.cmd = cmd;
	//strcpy((char*)&(imgInfo.info), (char*)data);
	//printf("%s", data);
	memcpy(&imgInfo, data, sizeof(MC_PUSH_IMG_INFO));
		
	sData.length = sizeof(MC_PUSH_IMG_INFO) + sizeof(MC_FASTBOOT_CMD_HEADER);
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgInfo, sizeof(MC_PUSH_IMG_INFO));
	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}

		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}

	crc = vds_CRC32(bufTmp, sData.length);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));
	if (oLen > (BUFF_LENGTH*4 - 2))
	{
		sprintf(mError, "data too long. oLen (%d)", oLen);
		return -1;
	}

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;

	WriteDataToPort(databuf, oLen + 2, m_pProChan);

	if (cmd == MC_FILESYSTE_DIR_PATH)
	{
		while (1)
		{
			if (check_response(m_pProChan, 0, TRUE, res, FALSE) < 0)//列出所有文件文件夹信息
			{
				return -1;
			}

			if (memcmp(res, "finish", strlen("finish")) == 0)
			{
				printf("\b\b\b\b\b\b");
				break;
			}

		}

	}
	else
	{
		if (cmd == MC_FILESYSTE_FORMAT)
		{
			timeOut = 30 * 1000;
			if (check_response(m_pProChan, 0, TRUE, res, FALSE) == 0)
			{
				if (strlen(res) == 0)
				{
					return 0;
				}
				if (strcmp(res, "Equal size") == 0)
				{
					timeOut = 30 * 1000;
				}
				else
				{
					timeOut = 60 * 1000;
				}
			}
			else
			{
				return -1;
			}
			
		}

		return check_response(m_pProChan, 0, TRUE, res, FALSE);
	}
	
	return 0;
}
static int _nvEdiet_command_send(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *info, const void *data, unsigned int size, char *response)
{
	BYTE signData[256] = { 0 };
	UINT32 crc = 0;
	int oLen = 0;
	MC_FASTBOOT_CMD_HEADER sData = { 0 };
	UINT8 bufTmp[BUFF_LENGTH*2] = { 0 };
	MC_NV_EDITE_INFO nvInfo = {0};

	nvInfo.itemID = atoi((char*)info);
	nvInfo.len = size;

	sData.magicNumber = 0x5a5aa5a5;
	sData.cmd = cmd;
	if (sData.cmd == MC_READ_NV)
	{
		sData.length = sizeof(MC_NV_EDITE_INFO) + sizeof(MC_FASTBOOT_CMD_HEADER);
		memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &nvInfo, sizeof(MC_NV_EDITE_INFO));
		memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
		if (!g_bISRSA)
		{
			srand(time(NULL));
			int num[RANDCOUNT] = { 0 };
			for (int i = 0; i<RANDCOUNT; i++)
			{
				num[i] = rand();
				memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
			}

			dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
			memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
		}
		else
		{
			dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
			memcpy(bufTmp + sizeof(UINT32), signData, 256);
		}
		

		crc = vds_CRC32(bufTmp, sData.length);
		memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
		oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));
		//printf("oLen %d\r\n", oLen);
		if (oLen > (BUFF_LENGTH*4 - 2))
		{
			sprintf(mError, "data too long. oLen (%d)", oLen);
			return -1;
		}

		databuf[0] = 0x7E;
		memcpy(&databuf[1], tmp, oLen);
		databuf[oLen + 1] = 0x7E;

		WriteDataToPort(databuf, oLen + 2, m_pProChan);

		UINT8 iData = 0;
		UINT32 iLen = 0, oLen = 0;
		DWORD t = 0;
		MC_FASTBOOT_RSP_HEADER rData = { 0 };

		unsigned char *tmpBuf = NULL;
		tmpBuf = (unsigned char *)malloc(300*1024);
		if (tmpBuf == NULL)
		{
			sprintf(mError, "malloc tmpbuf fail");
			return -1;
		}
		memset(tmpBuf, 0, 300 * 1024);
		t = GetTickCount();
		do {
			if (GetTickCount() - t > 5 * 1000)
			{
				sprintf(mError, "Time out");
				if (tmpBuf != NULL)
				{
					free(tmpBuf);
				}
				return -1;
			}
			iLen = ReadDataFromPort(&iData, sizeof(UINT8), m_pProChan);

		} while (iData != 0x7E);
		iLen = 0;
		do {

			if (ReadDataFromPort(&iData, sizeof(char), m_pProChan))
			{
				tmpBuf[iLen] = iData;
				iLen += 1;
			}
			else
			{
				iData = 0;
			}

		} while (iData != 0x7E);
		iLen--;

		unsigned char *sendbuf = NULL;
		sendbuf = (unsigned char *)malloc(300 * 1024);
		if (sendbuf == NULL)
		{
			sprintf(mError, "malloc tmpbuf fail");
			return -1;
		}
		memset(sendbuf, 0, 300 * 1024);

		oLen = deEscape(tmpBuf, sendbuf, iLen);
		if (tmpBuf != NULL)
		{
			free(tmpBuf);
			tmpBuf = NULL;
		}

		memcpy(&rData, sendbuf, sizeof(MC_FASTBOOT_RSP_HEADER));
		if (rData.result == 0)
		{
			sprintf(mError, "remote: %s", sendbuf + sizeof(MC_FASTBOOT_RSP_HEADER));
			return -1;
		}
		else
		{
			for (int i=0;i<rData.len; i++)
			{
				if (i != 0 && i%8==0)
				{
					printf("\r\n");
				}
				printf("%02X ", sendbuf[sizeof(MC_FASTBOOT_RSP_HEADER)+i]);
			}
			printf("\r\n");
			if (sendbuf != NULL)
			{
				free(sendbuf);
				sendbuf = NULL;
			}
			return 0;
		}

	}
	else
	{
		sData.length = sizeof(MC_NV_EDITE_INFO) + sizeof(MC_FASTBOOT_CMD_HEADER)+size;
	}


}
static int _command_send(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, char *response)
{
	UINT32 crc = 0;
	int oLen = 0;
	MC_FASTBOOT_CMD_HEADER sData = { 0 };	//数据头，包含命令，签名等信息。
	UINT8 bufTmp[BUFF_LENGTH*2] = { 0 };
	BYTE signData[256] = {0};
	unsigned char strInfo[256] = { 0 };

	if (size > (BUFF_LENGTH*2 - sizeof(MC_FASTBOOT_CMD_HEADER)))
	{
		sprintf(mError, "data too long. size (%d)", size);
		return -1;
	}

	sData.magicNumber = 0x5a5aa5a5;	
	sData.cmd = cmd;
	
	sData.length = size + sizeof(MC_FASTBOOT_CMD_HEADER);
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), data, size);
	
	
	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}

		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}
	

	crc = vds_CRC32(bufTmp, sData.length);
	//printf("\r\n crc:0x%08X \r\n", crc);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));//0x7E 转义

//	oLen = escape(bufTmp, tmp, sData.length);
	if (oLen > (BUFF_LENGTH*4 - 2))
	{
		sprintf(mError, "data too long. oLen (%d)", oLen);
		return -1;
	}

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;

	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (cmd == MC_READ_APPINFO)
	{
		return check_response(m_pProChan, 0, TRUE, NULL, TRUE);
	}
	else
		return check_response(m_pProChan, 0, TRUE, NULL, FALSE);
}
static int _writeSign_data(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, char *type, char *response)
{
	UINT32 crc = 0;
	int oLen = 0;
	MC_FASTBOOT_CMD_HEADER sData = { 0 };		//数据头，包含命令，签名等信息。
	MC_DOWNLOAD_IMG_BLOCK imgBlock = { 0 };	

	UINT8 *dataTmp = (UINT8 *)data;
	UINT8 imgBuf[BUFF_LENGTH * 2] = { 0 };
	UINT8 bufTmp[BUFF_LENGTH * 2] = { 0 };
	BYTE signData[256] = { 0 };
	char fileName[32] = { 0 };

	sData.magicNumber = 0x5a5aa5a5;
	sData.cmd = cmd;

	if (strcmp(type, "boot") == 0)
	{
		imgBlock.addr = 0x89ABCDEF;	//指明要烧写bootloader的签名数据及公钥。
		imgBlock.len = sizeof(RDA_PUBLIC_KEY) + sizeof(RDA_SIGNATRUE);		//待写入数据长度
	}
	else if (strcmp(type, "ap") == 0)
	{
		imgBlock.addr = 0xEFCDAB89;	//指明要烧写ap的签名数据。
		if(g_bISRSA)
			imgBlock.len = 256;
		else
			imgBlock.len = sizeof(RDA_SIGNATRUE);		//待写入数据长度	
	}
	else if (strcmp(type, "ft") == 0)
	{
		imgBlock.addr = 0xEFCDAB99;	//指明要烧写fastboot的签名数据。
		if (g_bISRSA)
			imgBlock.len = 256;
		else
			imgBlock.len = sizeof(RDA_SIGNATRUE);		//待写入数据长度	
	}
	else
	{
		sprintf(mError, "Param error: %s", type);
		return -1;
	}
	printf("imgBlock.len %d\r\n", imgBlock.len);
	if(g_bISRSA)
		memcpy(imgBuf, dataTmp + (size - 512 - sizeof(RDA_SIGNATRUE)), imgBlock.len);
	else
		memcpy(imgBuf, dataTmp + (size - 512 - imgBlock.len), imgBlock.len);
	sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_DOWNLOAD_IMG_BLOCK) + imgBlock.len;

	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgBlock, sizeof(MC_DOWNLOAD_IMG_BLOCK));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_DOWNLOAD_IMG_BLOCK), imgBuf, imgBlock.len);
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}

		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(RDA_SIGNATRUE) - sizeof(UINT32), signData);
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - 256 - sizeof(UINT32), signData);
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}


	crc = vds_CRC32(bufTmp, sData.length);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;

	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (check_response(m_pProChan, 0, FALSE, NULL, FALSE) != 0)
	{
		return -1;
	}

	
	return 0;
}
static int _download_logo_img(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, char *response)
{
	UINT32 crc = 0;
	int oLen = 0;
	MC_FASTBOOT_CMD_HEADER sData = { 0 };		//数据头，包含命令，签名等信息。
	MC_DOWNLOAD_IMG_BLOCK imgBlock = { 0 };		//烧写app信息，包含板端地址，数据大小等
	sData.magicNumber = 0x5a5aa5a5;
	sData.cmd = cmd;

	
	UINT8 imgBuf[BUFF_LENGTH * 2] = { 0 };
	UINT8 bufTmp[BUFF_LENGTH * 2] = { 0 };
	BYTE signData[256] = { 0 };
	UINT32 offset = 0;
	UINT32 imgSize = size, dsize = 0, msendSize = 1024 + 512, count = 0;

	imgBlock.addr = 0x89ABCDEF;	
	imgBlock.len = size;		
	sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_DOWNLOAD_IMG_BLOCK);

	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgBlock, sizeof(MC_DOWNLOAD_IMG_BLOCK));
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}

		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(RDA_SIGNATRUE) - sizeof(UINT32), signData);	//签名
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - 256 - sizeof(UINT32), signData);	//签名
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}
	


	crc = vds_CRC32(bufTmp, sData.length);
	//printf("\r\n crc:0x%08X \r\n", crc);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;
	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (check_response(m_pProChan, 0, FALSE, NULL, FALSE) != 0)
		return -1;

	int l_nRealTimeStep = 0;
	while (imgSize > 0)
	{
		dsize = (imgSize > msendSize) ? msendSize : imgSize;
		memcpy(imgBuf, (UINT8*)data + offset, dsize);

		imgBlock.addr = 0; 
		imgBlock.len = dsize;			   

		sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_DOWNLOAD_IMG_BLOCK) + imgBlock.len;

		memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
		memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgBlock, sizeof(MC_DOWNLOAD_IMG_BLOCK));
		memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_DOWNLOAD_IMG_BLOCK), imgBuf, imgBlock.len);
		if (!g_bISRSA)
		{
			srand(time(NULL));
			int num[RANDCOUNT] = { 0 };
			for (int i = 0; i<RANDCOUNT; i++)
			{
				num[i] = rand();
				memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
			}
			dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(RDA_SIGNATRUE) - sizeof(UINT32), signData);
			memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
		}
		else {
			dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - 256 - sizeof(UINT32), signData);
			memcpy(bufTmp + sizeof(UINT32), signData, 256);
		}
		

		crc = vds_CRC32(bufTmp, sData.length);
		//printf("\r\n crc:0x%08X \r\n", crc);
		memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
		oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

		databuf[0] = 0x7E;
		memcpy(&databuf[1], tmp, oLen);
		databuf[oLen + 1] = 0x7E;

		WriteDataToPort(databuf, oLen + 2, m_pProChan);

		if (check_response(m_pProChan, 0, FALSE, NULL, FALSE) == 0)
		{
			offset += dsize;
			imgSize -= dsize;
			count += dsize;

			l_nRealTimeStep = 100 * (double)count / (double)size;
			printf("\b\b\b\b%3d%%", l_nRealTimeStep);

		}
		else
		{
			return -1;
		}
	}
	printf("\r\n");
	printf("write img to flash...\r\n");

	imgBlock.addr = 0xEFCDAB89;//指明app下载完成
	imgBlock.len = 0;
	sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_DOWNLOAD_IMG_BLOCK);

	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgBlock, sizeof(MC_DOWNLOAD_IMG_BLOCK));

	if (!g_bISRSA)
	{
		int num[RANDCOUNT] = { 0 };
		srand(time(NULL));

		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}
		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(RDA_SIGNATRUE) - sizeof(UINT32), signData);
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - 256 - sizeof(UINT32), signData);
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}
	

	crc = vds_CRC32(bufTmp, sData.length);
	//printf("\r\n crc:0x%08X \r\n", crc);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;

	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (check_response(m_pProChan, 0, FALSE, NULL, FALSE) != 0)
	{
		return -1;
	}

	return 0;
}
static int _download_img(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, char *response)
{
	UINT32 crc = 0;
	int oLen = 0;
	MC_FASTBOOT_CMD_HEADER sData = { 0 };		//数据头，包含命令，签名等信息。
	MC_DOWNLOAD_IMG_BLOCK imgBlock = { 0 };		//烧写app信息，包含板端地址，数据大小等
	
	UINT8 imgBuf[BUFF_LENGTH*2] = { 0 };
	UINT8 bufTmp[BUFF_LENGTH*2] = { 0 };
	BYTE signData[256] = { 0 };
	UINT32 offset = 0;
	UINT32 imgSize = size, dsize = 0, msendSize = 1024+512 ,count = 0;

	sData.magicNumber = 0x5a5aa5a5;
	sData.cmd = cmd;
	
	timeOut = 10 * 1000;

	imgBlock.addr = 0x89ABCDEF;	//指明要下载app
	imgBlock.len = size;		//App总长度
	sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_DOWNLOAD_IMG_BLOCK);

	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgBlock, sizeof(MC_DOWNLOAD_IMG_BLOCK));
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}

		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(RDA_SIGNATRUE) - sizeof(UINT32), signData);	//签名
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - 256 - sizeof(UINT32), signData);	//签名
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}
	


	crc = vds_CRC32(bufTmp, sData.length);
	//printf("\r\n crc:0x%08X \r\n", crc);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));
	
	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;
	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (check_response(m_pProChan, 0, FALSE, NULL, FALSE) != 0)
	{
		//return -1;
		if (strstr(mError, "APP is Running") != NULL)
		{
			int len = 0;
			//printf("APP is Running,set dont auto run and reset\r\n");
			sprintf(mError, "APP is Running,set dont auto run and reset");
			sData.magicNumber = 0x5a5aa5a5;
			sData.cmd = MC_AUTO_RUN_APP;

			len = strlen("0");

			sData.length = len + sizeof(MC_FASTBOOT_CMD_HEADER);
			memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), "0", len);


			memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
			if (!g_bISRSA)
			{
				srand(time(NULL));
				int num[RANDCOUNT] = { 0 };
				for (int i = 0; i<RANDCOUNT; i++)
				{
					num[i] = rand();
					memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
				}

				dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
				memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
			}
			else {
				dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
				memcpy(bufTmp + sizeof(UINT32), signData, 256);
			}

			crc = vds_CRC32(bufTmp, sData.length);
			//printf("\r\n crc:0x%08X \r\n", crc);
			memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
			oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));//0x7E 转义

																	  //	oLen = escape(bufTmp, tmp, sData.length);
			if (oLen > (BUFF_LENGTH * 4 - 2))
			{
				sprintf(mError, "data too long. oLen (%d)", oLen);
				return -1;
			}

			databuf[0] = 0x7E;
			memcpy(&databuf[1], tmp, oLen);
			databuf[oLen + 1] = 0x7E;

			WriteDataToPort(databuf, oLen + 2, m_pProChan);
			if (check_response(m_pProChan, 0, TRUE, NULL, FALSE) == 0)
			{
				sData.magicNumber = 0x5a5aa5a5;
				sData.cmd = MC_RESET_SYSTEM;
				
				sData.length = 0 + sizeof(MC_FASTBOOT_CMD_HEADER);
				memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), data, 0);

				memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
				if (!g_bISRSA)
				{
					srand(time(NULL));
					int num[RANDCOUNT] = { 0 };
					for (int i = 0; i<RANDCOUNT; i++)
					{
						num[i] = rand();
						memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
					}

					dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
					memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
				}
				else
				{
					dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
					memcpy(bufTmp + sizeof(UINT32), signData, 256);
				}
				

				crc = vds_CRC32(bufTmp, sData.length);
				//printf("\r\n crc:0x%08X \r\n", crc);
				memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
				oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));//0x7E 转义

				if (oLen > (BUFF_LENGTH * 4 - 2))
				{
					sprintf(mError, "data too long. oLen (%d)", oLen);
					return -1;
				}

				databuf[0] = 0x7E;
				memcpy(&databuf[1], tmp, oLen);
				databuf[oLen + 1] = 0x7E;

				WriteDataToPort(databuf, oLen + 2, m_pProChan);
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	MERCURY_HEADER mercuryAppHeader = { 0 };

	memcpy(&mercuryAppHeader, data, sizeof(MERCURY_HEADER));

	int l_nRealTimeStep = 0;
	while (imgSize > 0)
	{
		dsize = (imgSize > msendSize) ? msendSize : imgSize;
		memcpy(imgBuf, (UINT8*)data+offset, dsize);

		imgBlock.addr = mercuryAppHeader.LoadAddress + offset; //App分包写入到flash的地址
		imgBlock.len = dsize;			   //App分包大小

		sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_DOWNLOAD_IMG_BLOCK) + imgBlock.len;

		memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
		memcpy(bufTmp+ sizeof(MC_FASTBOOT_CMD_HEADER), &imgBlock, sizeof(MC_DOWNLOAD_IMG_BLOCK));
		memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER)+ sizeof(MC_DOWNLOAD_IMG_BLOCK), imgBuf, imgBlock.len);
		if (!g_bISRSA)
		{
			srand(time(NULL));
			int num[RANDCOUNT] = { 0 };
			for (int i = 0; i<RANDCOUNT; i++)
			{
				num[i] = rand();
				memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
			}
			dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(RDA_SIGNATRUE) - sizeof(UINT32), signData);
			memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
		}
		else
		{
			dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - 256 - sizeof(UINT32), signData);
			memcpy(bufTmp + sizeof(UINT32), signData, 256);
		}
		

		crc = vds_CRC32(bufTmp, sData.length);
		//printf("\r\n crc:0x%08X \r\n", crc);
		memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
		oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));
		
		databuf[0] = 0x7E;
		memcpy(&databuf[1], tmp, oLen);
		databuf[oLen + 1] = 0x7E;

		WriteDataToPort(databuf, oLen + 2, m_pProChan);

		if (check_response(m_pProChan, 0, FALSE, NULL, FALSE) == 0)
		{
			offset += dsize;
			imgSize -= dsize;
			count += dsize;

			l_nRealTimeStep = 100 * (double)count / (double)size;
			printf("\b\b\b\b%3d%%", l_nRealTimeStep);

		}
		else
		{
			return -1;
		}
	}
	printf("\r\n");
	printf("write img to flash...\r\n");

	imgBlock.addr = 0xEFCDAB89;//指明app下载完成
	imgBlock.len = 0;
	sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_DOWNLOAD_IMG_BLOCK);

	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgBlock, sizeof(MC_DOWNLOAD_IMG_BLOCK));
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}
		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(RDA_SIGNATRUE) - sizeof(UINT32), signData);
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - 256 - sizeof(UINT32), signData);
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}
	

	crc = vds_CRC32(bufTmp, sData.length);
	//printf("\r\n crc:0x%08X \r\n", crc);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;

	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (check_response(m_pProChan, 0, FALSE, NULL, FALSE) != 0)
	{
		return -1;
	}

	return 0;
}
static int _push_img(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, char *path)
{
	BYTE signData[256] = { 0 };
	UINT32 crc = 0;
	int oLen = 0;
	MC_FASTBOOT_CMD_HEADER sData = { 0 };//数据头，包含命令，签名等信息。
	MC_PUSH_IMG_INFO imgInfo = {0};			//待下载数据信息，包含 路径，长度信息
	UINT8 bufTmp[BUFF_LENGTH*2] = { 0 };
	char response[256] = {0};
	UINT8 imgBuf[BUFF_LENGTH * 2] = { 0 };
	UINT32 imgSize = size, dsize = 0, msendSize = 1024, count = 0;
	UINT32 offset = 0;

	sData.magicNumber = 0x5a5aa5a5;
	sData.cmd = cmd;


	memcpy(&(imgInfo.info), path, strlen(path));		//指明Push进文件系统的路径
	imgInfo.len = size;									//文件大小
	sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_PUSH_IMG_INFO);

	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgInfo, sizeof(MC_PUSH_IMG_INFO));
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}
		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}
	

	crc = vds_CRC32(bufTmp, sData.length);
	//printf("\r\n crc:0x%08X \r\n", crc);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;

	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (check_response(m_pProChan, 0, FALSE, response, FALSE) != 0)
	{
		return -1;
	}
	//返回 文件操作句柄
	CString strTmp(response);
	CString strHandle;
	int fileHandle = 0;

	int index = strTmp.Find(_T(":"));
	strHandle = strTmp.Right(strTmp.GetLength()-index-1);
	fileHandle = _ttoi(strHandle);
	printf("%d\r\n", fileHandle);

	int l_nRealTimeStep = 0;
	while (imgSize > 0)
	{
		dsize = (imgSize > msendSize) ? msendSize : imgSize;

		memset(&imgInfo, 0, sizeof(MC_PUSH_IMG_INFO));
		sprintf((char*)&(imgInfo.info), "handle:%d", fileHandle);	//指明要写入的文件句柄
		imgInfo.len = dsize;										//文件大小

		memcpy(imgBuf, (UINT8*)data + offset, dsize);

		sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_PUSH_IMG_INFO) + imgInfo.len;

		memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
		memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgInfo, sizeof(MC_PUSH_IMG_INFO));
		memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_PUSH_IMG_INFO), imgBuf, imgInfo.len);
		if (!g_bISRSA)
		{
			srand(time(NULL));
			int num[RANDCOUNT] = { 0 };
			for (int i = 0; i<RANDCOUNT; i++)
			{
				num[i] = rand();
				memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
			}
			dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
			memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));

		}
		else
		{
			dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
			memcpy(bufTmp + sizeof(UINT32), signData, 256);
		}
		
		crc = vds_CRC32(bufTmp, sData.length);
		//printf("\r\n crc:0x%08X \r\n", crc);
		memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
		oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

		databuf[0] = 0x7E;
		memcpy(&databuf[1], tmp, oLen);
		databuf[oLen + 1] = 0x7E;

		WriteDataToPort(databuf, oLen + 2, m_pProChan);

		if (check_response(m_pProChan, 0, FALSE, NULL, FALSE) == 0)
		{
			offset += dsize;
			imgSize -= dsize;
			count += dsize;

			l_nRealTimeStep = 100 * (double)count / (double)size;
			printf("\b\b\b\b%3d%%", l_nRealTimeStep);
			//printf("%d\r\n", imgSize);

		}
		else
		{
			return -1;
		}

	}
	printf("\r\n");

	memset(&imgInfo, 0, sizeof(MC_PUSH_IMG_INFO));
	sprintf((char*)&(imgInfo.info), "finish.handle:%d", fileHandle);//指明文件Push完成。 板端关闭对应句柄。
	imgInfo.len = 0;
	sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_PUSH_IMG_INFO);

	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgInfo, sizeof(MC_PUSH_IMG_INFO));
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}
		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}
	

	crc = vds_CRC32(bufTmp, sData.length);
	//printf("\r\n crc:0x%08X \r\n", crc);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;

	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (check_response(m_pProChan, 0, FALSE, NULL, FALSE) != 0)
	{
		return -1;
	}

	return 0;
}
static int _pull_img(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, char *path)
{
	BYTE signData[256] = { 0 };
	UINT32 crc = 0;
	int oLen = 0;
	MC_FASTBOOT_CMD_HEADER sData = { 0 };
	MC_PUSH_IMG_INFO imgInfo = { 0 };
	UINT8 bufTmp[BUFF_LENGTH] = { 0 };
	char response[256] = { 0 };
	unsigned int readSize = 0;

	sData.magicNumber = 0x5a5aa5a5;
	sData.cmd = cmd;

	memcpy(&(imgInfo.info), path, strlen(path));	//指明要读取的文件
	imgInfo.len = 0;
	sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + sizeof(MC_PUSH_IMG_INFO);

	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), &imgInfo, sizeof(MC_PUSH_IMG_INFO));
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}

		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}
	
	crc = vds_CRC32(bufTmp, sData.length);
	//printf("\r\n crc:0x%08X \r\n", crc);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;

	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (check_response(m_pProChan, &readSize, FALSE, response, FALSE) != 0)
	{
		return -1;
	}
	else
	{
		UINT8 *fileData = NULL;

		CString tmp(response);		//板端该返回文件大小
		int index = 0;
		index = tmp.Find(_T(":"));
		CString strFileSize;
		strFileSize = tmp.Right(tmp.GetLength()-index-1);
		
		int fileSize = _ttoi(strFileSize);

		printf("fileSize:%d\r\n", fileSize);
		fileData = (UINT8*)malloc(fileSize);
		if (fileData == NULL)
		{
			sprintf(mError, "Malloc Failed.");
			return -1;
		}
		int offset = 0;
		int l_nRealTimeStep = 0;
		//开始接收文件
		UINT8 tmpData = 0x7E;
		while (1)
		{
			readSize = ReadDataFromPort2(fileData + offset, 1024, m_pProChan, 1000);
			//printf("%d\r\n", readSize);
			//check_response(m_pProChan, &readSize, FALSE, response);
			//memcpy(fileData+offset, response, readSize);

			offset += readSize;

			l_nRealTimeStep = 100 * (double)offset / (double)fileSize;
			printf("\b\b\b\b%3d%%", l_nRealTimeStep);

			if (offset == fileSize)
			{
				break;
			}
			
		}
		printf("\r\n");
		//文件接收完成，保存到本地路径
		if (saveImg((char*)data, fileData, fileSize))
		{
			free(fileData);
			return 0;
		}
		else
		{
			free(fileData);
			sprintf(mError, "Save the file Failed.");
			return -1;
		}
		
	}


}
static int _read_logo_img(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, char *path)
{
	UINT32 readLen = 0;
	UINT32 allSize = 0;
	BYTE signData[256] = { 0 };
	UINT32 crc = 0;
	int oLen = 0;
	MC_FASTBOOT_CMD_HEADER sData = { 0 };
	
	UINT8 bufTmp[BUFF_LENGTH] = { 0 };
	char response[256] = { 0 };
	unsigned int readSize = 0;

	sData.magicNumber = 0x5a5aa5a5;
	sData.cmd = cmd;

	sData.length = sizeof(MC_FASTBOOT_CMD_HEADER) + size;

	memcpy(bufTmp, &sData, sizeof(MC_FASTBOOT_CMD_HEADER));
	memcpy(bufTmp + sizeof(MC_FASTBOOT_CMD_HEADER), data, size);
	if (!g_bISRSA)
	{
		srand(time(NULL));
		int num[RANDCOUNT] = { 0 };
		for (int i = 0; i<RANDCOUNT; i++)
		{
			num[i] = rand();
			memcpy(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE) + i * sizeof(int), &num[i], sizeof(int));
		}

		dataSign(bufTmp + sizeof(UINT32) + sizeof(RDA_SIGNATRUE), sData.length - sizeof(UINT32) - sizeof(RDA_SIGNATRUE), signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, sizeof(RDA_SIGNATRUE));
	}
	else
	{
		dataSign(bufTmp + sizeof(UINT32) + 256, sData.length - sizeof(UINT32) - 256, signData);//对命令签名
		memcpy(bufTmp + sizeof(UINT32), signData, 256);
	}
	
	crc = vds_CRC32(bufTmp, sData.length);
	//printf("\r\n crc:0x%08X \r\n", crc);
	memcpy(bufTmp + sData.length, &crc, sizeof(UINT32));
	oLen = escape(bufTmp, tmp, sData.length + sizeof(UINT32));

	databuf[0] = 0x7E;
	memcpy(&databuf[1], tmp, oLen);
	databuf[oLen + 1] = 0x7E;

	WriteDataToPort(databuf, oLen + 2, m_pProChan);
	if (check_response(m_pProChan, &readSize, FALSE, response, FALSE) != 0)
	{
		return -1;
	}
	else
	{
		UINT8 *fileData = NULL;

		CString tmp(response);		//板端该返回文件大小
		int index = 0;
		index = tmp.Find(_T(":"));
		CString strFileSize;
		strFileSize = tmp.Right(tmp.GetLength() - index - 1);

		int fileSize = _ttoi(strFileSize);

		printf("fileSize:%d\r\n", fileSize);

		allSize = fileSize;
		fileData = (UINT8*)malloc(fileSize);
		if (fileData == NULL)
		{
			sprintf(mError, "Malloc Failed.");
			return -1;
		}
		int offset = 0;
		int l_nRealTimeStep = 0;
		Sleep(100);
		while (allSize)
		{
			
			readLen = (allSize > 1024) ? 1024 : allSize;
			readSize = 0;
			do
			{
				readSize += ReadDataFromPort2(fileData + offset + readSize, readLen - readSize, m_pProChan, 10);
				//printf("%d\r\n", readSize);
				
			} while (readSize < readLen);
			
					
			offset += readLen;
			allSize -= readLen;
			

			l_nRealTimeStep = 100 * (double)offset / (double)fileSize;
			printf("\b\b\b\b%3d%%", l_nRealTimeStep);

			if (offset == fileSize)
			{
				break;
			}
			
		}
		printf("\r\n");
		//文件接收完成，保存到本地路径
		if (saveImg(path, fileData, fileSize))
		{
			free(fileData);
			return 0;
		}
		else
		{
			free(fileData);
			sprintf(mError, "Save the file Failed.");
			return -1;
		}

	}


}
int fb_command(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size)
{
	return _command_send(m_pProChan, cmd, data, size, 0);
}
int fb_downlad(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size)
{
	return _download_img(m_pProChan, cmd, data, size, 0);
}
int fb_push(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const char *path, const void *data, unsigned int size)
{
	return _push_img(m_pProChan, cmd, data, size, (char*)path);
}
int fb_pull(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const char *path, const void *data, unsigned int size)
{
	return _pull_img(m_pProChan, cmd, data, size, (char*)path);

}

int fb_fileSys(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size)
{
	return _file_command_send(m_pProChan, cmd, data, size, 0);
}
int fb_nvEdite(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *info, const void *data, unsigned int size)
{

	return _nvEdiet_command_send(m_pProChan, cmd, info, data, size, 0);
}

int fb_writeSign(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, char *type, unsigned int size)
{
	return _writeSign_data(m_pProChan, cmd, data, size, type, 0);
}

int fb_downlad_img(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size)
{
	return _download_logo_img(m_pProChan, cmd, data, size, 0);
}
int fb_read_img(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, const void *Path)
{
	return _read_logo_img(m_pProChan, cmd, data, size, (char*)Path);
}