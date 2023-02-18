#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryMem.h"
#include "MercuryFile.h"

#include "speakerapp.h"

// demo for http get mode
void test_http_get(const char * httpUrl)
{
	int ret = 0;
	char *buff;
	int recv_len = 0;
	// const char *httpUrl= "http://152.136.213.55:10501/aux/get-resp-n-bytes?len=64300";

	//malloc memory to store data received from http server
	buff = LocalAlloc(LMEM_ZEROINIT, 3*1024);

	//get data by http-get-mode
	ret = sdk_http_get(httpUrl, NULL, NULL, buff, 3*1024, &recv_len);
	MercuryTrace("http get buf:%s\n", buff);
	MercuryTrace("http get len:%d\n", recv_len);

	//http get success, user can deal with the received data
	// in this demo, store the received data to mounted flash
	if (ret == 0)  
	{
		char FilePath[256] = {0};
		HANDLE fp;
		LONG nwrite;

		//create file on mounted flash to store data from http server
		// for file system, file name need to be converted to unicode encoding from ascii encoding
		CharString2UnicodeString(FilePath, "E://http_rspdata.txt");
		fp = CreateFile(FilePath,MCFILE_OPERATE_MODE_WRITE|MCFILE_ACCESS_MODE_CREATE_ALWAYS,
            0,NULL,0,0,0);

		// create file fail
	    if(fp == NULL)
	    {
	    	MercuryTrace("open http data file fail \n");
			LocalFree(buff);
	    	return -1;
	    }

		// write received data to file
		WriteFile(fp, buff, recv_len, &nwrite, NULL);

		//close file handle
		CloseHandle(fp);
	}
	else 
	{
		MercuryTrace("not recv rsp \n");		
	}
	LocalFree(buff);
	
	return 0;
}

void test_http() {
	int i;
	// const char *httpUrl[]= {
	// 	"https://www.ytwei.cn:8150/transcoding/time/str2timestamp?time_str=2021.01.20%2016:38:55",
	// 	NULL
	// 	};
	// for (i=0;;i++) {
	// 	char * url = httpUrl[i];
	// 	if (url == NULL) 
	// 		break;
	// 	MercuryTrace("==========> Step %d: test http get: %s\n", i, url);
	// 	test_http_get(url);
	// }

	const char *otaUrl[]= {
		// "https://iotx-ota.oss-cn-shanghai.aliyuncs.com/ota/4c435487e4b517a175e6145f801ac4f0/ckwnbc2yd00083p8a5cojcqsc.bin?Expires=1638581991&OSSAccessKeyId=LTAI4G1TuWwSirnbAzUHfL3e&Signature=6Me0%2FMBXkGYc%2BZxQCpWG4gwK9Ug%3D",
		"https://www.ytwei.cn:8150/speaker/fw/download-bin2?devSN=cs_1201",
		// "http://www.ytwei.cn:10501/speaker/fw/download-bin2?devSN=cs_1201",
		NULL
		};
	for (i=0;;i++) {
		char * url = otaUrl[i];
		if (url == NULL) 
			break;
		// MercuryTrace("==========> Step %d: test OTA download: %s\n", i*2, url);
		// qyy_set_ota_mode(0, 8);
		// qyy_fota_download_http(url);

		MercuryTrace("==========> Step %d: test OTA download (Range): %s\n", i*2 + 1, url);
		qyy_set_ota_mode(1, 8);
		qyy_fota_download_http(url);
	}
	while (1)
	{
		Sleep(100);
	}
} 

//demo for http post mode
void test_http_post(void)
{
	int ret = 0;
	char buff[3072] = {0};
	// const char *http_post_addr = "http://152.136.213.55:10501/transcoding/b64/encode";
	// const char *data = "plain_str=123456&url_safe=1";
	const char *http_post_addr = "http://152.136.213.55:10501/aux/post-resp-n-bytes";
	const char *data = "len=2000";
	int recv_len = 0;

	//get data by http-post-mode
	ret = sdk_http_post_auto(http_post_addr, data, "application/x-www-form-urlencoded", buff, 3072, &recv_len);
	MercuryTrace("post recv_buf:%s\n", buff);
	MercuryTrace("post recv_len:%d\n", recv_len);
	
	//user can deal with the data receiced here
	
	return ret;
}

