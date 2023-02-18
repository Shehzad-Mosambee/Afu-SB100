#ifndef _SDK_API_SYS_H
#define _SDK_API_SYS_H

#include "MercuryDef.h"
#include "MercuryPlat.h"
/*************************************************
 *function  : system init, please call at start
 *parameter :
 *return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_System_Init( void );

/*************************************************
 *function  : GPRS init
 *parameter :
 *return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_network_gprs_init( void );

/*************************************************
 *function  : wifi init
 *parameter :
 *return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_network_wifi_init( void );

/*************************************************
 *function  : check wifi init OK, make sure this function return TRUE before call other wifi interface
 *parameter :
 *return    : TRUE indicate wifi init OK, FALSE indicate wifi init not OK
 * **********************************************/
BOOL sdk_get_wifi_init_OK( void );


#define GPRS_NETWORK_STATUS_OK			0		
#define GPRS_NETWORK_STATUS_DISCONN		-1	
#define GPRS_NETWORK_STATUS_INITIAL		-2	
#define GPRS_NETWORK_STATUS_NEEDRESET	-15	

/*************************************************
 *function  : check gprs network ready, make sure this function return TRUE before use gprs network
 *parameter :
 *return    : TRUE indicate gprs network ready, FALSE indicate gprs network not ready
 * **********************************************/
int sdk_get_gprs_network_status( void );

/*************************************************
 *function  : get SDK VERSION
 *parameter :
 *return    :
 * **********************************************/
char *sdk_GET_SDK_VERSION( void );

//network mode enum
typedef enum
{
	NETWORK_MODE_GPRS = 0,
	NETWORK_MODE_WIFI = 1,
}NETWORK_MODE_E;

/*************************************************
 *function  : set network mode
 *parameter : mode
 *return    :
 * **********************************************/
int sdk_set_network_mode( NETWORK_MODE_E mode );

/*************************************************
 *function  : get current network mode
 *parameter : 
 *return    : mode
 * **********************************************/
NETWORK_MODE_E sdk_get_network_mode( void );


void sdk_wifi_power_off(void);
void sdk_wifi_power_on(void);
/*************************************************
 *function  : read secure boot rsa
 *parameter : 
 *return    : 
 * **********************************************/
int sdk_read_secure_boot_rsa( unsigned char *secure_boot_rsa );

/*************************************************
 *function  : read secure boot state
 *parameter : 
 *return    : 
 * **********************************************/
int sdk_read_secure_boot_state( MERCURY_SECURE_STATE *sec );


/*************************************************
 *function  : sdk_uri_encode
 *parameter : 
 *  uri: GBK charset url
 *  out_arry: out buffer for encoding url
 *  buf_size: out buffer size
  *return    : encoding url
 * **********************************************/
unsigned char *sdk_uri_encode(unsigned short *uri, unsigned char *out_array, int buf_size);

/*************************************************
 *function  : sdk_hmac_sha1
 *parameter : 
   *return    : 
 * **********************************************/
void sdk_hmac_sha1(const uint8_t *msg, uint32_t msg_len, const uint8_t *key, uint32_t key_len, uint8_t output[20]);

struct st_sdk_ps_notify
{
	void *(*cb_pdp_deattach_station)(void *);
	void *(*cb_chr_plug_in)(void *);
	void *(*cb_chr_plug_out)(void *);
};

int sdk_powersave_notify_reg_cb(struct st_sdk_ps_notify *pnotify);


/* SDK debug level set */
#define LOG_DLTEMP              0x8   
#define LOG_DLDEBUG             0x4  
#define LOG_DLRELEASE           0x2   
#define LOG_DLCLOSE            0      
void sdk_set_log_level(int level);

struct st_qyy_power_save
{
	BOOL enable;

	/* POWER ENABLE */
	BOOL wifi_power;

	/* Should lt mqtt keepalive interval */
	unsigned int select_outtime_hms;
};	
int sdk_power_save(void *arg);

void sdk_gpio_key_reg_cb(
			void *(*vu)(void*)
			,void *(*vd)(void*)
			,void *(*rep)(void*)
			,void *(*lvu)(void*)
			,void *(*lvd)(void*)
			,void *(*lrep)(void*)
			,void *(*lvu_lrep)(void*)
			);
void sdk_gpio_key_reg_cb_short_double(
			void *(*vu_db)(void*)
			,void *(*vd_db)(void*)
			,void *(*rep_db)(void*)
			);
void sdk_power_save_key_fetch(void);

typedef void *(*QYY_GPIO_KEY_CB)(void *);
struct st_qyy_gpio_key_cb
{
	QYY_GPIO_KEY_CB	short_volume_up;
	QYY_GPIO_KEY_CB	short_volume_down;
	QYY_GPIO_KEY_CB	short_replay;

	/* 2021/03/03 */
	QYY_GPIO_KEY_CB	short_volume_up_double;
	QYY_GPIO_KEY_CB	short_volume_down_double;
	QYY_GPIO_KEY_CB	short_replay_double;


	QYY_GPIO_KEY_CB	long_volume_up;
	QYY_GPIO_KEY_CB	long_volume_down;
	QYY_GPIO_KEY_CB	long_replay;

	QYY_GPIO_KEY_CB both_volup_replay;
	QYY_GPIO_KEY_CB both_voldown_replay;

	QYY_GPIO_KEY_CB bothr_voldown_replay;
};

#define DEFAULT_TIMEZONE 8
//timezone [-11, 13]
int sdk_get_ntptime(int timezone );

// unsigned char * sdk_utf_to_gb(const char *utf8);
#endif
