// fastboot.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "fastboot.h"
#include "ICommChannel.h"
#include "EnumSerial.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

#define VERSION "v1.4.2"

#define skip(n) do { argc -= (n); argv += (n); } while (0)


#define WRSA_N   "d96c1340612be438a23013059129"\
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


#define WRSA_E   "10001"

BOOL g_bISRSA = FALSE;

RDA_KEY_PAIR_T keyPair = { 0 };
DLL_SIGN_FUNC dll_sign_func = NULL;
DLL_VERITY_FUNC dll_verify_func = NULL;
DLL_GENERATE_KEY_PAIRN_FUNC dll_generate_key_pair_func = NULL;

static BOOL ChannelOpen(int PortNum, ICommChannel* m_pProChan)
{
	CHANNEL_ATTRIBUTE caPro;
	DWORD PortOpen = 0;

	caPro.Com.dwPortNum = PortNum;
	caPro.ChannelType = CHANNEL_TYPE_COM;
	caPro.Com.dwBaudRate = 115200;
	
	return m_pProChan->Open(&caPro);
	
}

unsigned int ReadDataFromPort(unsigned char *Buffer, unsigned int BytesToBeRead, ICommChannel* m_pProChan)
{	
	return m_pProChan->Read((BYTE*)Buffer, BytesToBeRead, 10);
}

unsigned int ReadDataFromPort2(unsigned char *Buffer, unsigned int BytesToBeRead, ICommChannel* m_pProChan, DWORD dwTimeOut)
{
	return m_pProChan->Read((BYTE*)Buffer, BytesToBeRead, dwTimeOut);
}

void WriteDataToPort(unsigned char *Buffer, unsigned int BytesToBeWritten, ICommChannel* m_pProChan)
{
	
	m_pProChan->Write((char*)Buffer, BytesToBeWritten);

}
void ClosePort(ICommChannel* m_pProChan)
{
	m_pProChan->Close();
}

void portClear(ICommChannel* m_pProChan)
{
	m_pProChan->Clear();
}

int findUart(void)
{
	CString str;

	CArray<SSerInfo, SSerInfo&> asi;
	EnumSerialPorts(asi, TRUE);

	for (int i = 0; i < asi.GetSize(); i++)
	{
		str = asi[i].strFriendlyName;

		if (str.Find(_T("Unisoc Usb Serial Port 0")) != -1)
		{
			int count;
			int first = str.Find(_T('M'));
			count = str.Find(_T(')')) - str.Find(_T('M')) - 1;
			CString sSubStr = str.Mid(first + 1, count);

			return _ttoi(sSubStr);
		}

	}
	return -1;
}

char *load_file(const char *fn, unsigned *sz)
{

	FILE * fp = NULL;
	char *data;
	fp = fopen(fn, "rb");
	if (fp == NULL)
	{
		printf("File open error!\r\n");
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	data = (char *)malloc(size);
	fread(data, 1, size, fp);
	*sz = size;
	fclose(fp);

	CString tmp(fn);

	char buf[128] = { 0 };
	CFileStatus status;
	CString tst;
	
	CFile::GetStatus(tmp, status);
	tst.Format(_T("%s"), status.m_mtime.Format("%Y-%m-%d %H:%M:%S"));

	USES_CONVERSION;
	char *show = T2A(tst);

	sprintf(buf, "App buildtime : %s", show);
	printf("%s\r\n", buf);

	return data;
}
char *GetFilename(char *p)
{
	char ch = '\\';
	char *q = strrchr(p, ch);

	if (q == NULL)
	{
		return p;
	}
	else
	{
		return q+1;
	}

	
}

char *read_bmp(const char *fn, unsigned int *sz, unsigned int *mwidth, unsigned int *mheight)
{

	FILE * fp = NULL;
	char *data;
	int width = 0;
	int height = 0;

	unsigned int offset = 0;
	BITMAPINFOHEADER infoheader;
	BITMAPFILEHEADER header;

	fp = fopen(fn, "rb");
	if (fp == NULL)
	{
		printf("File open error!");
		return 0;
	}


	offset = fread(&header, 1, sizeof(BITMAPFILEHEADER), fp);

	fseek(fp, offset, SEEK_SET);
	offset = fread(&infoheader, 1, sizeof(BITMAPINFOHEADER), fp);
	width = infoheader.biWidth;
	height = infoheader.biHeight;
	//*sz = header.bfSize - header.bfOffBits;
	//*sz = infoheader.biSizeImage;


	*sz = width*height * 2;
	if (*sz <= 0)
	{
		return 0;
	}

	fseek(fp, header.bfOffBits, SEEK_SET);

	data = (char *)malloc(*sz);
	fread(data, 1, *sz, fp);
	fclose(fp);


	char *mdata = (char *)malloc(*sz);
	int index = 0;

	for (int i = height - 1; i >= 0; i--)
	{
		for (int j = 0; j<width * 2; j++)
		{
			mdata[index] = data[i * width * 2 + j];
			index++;
		}

	}

	free(data);
	*mwidth = width;
	*mheight = height;

	return mdata;

}

void usage(void)
{
	char tmp[4096];
	sprintf(tmp, "fastboot version %s\n\n%s", VERSION,
		"usage: fastboot [ COMX ] <parameter>\n"
		"\n"
		"commands:\n"
		"  flash app <filename>                                 write file to app partition\n"
		"  flash appExt <filename>                              write file to ext flash\n"
		"  flash logoPart <filename>                            write file to logo partition\n"
		"  flash logoPartRead <addroffset> <len>                read data from logo partition\n"
		"  run app                                              run application\n"
		"  autoRun                                              Auto run application when boot \n"
		"  dontAutoRun                                          Don't Auto run app when boot\n"
		"  mercuryVersion                                       show Mercurry version\n"
		"  throughMode <COMID>                                  set unvarnished transmission\n"
		"  writeAppInfo <hexdata>                               write data to nv\n"
		"  readAppInfo                                          read data from nv\n"
		"  readAppVer                                           read App ver and company\n"
		"  reset                                                reset device\n"
		"  mercuryID                                            mercury ID\n"
		"  powerOff                                             powerOff\n"
		"  getAutoRunStatus                                     get autoRun state\n"
		"  getCalFlag                                           get the calibration info\n"
		"  getSimInfo                                           get the SIM info\n"
		"  channelSwitch <COM0/USB>                             fastboot channle cfg\n"
		"  readImei                                             read the IMEI\n"
		"  readNV [ItemID]                                      read the NV\n"
		"  fileSys <cmd> [parameter]                            fileSystem cmd\n"
		"  writeSignData <img> <filename>                       writeSignData to flash\n"
		"  readPubKey                                           read public key\n"
		"  secureBoot                                           enable secure boot\n"
		"  readSecState                                         read secure boot state\n"
		"  setLogo [logoFile] [left] [top] [index] [BL] [GPIO]  download logo\n"
		"  deleteLogo                                           delete logo\n"
		"  checkApp                                             set App flag\n"
		"  setFotaInfo [picFile] [left] [top]                   set fota info pic\n"
		"  delFotaInfo                                          delete fota info pic\n"
		"  clearCalFlag [para]                                  para:0 clear Cal flag\n"
		"  dontCheckDown                                        Set download process is not verified\n"
		"  checkDown                                            Set download process verification\n"
		"  delRunNV                                             Delete runtime NV\n"
		"  writePubkeyRSA                                       Write public key for RSA\n"
		"  configBootOTP                                        Set boot Part OTP\n"
		"  help                                                 show this help message\n"
		"\n");

	printf(tmp);

}
void fileSysUsage(void)
{
	char tmp[4096];
	sprintf(tmp, "fastboot fileSys \n%s",
		"commands:\n"
		"  config [type] [dmaFlag] [clock]                             Config fs,comm spi clock unit is MHz\n"
		"  init [E:] [starAddr] [size]                                 init file system\n"
		"  initPlus [E:] [starAddr] [size] [spiFlsahSize]              init file system\n"
		"  fsMount [U] [starAddr] [size] [flashtype] [level]           mount fs\n"
		"  format [E:] [password]                                      format file system\n"
		"  dir [E:/]                                                   Display file list\n"
		"  push [local Path] [remote Path]                             push file\n"
		"  pull [remote Path] [local Path]                             pull file\n"
		"  createDir [E:/path]                                         create directory\n"
		"  deleteDir [E:/path]                                         delete directory\n"
		"  deleteFile [E:/path]                                        delete file\n"
		"  help                                                        show this help message\n"
		"\n");

	printf(tmp);
}

int Hex2bin(unsigned char *src, unsigned char *dest, int len)
{
	unsigned char hb;
	unsigned char lb;

	int result = 0;
	int i = 0, j = 0;

	for (i = 0, j = 0; i<len; i++)
	{
		hb = src[i];
		if (hb >= 'a' && hb <= 'f')
			hb = hb - 'a' + 10;
		else if (hb >= 'A' && hb <= 'F')
			hb = hb - 'A' + 10;
		else if (hb >= '0' && hb <= '9')
			hb = hb - '0';
		else
		{
			result = -1;
			goto exit;
		}
		i++;
		lb = src[i];
		if (lb >= 'a' && lb <= 'f')
		{
			lb = lb - 'a' + 10;
		}
		else if (lb >= 'A' && lb <= 'F')
		{
			lb = lb - 'A' + 10;
		}
		else if (lb >= '0' && lb <= '9')
		{
			lb = lb - '0';
		}
		else
		{
			result = -1;
			goto exit;
		}
		dest[j++] = (hb << 4) | (lb);
	}

exit:
	return result;
}

int main(int argc, char** argv)
{
    int nRetCode = 0;

	char *fData = NULL;
	char *data = NULL;
	HANDLE iFile = INVALID_HANDLE_VALUE;

	pfCreateChannel m_pfCreateChannel = NULL;
	pfReleaseChannel m_pfReleaseChannel = NULL;
	ICommChannel* m_pProChan = NULL;

	HMODULE  m_hChannelLib;
	CString   m_strPathDir;

	TCHAR path_tmp[MAX_PATH];
	::GetModuleFileName(NULL, path_tmp, MAX_PATH);

	m_strPathDir = (CString)path_tmp;
	int ii = m_strPathDir.ReverseFind('\\');
	m_strPathDir = m_strPathDir.Left(ii);
	int uartPortNo = 0;


    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 初始化 MFC 并在失败时显示错误
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 更改错误代码以符合您的需要
            wprintf(L"错误: MFC 初始化失败\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 在此处为应用程序的行为编写代码。
			DWORD iFileLen = 0;
			DWORD ReadWriteBytes = 0;
			CString strIniPath = m_strPathDir + _T("\\fastboot.ini");
			CFileFind finder;
			BOOL ifFind = finder.FindFile(strIniPath);
			if (!ifFind)
			{
				printf("Cannot open fastboot.ini\r\n");
				nRetCode = -1;
				goto EXIT;

			}
			CString strTmp;
			::GetPrivateProfileStringW(_T("Para Info"), _T("isRSA"), _T("0"), strTmp.GetBuffer(MAX_PATH), MAX_PATH, strIniPath);
			strTmp.ReleaseBuffer();
			if (strTmp == L"1")
			{
				g_bISRSA = TRUE;
			}


#if 0
			CString strPatcDLL = m_strPathDir + _T("\\Channel9D.dll");
#else
			CString strPatcDLL = m_strPathDir + _T("\\Channel9.dll");
#endif
			CString strKeyPairFile = m_strPathDir + _T("\\keyPair.bin");
			iFile = CreateFile(strKeyPairFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
			if (iFile == INVALID_HANDLE_VALUE)
			{
				printf("Cannot open keyPair.bin -- %s\r\n");
				nRetCode = -1;
				goto EXIT;
			}
			iFileLen = GetFileSize(iFile, NULL);
			if (iFileLen != sizeof(RDA_KEY_PAIR_T))
			{
				printf("keyPair.bin length is error!\r\n");
				nRetCode = -1;
				goto EXIT;
			}
			
			ReadFile(iFile, &keyPair, iFileLen, &ReadWriteBytes, NULL);
			if (ReadWriteBytes != iFileLen)
			{
				printf("Failed to Read keyPair.bin!\r\n");
				nRetCode = 1;
				goto EXIT;
			}

			m_hChannelLib = LoadLibrary(strPatcDLL);

			if (m_hChannelLib == NULL)
			{

				printf("Cannot load Channel DLL (%s).\r\n", strPatcDLL);
				nRetCode = -1;
				goto EXIT;
			}
			m_pfCreateChannel = (pfCreateChannel)GetProcAddress(m_hChannelLib, "CreateChannel");
			m_pfReleaseChannel = (pfReleaseChannel)GetProcAddress(m_hChannelLib, "ReleaseChannel");

			if (m_pfCreateChannel == NULL || m_pfReleaseChannel == NULL)
			{
				printf("Cannot load Channel DLL (%s).\r\n", strPatcDLL);
				nRetCode = -1;
				goto EXIT;
			}

			if (!m_pfCreateChannel)
			{

				printf("CreateChannel Faild\r\n");
				nRetCode = -1;
				goto EXIT;
			}
			if (!m_pfCreateChannel((ICommChannel **)&m_pProChan, CHANNEL_TYPE_COM))
			{
				printf("CreateChannel Faild\r\n");
				nRetCode = -1;
				goto EXIT;
			}

			HINSTANCE hDll = NULL;
			if (hDll = LoadLibrary(TEXT("RDASign.dll")))
			{
				dll_sign_func = (DLL_SIGN_FUNC)GetProcAddress(hDll, "sign");
				if (dll_sign_func == NULL)
				{
					printf("generate_key_pair dll call fail \n");
					nRetCode = -1;
					goto EXIT;
				}
					

				dll_verify_func = (DLL_VERITY_FUNC)GetProcAddress(hDll, "verify");
				if (dll_verify_func == NULL)
				{
					printf("generate_key_pair dll call fail \n");
					nRetCode = -1;
					goto EXIT;
				}
			}
			else
			{
				printf("rdasign dll load fail\n");
				nRetCode = 1;
				goto EXIT;
			}

			if (argc < 2)
			{
				usage();
				nRetCode = -1;
				goto EXIT;
			}
			if (!strcmp(argv[1], "help"))
			{
				usage();
				nRetCode = 0;
				goto EXIT;
			}
			if (strstr(argv[1],"COM") != NULL)
			{
				CString str(argv[1]);
				CString strTmp = str.Right(str.GetLength() - 3);

				uartPortNo = _ttoi(strTmp);
				skip(1);
			}
			else
			{
				uartPortNo = findUart();
			}

			
			if (!ChannelOpen(uartPortNo, m_pProChan))
			{
				printf("Open uart%d fail!\r\n", uartPortNo);
				nRetCode = -1;
				goto EXIT;
			}
			printf("Open uart%d...\r\n", uartPortNo);
			skip(1);

			if (!strcmp(*argv, "flash"))
			{
				skip(1);
				if (argc != 2 && argc != 4)
				{
					printf("usge: fastboot.exe flash app filePath\r\n");
					printf("usge: fastboot.exe flash app filePath\r\n");
					printf("usge: fastboot.exe flash appExt filePath\r\n");
					printf("usge: fastboot.exe flash logoPart filePath\r\n");
					printf("usge: fastboot.exe flash logoPartRead addrOffset len filePath\r\n");
					goto EXIT;
				}
				if (!strcmp(*argv, "app"))
				{
					skip(1);
					char *fname = *argv;					
					unsigned int fileSize = 0;
					char tmp[256] = {0};

					data = load_file(fname, &fileSize);
					if (data == NULL)
					{
						printf("Get app data fail!\r\n");
						goto EXIT;
					}
					sprintf(tmp, "Download app %d byte", fileSize);
					fb_queue_download(MC_DOWNLOAD_APP, tmp, data, fileSize);

					

				}
				else if (!strcmp(*argv, "appExt")) 
				{
					skip(1);
					char *fname = *argv;
					unsigned int fileSize = 0;
					char tmp[256] = { 0 };

					data = load_file(fname, &fileSize);
					if (data == NULL)
					{
						printf("Get app data fail!\r\n");
						goto EXIT;
					}
					sprintf(tmp, "Download app %d byte to ext flash", fileSize);
					fb_queue_download(MC_DOWNLOAD_APP_EXT, tmp, data, fileSize);
				}
				else if (!strcmp(*argv, "logoPart"))
				{
					skip(1);
					char *fname = *argv;
					unsigned int fileSize = 0;
					char tmp[256] = { 0 };

					data = load_file(fname, &fileSize);
					if (data == NULL)
					{
						printf("Get logo data fail!\r\n");
						goto EXIT;
					}
					sprintf(tmp, "Download data to logo partition %d byte", fileSize);
					
					fb_queue_img(MC_LOGOFLASH_DATAWRITE, tmp, data, fileSize);
				}
				else if (!strcmp(*argv, "logoPartRead"))
				{
					skip(1);

					if (argc != 3)
					{
						printf("fastboot logoPartRead addrOffset len path!\r\n");
						goto EXIT;
					}
					MC_DOWNLOAD_IMG_BLOCK rdata = {0};
					rdata.addr = atoi(*argv);
					skip(1);
					rdata.len = atoi(*argv);
					skip(1);
					fb_queue_readLogoPart(MC_LOGOFLASH_DATAREAD, "Read data from logoPart", (char*)&rdata, sizeof(MC_DOWNLOAD_IMG_BLOCK), *argv);

				}
				else
				{
					printf("usge: fastboot.exe flash app filePath\r\n");
					printf("usge: fastboot.exe flash logoPart filePath\r\n");
					printf("usge: fastboot.exe flash appExt filePath\r\n");
					goto EXIT;
				}
			}
			else if (!strcmp(*argv, "setLogo"))
			{
				MERCURY_BOOT_IMAGE_S logoInfo = { 0 };
				unsigned int width = 0;
				unsigned int height = 0;
				unsigned int sz = 0;

				skip(1);
				if (argc != 9 && argc != 5)
				{
					printf("usge: fastboot.exe setLogo logoFile leftPoint topPint LCDType backLightLevel [powerGpioNo] [GPIOLevel] [LCDIOLevel] [LCDClk]\r\n");
					goto EXIT;
				}
				char *fname = argv[0];
				fData = read_bmp(fname, &sz, &width, &height);
				if (fData == NULL)
				{
					goto EXIT;
				}
				if (sz > 150*1024)
				{
					printf("The picture is too big");
					goto EXIT;
				}

				logoInfo.magicNum = 0x5a5aa5a5;
				logoInfo.height = height;
				logoInfo.width = width;
				logoInfo.imageLen = sz;
				logoInfo.left = atoi(argv[1]);
				logoInfo.top = atoi(argv[2]);
				logoInfo.lcdTypeIndex = atoi(argv[3]);
				logoInfo.bootloaderLogoFinish = 0;
				logoInfo.backLightLevel = atoi(argv[4]);
				if (argc == 9)
				{
					logoInfo.powerGpioNo = atoi(argv[5]);
					logoInfo.gpioLevel = atoi(argv[6]);
					logoInfo.LCDIOLevel = atoi(argv[7]);
					logoInfo.LCDClk = atoi(argv[8]);
				}
					
				else
					logoInfo.powerGpioNo = 254;

				printf("logoInfo.imageLen:%d\r\n", logoInfo.imageLen);
				printf("logoInfo.left:%d\r\n", logoInfo.left);
				printf("logoInfo.top:%d\r\n", logoInfo.top);
				printf("logoInfo.lcdTypeIndex:%d\r\n", logoInfo.lcdTypeIndex);
				printf("logoInfo.powerGpioNo:%d\r\n", logoInfo.powerGpioNo);

				data = (char*)malloc(sizeof(MERCURY_BOOT_IMAGE_S) + sz);
				memcpy(data, &logoInfo, sizeof(MERCURY_BOOT_IMAGE_S));
				memcpy(data + sizeof(MERCURY_BOOT_IMAGE_S), fData, sz);

				fb_queue_img(MC_SET_LOGO, "Download logo", data, sizeof(MERCURY_BOOT_IMAGE_S) + sz);

			}
			else if (!strcmp(*argv, "setFotaInfo"))
			{
				MERCURY_SHOW_FOTA_INFO_S fotaInfo = { 0 };
				unsigned int width = 0;
				unsigned int height = 0;
				unsigned int sz = 0;

				skip(1);
				if (argc != 3)
				{
					printf("usge: fastboot.exe setFotaInfo logoFile leftPoint topPint\r\n");
					goto EXIT;
				}
				char *fname = argv[0];
				fData = read_bmp(fname, &sz, &width, &height);
				if (fData == NULL)
				{
					goto EXIT;
				}
				if (sz > (16 * 1024 - sizeof(MERCURY_SHOW_FOTA_INFO_S)))
				{
					printf("The picture is too big");
					goto EXIT;
				}

				fotaInfo.magicNum = 0x5a5aa5a5;
				fotaInfo.height = height;
				fotaInfo.width = width;
				fotaInfo.imageLen = sz;
				fotaInfo.left = atoi(argv[1]);
				fotaInfo.top = atoi(argv[2]);
				

				printf("logoInfo.imageLen:%d\r\n", fotaInfo.imageLen);
				printf("logoInfo.left:%d\r\n", fotaInfo.left);
				printf("logoInfo.top:%d\r\n", fotaInfo.top);
				printf("logoInfo.height:%d\r\n", fotaInfo.height);
				printf("logoInfo.width:%d\r\n", fotaInfo.width);

				data = (char*)malloc(sizeof(MERCURY_SHOW_FOTA_INFO_S) + sz);
				memcpy(data, &fotaInfo, sizeof(MERCURY_SHOW_FOTA_INFO_S));
				memcpy(data + sizeof(MERCURY_SHOW_FOTA_INFO_S), fData, sz);

				fb_queue_img(MC_CMD_FOTA_INFO, "Download logo", data, sizeof(MERCURY_SHOW_FOTA_INFO_S) + sz);

			}
			else if (!strcmp(*argv, "deleteLogo"))
			{
				fb_queue_command(MC_DEL_LOGO, "Delete logo", NULL, 0);
			}
			else if (!strcmp(*argv, "mercuryVersion"))
			{
				fb_queue_command(MC_READ_VER, "Mercury Version", NULL, 0);
			}
			else if (!strcmp(*argv, "modelName"))
			{
				fb_queue_command(MC_READ_MODELNAME, "Model name", NULL, 0);
			}
			
			else if (!strcmp(*argv, "readImei"))
			{
				fb_queue_command(MC_READ_IMEI, "read imei", NULL, 0);
			}
			else if (!strcmp(*argv, "run"))
			{
				skip(1);
				if (*argv == NULL)
				{
					printf("usge: fastboot.exe run app\r\n");
					goto EXIT;
				}
				if (!strcmp(*argv, "app"))
				{
					fb_queue_command(MC_RUN_APP, "run app", NULL, 0);
				}
				else
				{
					printf("usge: fastboot.exe run app\r\n");
					goto EXIT;
				}
				
			}
			else if (!strcmp(*argv, "getCalFlag"))
			{
				fb_queue_command(MC_READ_CAL, "Get calibration flag", NULL, 0);
			}
			else if (!strcmp(*argv, "clearCalFlag"))
			{
				skip(1);
				if (*argv == NULL)
				{
					printf("usge: fastboot.exe clearCalFlag 0/1\r\n");
					goto EXIT;
				}

				fb_queue_command(MC_CMD_CLEAR_CALIFLAG, "Set CalFlag", argv[0], strlen(argv[0]));
			}

			else if (!strcmp(*argv, "autoRun"))
			{
				fb_queue_command(MC_AUTO_RUN_APP, "Set auto run app", "1", strlen("1"));
			}
			else if (!strcmp(*argv, "dontAutoRun"))
			{
				fb_queue_command(MC_AUTO_RUN_APP, "Set don't auto run app", "0", strlen("0"));
			}
			else if (!strcmp(*argv, "dontCheckDown"))
			{
				fb_queue_command(MC_CMD_DOWNLOADAPP_CHECKSET, "Set download process is not verified", "0", strlen("0"));
			}
			else if (!strcmp(*argv, "checkDown"))
			{
				fb_queue_command(MC_CMD_DOWNLOADAPP_CHECKSET, "Set download process verification", "1", strlen("1"));
			}
			else if (!strcmp(*argv, "getAutoRunStatus"))
			{
				fb_queue_command(MC_GET_AUTO_RUN_STATUS, "Get auto run status", NULL, 0);
			}
			else if (!strcmp(*argv, "throughMode"))
			{
				skip(1);
				if (*argv == NULL)
				{
					printf("usge: fastboot.exe throughMode COM0/COM1\r\n");
					goto EXIT;
				}
				if (!strcmp(*argv, "COM0"))
				{
					fb_queue_command(MC_THROUGH_MODE, "Set through mode", "COM0", strlen("COM0"));
				}
				else if (!strcmp(*argv, "COM1"))
				{
					fb_queue_command(MC_THROUGH_MODE, "Set through mode", "COM1", strlen("COM1"));
				}
				else
				{
					printf("usge: fastboot.exe throughMode COM0/COM1\r\n");
					goto EXIT;
				}
				
			}
			else if (!strcmp(*argv, "getSimInfo"))
			{
				fb_queue_command(MC_READ_SIM_INFO, "Get SIM info", NULL, 0);
			}
			else if (!strcmp(*argv, "reset"))
			{
				fb_queue_command(MC_RESET_SYSTEM, "Reset system", NULL, 0);
			}
			else if (!strcmp(*argv, "powerOff"))
			{
				fb_queue_command(MC_POWER_DOWN, "System power off", NULL, 0);
			}
			else if (!strcmp(*argv, "mercuryID"))
			{
				fb_queue_command(MC_READ_MERCURYID, "Read mercury Id", NULL, 0);
			}
			else if (!strcmp(*argv, "channelSwitch"))
			{
				skip(1);
				if (*argv == NULL)
				{
					printf("usge: fastboot.exe channelSwitch COM0/USB\r\n");
					goto EXIT;
				}
				if (argc != 1)
				{
					printf("usge: fastboot.exe channelSwitch COM0/USB\r\n");
					goto EXIT;
				}
				if (!strcmp(*argv, "COM0"))
				{
					fb_queue_command(MC_CHANNEL_SWITCH, "Fastboot channel switch to Uart0", *argv, strlen(*argv));
				}
				else if(!strcmp(*argv, "USB"))
				{
					fb_queue_command(MC_CHANNEL_SWITCH, "Fastboot channel switch to USB", *argv, strlen(*argv));
				}
				else
				{
					printf("usge: fastboot.exe channelSwitch COM0/USB\r\n");
					goto EXIT;
				}
			}
			else if (!strcmp(*argv, "readNV"))
			{
				char tmp[256] = { 0 };
				skip(1);
				if (argc != 1)
				{
					printf("usge: fastboot.exe readNV ItemID\r\n");
					goto EXIT;
				}
				sprintf(tmp, "Read NV itemID %s", *argv);
				fb_queue_nvEdite(MC_READ_NV, tmp, *argv, NULL, 0);

			}
			else if (!strcmp(*argv, "readAppInfo"))
			{
				skip(1);
				if (argc != 0)
				{
					printf("usge: fastboot.exe readAppInfo\r\n");
					goto EXIT;
				}
				fb_queue_command(MC_READ_APPINFO, "Read App info", NULL, 0);
			}
			else if (!strcmp(*argv, "writeAppInfo"))
			{
				UINT8 Binary[1024] = {0};
				int BinaryLen = 0;

				skip(1);
				if (argc != 1)
				{
					printf("usge: fastboot.exe writeAppInfo hexdta\r\n");
					goto EXIT;
				}
				//Binary2Hex(Binary, UINT32 BinaryLen, UINT8 *Hex, UINT32 *HexLen);
				BinaryLen = Hex2bin((unsigned char*)*argv, Binary, strlen(*argv));
				if (BinaryLen == -1)
				{
					printf("data error\r\n");
					goto EXIT;
				}


				fb_queue_command(MC_WRITE_APPINFO, "Wead App info", (char*)Binary, strlen(*argv)/2);
			}
			else if (!strcmp(*argv, "writePubkeyRSA"))
			{
				MERCURY_SECURERSA_S secInfoRSA = {0};
				skip(1);
				if (argc != 0)
				{
					printf("usge: fastboot.exe writePubkeyRSA\r\n");
					goto EXIT;
				}
				
				strcpy((char*)&secInfoRSA.pubkeyRSA_N, WRSA_N);
				strcpy((char*)&secInfoRSA.pubkeyRSA_E, WRSA_E);

				fb_queue_command(MC_WRITE_PUB_KEY, "write rsa Pubkey", (char*)&secInfoRSA, sizeof(MERCURY_SECURERSA_S));
			}
			else if (!strcmp(*argv, "configBootOTP"))
			{
				fb_queue_command(MC_SET_OTP, "Set boot part OTP", NULL, 0);
			}
			else if (!strcmp(*argv, "readAppVer"))
			{
				skip(1);
				if (argc != 0)
				{
					printf("usge: fastboot.exe readAppVer\r\n");
					goto EXIT;
				}
				fb_queue_command(MC_READ_COMPANY, "Read App version and company,if App exist", NULL, 0);
			}
			else if (!strcmp(*argv, "fileSys"))
			{
				skip(1);
				if (*argv == NULL)
				{
					fileSysUsage();
					goto EXIT;
				}
				if (!strcmp(*argv, "help"))
				{
					fileSysUsage();
					goto EXIT;
				}
				else if (!strcmp(*argv, "config"))
				{
					skip(1);
					MC_FILESYS_CONFIG fileSysConfig = {};
					if (argc != 3)
					{
						fileSysUsage();
						goto EXIT;
					}
					fileSysConfig.type = atoi(*argv);
					skip(1);
					fileSysConfig.dmaFlag = atoi(*argv);
					skip(1);
					fileSysConfig.baud = atoi(*argv);

					fb_queue_fileSys(MC_FILESYSTE_CONFIG, "Config file system", (const char*)&fileSysConfig, sizeof(MC_FILESYS_INFO));
				}
				else if (!strcmp(*argv, "init"))
				{
					skip(1);
					if (argc != 3)
					{
						fileSysUsage();
						goto EXIT;
					}
					MC_FILESYS_INFO fileSysInfo = {0};
					strcpy(fileSysInfo.point, *argv);
					skip(1);
					fileSysInfo.startAddr = atoi(*argv);
					skip(1);
					fileSysInfo.fileSysCap = (FILESYS_CAPACITY_E)atoi(*argv);
					printf("addr %d, size %d\r\n", fileSysInfo.startAddr, fileSysInfo.fileSysCap);

					fb_queue_fileSys(MC_FILESYSTE_INIT, "Init file system", (const char*)&fileSysInfo, sizeof(MC_FILESYS_INFO));
				}
				else if (!strcmp(*argv, "initPlus"))
				{
					skip(1);
					if (argc != 4)
					{
						fileSysUsage();
						goto EXIT;
					}
					MC_FILESYS_INFO fileSysInfo = { 0 };
					strcpy(fileSysInfo.point, *argv);
					skip(1);
					fileSysInfo.startAddr = atoi(*argv);
					skip(1);
					fileSysInfo.fileSysCap = (FILESYS_CAPACITY_E)atoi(*argv);
					skip(1);
					fileSysInfo.spiFlashCap = (FILESYS_CAPACITY_E)atoi(*argv);
					printf("addr %d, size %d, fSize %d\r\n", fileSysInfo.startAddr, fileSysInfo.fileSysCap, fileSysInfo.spiFlashCap);
					fb_queue_fileSys(MC_FILESYSTE_INIT_PLUS, "Init file system", (const char*)&fileSysInfo, sizeof(MC_FILESYS_INFO));

				}
				else if (!strcmp(*argv, "fsMount"))//fsMount
				{
					skip(1);
					if (argc != 5)
					{
						fileSysUsage();
						goto EXIT;
					}
					
					MARS_FS_SET_T marsFsSet = { 0 };

					marsFsSet.disk_sym = *argv[0];
					if (marsFsSet.disk_sym != 'U' && marsFsSet.disk_sym!='B' && marsFsSet.disk_sym !='D')
					{
						printf("disk_sym must be 'U' 'B' 'D'\r\n");

						goto EXIT;
					}
					skip(1);

					marsFsSet.start_add = atoi(*argv);
					skip(1);
					marsFsSet.size = atoi(*argv);
					skip(1);
					marsFsSet.flashType = atoi(*argv);
					skip(1);
					marsFsSet.ioLevel = atoi(*argv);
					marsFsSet.powerLevel = atoi(*argv);

					fb_queue_fileSys(MC_FILESYSTE_INIT_PLUS, "Mount file system", (const char*)&marsFsSet, sizeof(MARS_FS_SET_T));
				}
				
				else if (!strcmp(*argv, "format"))
				{
					skip(1);
					if (argc != 2)
					{
						fileSysUsage();
						goto EXIT;
					}
					MC_FILESYS_FORMAT fileFormat = {0};
					strcpy(fileFormat.point, *argv);
					skip(1);
					strcpy(fileFormat.password, *argv);
					skip(1);
					fb_queue_fileSys(MC_FILESYSTE_FORMAT, "Format file system", (char*)&fileFormat, sizeof(MC_FILESYS_FORMAT));
				}
				else if (!strcmp(*argv, "push"))
				{
					
					unsigned int fileSize = 0;
					char tmp[256] = { 0 };

					skip(1);
					if (argc != 2)
					{
						fileSysUsage();
						goto EXIT;
					}
					data = load_file(*argv, &fileSize);
					if (data == NULL)
					{
						//printf("usge: fastboot.exe flash app fileName\r\n");
						goto EXIT;
					}
					skip(1);

					sprintf(tmp, "Push file %d byte", fileSize);

					//fb_queue_command(MC_FILESYSTE_PUSH_FILE, "push file system", NULL, 0);
					fb_queue_push(MC_FILESYSTE_PUSH_FILE, tmp, *argv, data, fileSize);

				}
				else if (!strcmp(*argv, "pull"))
				{
					char tmp[256] = { 0 };
					char remotePath[256] = { 0 };
					skip(1);
					if (argc != 2)
					{
						fileSysUsage();
						goto EXIT;
					}
					strcpy(remotePath, *argv);
					sprintf(tmp, "Pull file %s", remotePath);
					skip(1);
					fb_queue_pull(MC_FILESYSTE_PULL_FILE, tmp, remotePath, *argv, strlen(remotePath));
				}
				else if (!strcmp(*argv, "deleteFile"))
				{
					skip(1);
					char tmp[256] = { 0 };

					if (argc != 1)
					{
						fileSysUsage();
						goto EXIT;
					}

					sprintf(tmp, "Delete file %s", *argv);
					fb_queue_fileSys(MC_FILESYSTE_DELETE_FILE, tmp, *argv, strlen(*argv));
				}
				else if (!strcmp(*argv, "createDir"))
				{
					skip(1);
					char tmp[256] = { 0 };

					if (argc != 1)
					{
						fileSysUsage();
						goto EXIT;
					}

					sprintf(tmp, "Create Directory %s", *argv);
					fb_queue_fileSys(MC_FILESYSTE_CREATE_DIR, tmp, *argv, strlen(*argv));
				}
				else if (!strcmp(*argv, "deleteDir"))
				{
					skip(1);
					char tmp[256] = { 0 };

					if (argc != 1)
					{
						fileSysUsage();
						goto EXIT;
					}

					sprintf(tmp, "Delete directory %s", *argv);
					fb_queue_fileSys(MC_FILESYSTE_DELETE_DIR, tmp, *argv, strlen(*argv));
				}
				else if (!strcmp(*argv, "dir"))
				{
					skip(1);
					char tmp[256] = { 0 };
					if (argc != 1)
					{
						fileSysUsage();
						goto EXIT;
					}
					//if (memcmp(*argv, "E:", strlen("E:")) != 0)
					//{
					//	fileSysUsage();
					//	goto EXIT;
					//}
					sprintf(tmp, "List dir file %s", *argv);
					fb_queue_fileSys(MC_FILESYSTE_DIR_PATH, tmp, *argv, strlen(*argv));
				}
				else
				{
					fileSysUsage();
					goto EXIT;

				}

			}
			else if (!strcmp(*argv, "readPubKey"))
			{
				fb_queue_command(MC_READ_PUB_KEY, "Read public key", NULL, 0);
			}
			else if (!strcmp(*argv, "secureBoot"))
			{
				fb_queue_command(MC_SEC_ENABLE, "Enable secure boot", NULL, 0);
			}
			else if (!strcmp(*argv, "readSecState"))
			{
				fb_queue_command(MC_GET_SEC_STATE, "Read secure boot state", NULL, 0);
			}
			else if (!strcmp(*argv, "writeSignData"))
			{
				skip(1);
				if (argc != 2)
				{
					printf("usge: fastboot.exe writeSignData boot filePath\r\n");
					printf("usge: fastboot.exe writeSignData ap filePath\r\n");
					printf("usge: fastboot.exe writeSignData ft filePath\r\n");
					printf("usge: fastboot.exe writeSignData cp filePath\r\n");

					goto EXIT;
				}
				
				if (!strcmp(*argv, "boot"))
				{

					skip(1);
					char *fname = *argv;
					unsigned int fileSize = 0;
					char tmp[256] = { 0 };

					data = load_file(fname, &fileSize);
					if (data == NULL)
					{
						//printf("usge: fastboot.exe flash app fileName\r\n");
						printf("Get data fail!\r\n");
						goto EXIT;
					}

					if (fileSize != 48960)
					{
						printf("The file size error: %d!\r\n", fileSize);
						goto EXIT;
					}
					sprintf(tmp, "Write sign data to bootloader");
					fb_queue_writeSign(MC_WRITE_SIGN_DATA, tmp, data, "boot", fileSize);

				}
				else if (!strcmp(*argv, "ap"))
				{
					skip(1);
					char *fname = *argv;
					unsigned int fileSize = 0;
					char tmp[256] = { 0 };

					data = load_file(fname, &fileSize);
					if (data == NULL)
					{
						//printf("usge: fastboot.exe flash app fileName\r\n");
						printf("Get data fail!\r\n");
						goto EXIT;
					}

					sprintf(tmp, "Write sign data to ap");
					fb_queue_writeSign(MC_WRITE_SIGN_DATA, tmp, data, "ap", fileSize);
				}	
				else if (!strcmp(*argv, "ft"))
				{
					skip(1);
					char *fname = *argv;
					unsigned int fileSize = 0;
					char tmp[256] = { 0 };

					data = load_file(fname, &fileSize);
					if (data == NULL)
					{
						//printf("usge: fastboot.exe flash app fileName\r\n");
						printf("Get data fail!\r\n");
						goto EXIT;
					}

					sprintf(tmp, "Write sign data to fastboot");
					fb_queue_writeSign(MC_WRITE_SIGN_DATA, tmp, data, "ft", fileSize);
				}
				else if (!strcmp(*argv, "cp"))
				{
					skip(1);
					char *fname = *argv;
					unsigned int fileSize = 0;
					char tmp[256] = { 0 };
					char *path = NULL;
					char remoteFilePath[64] = "/modem/";

					data = load_file(fname, &fileSize);
					if (data == NULL)
					{
						//printf("usge: fastboot.exe flash app fileName\r\n");
						printf("Get data fail!\r\n");
						goto EXIT;
					}
					path = GetFilename(fname);
					strcat(remoteFilePath, path);
					//printf("%s\r\n", path);
					//fb_queue_writeSign(MC_WRITE_SIGN_DATA, tmp, data, "ft", fileSize);
					//printf("%s\r\n", remoteFilePath);
					sprintf(tmp, "write sign data %s to cp.\r\n", remoteFilePath);
					fb_queue_push(MC_FILESYSTE_PUSH_FILE, tmp, remoteFilePath, data, fileSize);
					
					
				}
				else
				{
					printf("usge: fastboot.exe writeSignData boot filePath\r\n");
					printf("usge: fastboot.exe writeSignData ap filePath\r\n");
					printf("usge: fastboot.exe writeSignData ft filePath\r\n");
					printf("usge: fastboot.exe writeSignData cp filePath\r\n");

					goto EXIT;
				}
			}
			else if (!strcmp(*argv, "checkApp"))
			{
				fb_queue_command(MC_CHECK_APP, "Check app", NULL, 0);
			}
			else if (!strcmp(*argv, "delFotaInfo"))
			{
				fb_queue_command(MC_CMD_DEL_FOTA_INFO, "Delete FotaInfo", NULL, 0);
			}
			else if (!strcmp(*argv, "delRunNV"))
			{
				fb_queue_command(MC_RUNTIMENV_ERASE, "delete runtime NV", NULL, 0);
			}
			else
			{
				usage();
				nRetCode = 0;
				goto EXIT;
			}
			
			fb_execute_queue(m_pProChan);

        }
    }
    else
    {
        // TODO: 更改错误代码以符合您的需要
        wprintf(L"错误: GetModuleHandle 失败\n");
        nRetCode = 1;
    }
EXIT:
	if (iFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(iFile);
		iFile = INVALID_HANDLE_VALUE;
	}
	if (m_pProChan)
	{
		ClosePort(m_pProChan);
		m_pfReleaseChannel(m_pProChan);
	}
	if (data != NULL)
	{
		free(data);
	}
	if (fData != NULL)
	{
		free(fData);
	}
	//system("pause");
    return nRetCode;
}
