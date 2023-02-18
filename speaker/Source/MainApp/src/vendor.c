#include "MercuryDef.h"
#include "MercuryCSTD.h"
#include "MercuryPlat.h"
#include "MercuryNv.h"
#include "MercuryCom.h"
#include "MercuryTimer.h"

#include "SDK_API_POWER.h"

#include "vendor.h"
#include "nv.h"
#include "app_at.h"
#include "speakerapp.h"
#include "device.h"

#define USB_BUFFER_SIZE 2048

int vendor_UsbWrite(uint8 *pSendBuff, uint32 nSendBuffLen)
{
	uint32 bytesWrite = 0;
	VirComWrite(PORT_ID, pSendBuff, nSendBuffLen, &bytesWrite);
	return bytesWrite;
}

int vendor_UsbRead(uint8* pReadBuffer, uint32 nReadBufferLength, uint32 timeout)
{
	uint32 bytesRead = 0;
	VirComRead(PORT_ID, pReadBuffer, nReadBufferLength, &bytesRead, timeout);
	return bytesRead;
}

int set_Lifecycle(int life) //������������
{
	int ret;

	ret = NV_Write(NV_SYS_LIFECYCLE, sizeof(int), &life);
	if( 0 != ret ){
		MercuryTrace("ret=%x \n", ret);
		NV_Delete(NV_SYS_LIFECYCLE);
	}
	return ret;
}

int get_lifecycle(void)       //��ȡ��������
{
	int ret;
	int life = LIFECYCLE_VENDOR;
		
	MercuryTrace("[speaker app] get life cycle \n");
	
	ret = NV_Read(NV_SYS_LIFECYCLE, sizeof(int), &life);
	if (ret != 0 || life != LIFECYCLE_APP)
	{
		MercuryTrace("[speaker app] get life cycle fail \n");
		life = LIFECYCLE_VENDOR;
		ret = NV_Write(NV_SYS_LIFECYCLE, sizeof(int), &life);
	}

	MercuryTrace("[speaker app] get life cycle %0x \n", life);
	return life;
}


static void vendor_speaker_timer_callback( ULONG argc )
{
	// sdk_voiceplay(1,"���Ȳ���");
}

//дGPRS����
static int vendor_write_gprs_enable( int gprs_enable )
{
	int ret;

	//д��GPRS����
	ret = NV_Write(NV_GPRS_ENABLE, sizeof(int), &gprs_enable);
	if( 0 == ret )
	{
		MercuryTrace("NV_GPRS_ENABLE write success\r\n");
		return 0;
	}
	else
	{
		MercuryTrace("NV_GPRS_ENABLE write fail\r\n");
		return -1;
	}

}
//дWIFI����
static int vendor_write_wifi_enable( int wifi_enable )
{
	int ret;

	//д��WIFI����
	ret = NV_Write(NV_WIFI_ENABLE, sizeof(int), &wifi_enable);
	if( 0 == ret )
	{
		MercuryTrace("NV_WIFI_ENABLE write success\r\n");
		return 0;
	}
	else
	{
		MercuryTrace("NV_WIFI_ENABLE write fail\r\n");
		return -1;
	}
}

BOOL device_gprs_enable( void )
{
	int ret;
	int gprs_enable;

	//��GPRS����
	ret = NV_Read(NV_GPRS_ENABLE, sizeof(int), &gprs_enable);
	if( 0 == ret )
	{
		if( DEVICE_GPRS_ENABLE_TRUE == gprs_enable )
		{
			MercuryTrace("nv info gprs enable\r\n");
			return TRUE;
		}
		else
		{
			MercuryTrace("nv info gprs disable\r\n");
			return FALSE;
		}
	}
	else
	{
		MercuryTrace("read gprs enable nv info fail\r\n");
		return FALSE;
	}
}



struct vendor_at_cmd
{
	const char *cmd;

	int (*func)(char *data, int len);
};



int _vendor_at_toApp(char *data, int len)
{
	return set_Lifecycle(LIFECYCLE_APP); 
}

int _vendor_write_appinfo(int type, char *data, int len)
{
	AppInfoStruct app_info = {0};
	
	NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info);

	// if(app_info.model[0]==0)
    //     strncpy(app_info.model, "QF100", 5);

	switch(type)
	{
		case 1:
			strcpy(app_info.SN, data);			
			break;
		case 2:
			strcpy(app_info.cfg_serv_host, data);
			break;
		case 3:
			app_info.cfg_serv_port = atoi(data);
			break;
		case 4:
			strcpy(app_info.model, data);
			break;
		case 5:
			strcpy(app_info.item_no, data);
			break;

	}

	NV_Write(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info);
}

char vendor_sn[32] = {0};

// void app_license_apply_thread( ULONG argc, LPVOID lpThreadParameter )
// {	 
// 	sdk_license_apply(CUSTOMER_NAME, vendor_sn, 1);
	
// 	ExitThread(NULL);
// }

int _vendor_at_SN(char *data, int len)
{
	_vendor_write_appinfo(1, data, len);
	
	NV_Write(NV_SN, 32, data);
	strcpy(vendor_sn, data);
	
	// CreateThread(NULL,8*1024, app_license_apply_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);
	return 0;
}
int _vendor_at_serv_host(char *data, int len)
{
	_vendor_write_appinfo(2, data, len);

	NV_Write(NV_SERVERHOST, 64, data);
	return 0;
}
int _vendor_at_serv_port(char *data, int len)
{
	int port;
	
	_vendor_write_appinfo(3, data, len);

	port = atoi(data);
	NV_Write(NV_SERVERPORT, sizeof(int), &port);
	return 0;
}
int _vendor_at_dev_model(char *data, int len)
{
	_vendor_write_appinfo(4, data, len);

	return 0;
}
int _vendor_at_item_number(char *data, int len)
{
	_vendor_write_appinfo(5, data, len);
	return 0;
}

long data_hex( char *data){
	BYTE *k;
	long kk = strtol(data, &k, 16);
	MercuryTrace("..$$..........%02x.......\n",kk);
	return kk;
}

long data_hex2( char *data){
	long kk = strtol(data, (char **)NULL, 16);
	MercuryTrace("..$$..........%02x.......\n",kk);
	return kk;
}

int _vendor_at_toVendor(char *data, int len)
{
    NV_Delete(NV_VENDOR_TEST_RESULTS);

	set_restart_reason( E_RST_REBOOT );
	sdk_system_restart(NULL);
    return 0;
}

// ����ģʽ��ȡSN
int _vendor_at_get_SN(char *data, int len)
{
	return app_at_get_SN(data, len);
}

// ����ģʽ��ȡIMSI��ICCID
int _vendor_at_get_simInfo(char *data, int len)
{
    return app_at_get_simInfo(data, len);
}

// ����ģʽ��ȡIMEI
int _vendor_at_get_imei(char *data, int len)
{
	return app_at_get_imei(data, len);
}

// ����ģʽATָ���
static struct vendor_at_cmd vendor_at_tbl[] = 
{
	{USB_CMD_MANU_TO_APP, _vendor_at_toApp},
	{USB_CMD_MANU_TO_VENDOR, _vendor_at_toVendor},
	{USB_CMD_MANU_SN, _vendor_at_SN},
	{USB_CMD_MANU_SERV_HOST, _vendor_at_serv_host},
	{USB_CMD_MANU_SERV_PORT, _vendor_at_serv_port},
	{USB_CMD_MANU_PRODUCT_MODEL, _vendor_at_dev_model},
	{USB_CMD_MANU_PROJ_ID, _vendor_at_item_number},
	{USB_CMD_MANU_GET_SN, _vendor_at_get_SN},
	{USB_CMD_MANU_GET_SIMINFO, _vendor_at_get_simInfo},
	{USB_CMD_MANU_GET_IMEI, _vendor_at_get_imei},

	{NULL, NULL},
};


// APPӦ��ģʽATָ���
static struct vendor_at_cmd app_at_tbl[] = 
{
	{USB_CMD_APP_GET_SN, app_at_get_SN},
	{USB_CMD_APP_GET_SIMINFO, app_at_get_simInfo},
	{USB_CMD_APP_GET_IMEI, app_at_get_imei},
	{USB_CMD_APP_GET_PLATFORM, app_at_get_bottomBag_version},
	{USB_CMD_APP_GET_APP_VERSION, app_at_get_appVersion},
	{USB_CMD_APP_GET_PRODUCT_MODEL, app_at_get_dev_model},
	{USB_CMD_APP_GET_PROJ_ID, app_at_get_item_num},
	{USB_CMD_APP_GET_BATTERY_LEVEL, app_at_get_battery_level},

	{NULL, NULL},
};

static int vendor_usb_cmd_process( const char *cmd, int len )
{
	int cmd_len = 0;
	int i=0;
	int ret = -1;

	while(1)
	{
		if (LIFECYCLE_VENDOR == get_lifecycle()) {
			if (vendor_at_tbl[i].cmd == NULL)
				break;
			
			cmd_len = strlen(vendor_at_tbl[i].cmd);
			if (strncmp(vendor_at_tbl[i].cmd, cmd, cmd_len) == 0)
			{
				ret = vendor_at_tbl[i].func(cmd + cmd_len, len - cmd_len);
			}
		} else {
			if (app_at_tbl[i].cmd == NULL)
				break;
		
			cmd_len = strlen(app_at_tbl[i].cmd);
			if (strncmp(app_at_tbl[i].cmd, cmd, cmd_len) == 0)
			{
				ret = app_at_tbl[i].func(cmd + cmd_len, len - cmd_len);
			}
		}

		i++;
	}
	
	if( -1 != ret )
	{
		MercuryTrace("usb cmd success\r\n");
	}
	else
	{
		MercuryTrace("usb cmd fail\r\n");
	}
	return ret;
}

void vendor_usb_recv_thread(ULONG argc, LPVOID lpThreadParameter)
{
	char buffer[USB_BUFFER_SIZE];
	MERCURY_MESSAGE_S *at_msg = NULL;
	MERCURY_COM_MSG_DATA_T *msgData;
	// MercuryFastbootStateSet(FASTBOOT_STOP);
	// while(1)
	// {
	// 	if( FASTBOOT_STOP == MercuryFastbootStateGet() )
	// 	{
	// 		break;
	// 	}
	// 	else
	// 	{
	// 		MercuryTrace("fastboot stopping\r\n");
	// 		Sleep(100);
	// 	}
	// }

#define USB_READ_TIMEOUT 100
#define USB_CMD_HEAD "AT+"
#define USB_CMD_TAIL "\r\n"
#define USB_CMD_RESULT_SUCCESS "OK\r\n" //ָ��ִ�гɹ�
#define USB_CMD_RESULT_FAIL "ERROR\r\n" //ָ��ִ��ʧ��
#define USB_CMD_READ_TIMEOUT 0			//��λ S

	while (1)
	{
		int ret_val;
		uint32 ret = 0;
		int count = 0;
		memset(buffer, 0, USB_BUFFER_SIZE);
		MercuryGetMessage(&at_msg, GetCurrentThread());
		if (at_msg->MessageID == MERCURY_MSG_ID_COM_NEW_DATA)
		{
			if (at_msg->dataLen == sizeof(MERCURY_COM_MSG_DATA_T))
			{
				msgData = at_msg->pdata;
				if (msgData->comType == (uint8)MERCURY_COM_TYPE_USB)
				{
					//ÿ�ζ�һ��ָ��
					while (1)
					{
						// ret = MercuryUsbRead((unsigned char *)(buffer+count), 1);
						ret = vendor_UsbRead((unsigned char *)(buffer + count), 1, USB_CMD_READ_TIMEOUT);
						if (0 == ret)
						{
							// read timeout
							Sleep(100);
							continue;
						}
						MercuryTrace("usb read ret = %u\r\n", ret);
						count += ret;

						//һ��ָ�����
						if (count >= strlen(USB_CMD_TAIL))
						{
							if (0 == strncmp(buffer + count - strlen(USB_CMD_TAIL), USB_CMD_TAIL, strlen(USB_CMD_TAIL)))
							{
								MercuryTrace("usb cmd end\r\n");
								break;
							}
						}

						//�������
						if (count >= USB_BUFFER_SIZE)
						{
							MercuryTrace("usb cmd beyond limit\r\n");
							break;
						}
					}
				}
			}
		}

		if (at_msg->pdata)
			LocalFree(at_msg->pdata);
		if (at_msg)
			LocalFree(at_msg);
		//�ж�ָ���ʽ�Ƿ���ȷ
		if (count >= (strlen(USB_CMD_HEAD) + strlen(USB_CMD_TAIL)))
		{
			if ((0 != strncmp(buffer, USB_CMD_HEAD, strlen(USB_CMD_HEAD))) ||
				(0 != strncmp(buffer + count - strlen(USB_CMD_TAIL), USB_CMD_TAIL, strlen(USB_CMD_TAIL))))
			{
				MercuryTrace("usb cmd format error\r\n");
				vendor_UsbWrite(USB_CMD_RESULT_FAIL, strlen(USB_CMD_RESULT_FAIL));
				continue;
			}
		}
		else
		{
			MercuryTrace("usb cmd length error\r\n");
			vendor_UsbWrite(USB_CMD_RESULT_FAIL, strlen(USB_CMD_RESULT_FAIL));
			continue;
		}

		// ���β��\r\n
		buffer[count - strlen(USB_CMD_TAIL)] = 0;

		//ָ�����
		ret_val = vendor_usb_cmd_process(buffer + strlen(USB_CMD_HEAD), count - strlen(USB_CMD_HEAD) - strlen(USB_CMD_TAIL));
		if (0 == ret_val)
		{
			//����ִ�гɹ�
			vendor_UsbWrite(USB_CMD_RESULT_SUCCESS, strlen(USB_CMD_RESULT_SUCCESS));
		}
		// else if( 1 == ret_val )
		// {
		// 	//�յ�cmd:fastboot����ʱ,��FASTBOOT���˳����߳�
		// 	vendor_UsbWrite(USB_CMD_RESULT_SUCCESS, strlen(USB_CMD_RESULT_SUCCESS));
		// 	MercuryFastbootStateSet(FASTBOOT_RUNNING);
		// 	return;
		// }
		else
		{
			//����ִ��ʧ��
			vendor_UsbWrite(USB_CMD_RESULT_FAIL, strlen(USB_CMD_RESULT_FAIL));
		}
	}
}

int app_vendor_proc(void)
{
    HANDLE at_thread_handle = NULL;
	if (LIFECYCLE_VENDOR == get_lifecycle())
	{
		// sdk_voiceplay(1, "���к�δд�룬��д�����к�");
		// vendor_usb_recv_thread(0, NULL);
        at_thread_handle = CreateThread(NULL,16 * 1024,vendor_usb_recv_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);
    	VirCom_SetCb(at_thread_handle, PORT_ID, TRUE);
	}
	return 0;
}
