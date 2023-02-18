#ifndef __AMOI_FACTORYTEST_H__
#define __AMOI_FACTORYTEST_H__

#ifndef _INT8_T_DECLARED
typedef char        int8_t;
#define __INT8_T__
#define _INT8_T_DECLARED
#endif

#ifndef __INT16_T__
typedef short       int16_t;
#define __INT16_T__
#endif

#ifndef _INT32_T_DECLARED
typedef int         int32_t;
#define __INT32_T__
#define _INT32_T_DECLARED
#endif

#ifndef __U8_T__
typedef unsigned char u8_t;
#define __U8_T__
#endif

#ifndef __UINT8_T__
typedef unsigned char   uint8_t;
#define __UINT8_T__
#endif

#ifndef __UINT16_T__
typedef unsigned short  uint16_t;
#define __UINT16_T__
#endif

#ifndef __U16_T__
typedef unsigned short u16_t;
#define __U16_T__
#endif

#ifndef _UINT32_T_DECLARED
typedef unsigned int    uint32_t;
#define _UINT32_T_DECLARED
#endif

typedef unsigned char               bool;
#define true                        1
#define false                       0

#define ADC_CHANNEL_MAX 31

typedef void (*osiCallback_t)(void *ctx);
typedef void (*PsFuncPtr) (void *);
typedef void (*play_complete_cb)(void);
typedef void (*kpd_map_cb)(uint32_t id, uint32_t single);

enum ft_adc_scale
{
    FT_ADC_SCALE_1V250 = 0,
    FT_ADC_SCALE_2V444 = 1,
    FT_ADC_SCALE_3V233 = 2,
    FT_ADC_SCALE_5V000 = 3,
    FT_ADC_SCALE_MAX = 3,
};

typedef enum{
    LED_TYPE_LCD = 0,
    LED_TYPE_INICATION,
    LED_TYPE_CAMFLASH,
    LED_TYPE_MAX
}BACKLIGHT_TYPE_E;

typedef enum
{
    AURECORDER_STATUS_IDLE,     ///< recorder not started
    AURECORDER_STATUS_RECORD,   ///< recorder is started
    AURECORDER_STATUS_PAUSE,    ///< recorder is paused
    AURECORDER_STATUS_FINISHED, ///< recorder is finished
} RecorderStatus_E;

typedef enum
{
    FT_CLK_26M_USER_AUDIO = (1 << 0),
    FT_CLK_26M_USER_WCN = (1 << 1),
    FT_CLK_26M_USER_ADC = (1 << 2),
    FT_CLK_26M_USER_AUX1 = (1 << 3),
} ft_clock26MUser_t;

/**
 * \brief audio stream format
 */
typedef enum
{
    FT_AUSTREAM_FORMAT_UNKNOWN, ///< placeholder for unknown format
    FT_AUSTREAM_FORMAT_PCM,     ///< raw PCM data
    FT_AUSTREAM_FORMAT_WAVPCM,  ///< WAV, PCM inside
    FT_AUSTREAM_FORMAT_MP3,     ///< MP3
    FT_AUSTREAM_FORMAT_AMRNB,   ///< AMR-NB
    FT_AUSTREAM_FORMAT_AMRWB,   ///< AMR_WB
    FT_AUSTREAM_FORMAT_SBC,     ///< bt SBC
} ft_auStreamFormat_t;

typedef struct _ft_pcm_config
{
    int channels;
    int samplerate;
    int format;
}ft_pcm_config;

typedef struct
{
    uint8_t imsi_len;
    uint8_t imsi_val[20];
} FT_SIM_IMSI_T;

typedef struct
{
    uint8_t id_num[20 + 1];
} FT_SIM_ICCID_T;

typedef struct osiEvent
{
    uint32_t id;     ///< event identifier
    uint32_t param1; ///< 1st parameter
    uint32_t param2; ///< 2nd parameter
    uint32_t param3; ///< 3rd parameter
} osiEvent_t;

void* ft_thread_create(const char *name, osiCallback_t func, void *argument,
                             uint32_t priority, uint32_t stack_size,
                             uint32_t event_count);
void ft_thread_exit(void);
void ft_sleep(uint32_t ms);
bool ft_event_send(void *thread, const osiEvent_t *event);
bool ft_event_wait(void *thread, osiEvent_t *event, uint32_t timeout);

int32_t ft_uart_register(uint32_t port, PsFuncPtr callback_uart, void *Param);
int32_t ft_uart_open(uint32_t port, uint32_t baudrate, int32_t flowCtrl);
int32_t ft_uart_write(uint32_t port, uint8_t* data, int32_t writeLen );
int32_t ft_uart_read(uint32_t port, uint8_t* data, uint32_t readLen);
int32_t ft_uart_close(uint32_t port);

//int ft_usb_set_cb(uint32_t id, void* thread, bool enable);
int ft_usb_open(uint32_t id, PsFuncPtr readCb, void *Param);
int ft_usb_close(uint32_t id);
uint32_t ft_usb_write(uint32_t id, uint8_t *buffer, uint32_t length);
uint32_t ft_usb_read(uint32_t id, uint8_t *buffer, uint32_t length);

//audio play
int32_t ft_pcm_play_init(play_complete_cb play_cb);
int32_t ft_pcm_play(ft_pcm_config pcm_config, unsigned char *buffer, int size);
int32_t ft_pcm_play_stop(void);
int32_t ft_play_tone(uint32_t tone, uint32_t duration);
int32_t ft_stop_tone(void);

int32_t ft_audio_set_output(uint32_t channel);
uint32_t ft_audio_get_output(void);
int32_t ft_audio_set_play_volume(uint32_t vol);
uint32_t ft_audio_get_play_volume(void);

int32_t ft_pmu_switch_power(uint32_t id, bool enabled, bool lp_enabled);
bool ft_pmu_set_power_level(uint32_t id, uint32_t mv);
int32_t ft_set_light_level(uint32_t type, bool onOff, uint32_t light_level);

void* ft_keypad_listen(uint32_t id, kpd_map_cb func);
int32_t ft_adc_get_channel_volt(uint32_t id, int32_t scale);

uint32_t ft_get_charger_state(void);
uint32_t ft_get_charge_curr_value(void);

bool ft_iomux_set_function(uint32_t fun_pad);
bool ft_iomux_set_pad_pull(uint32_t fun_pad, uint32_t pull);
bool ft_get_sim_info(FT_SIM_IMSI_T* imsi_str, FT_SIM_ICCID_T* ccid);

int32_t ft_read_imei(int simid, char *imei);

bool ft_start_record(uint32_t format, uint32_t size);
bool ft_stop_record(void);
uint32_t ft_get_record_buff(char **buffer);
uint32_t ft_get_record_state(void);
int32_t ft_audio_set_input(uint32_t channel);
uint32_t ft_audio_get_input(void);

void ft_clock26M_request(uint32_t user);
void ft_clock26M_release(uint32_t user);

void ft_set_bias_en(uint32_t type, uint32_t en);
int ft_get_spi_flash_id(int flash_chl);

#endif
