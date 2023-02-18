#include "transactions.h"

#include "audio.h"
#include "nv.h"

#include "connect_mqtt_soundbox_server.h"
#include "global.h"



int Trans_RegisterDevice_Process()
{
    char * payload = NULL;
    int ret = 0;
    const char *http_post_addr = NULL;
    char recv_buf[3072] = {0};
	int recv_len = 0;

    payload = createJsonForDeviceRegistration();

    if(payload == NULL || payload == 0)
        return ERROR;


    MercuryTrace("Payload: %s\r\n", payload);

    #if defined(DEVELOPMENT_BUILD)
    http_post_addr = "https://uat.notifypro.in/api/device/v1/soundbox/registration";
    #elif defined(PRODUCTION_BUILD)
    http_post_addr = "https://notifypro.in/api/device/v1/soundbox/registration";
    #endif

    MercuryTrace("URL: %s\r\n", http_post_addr);

    //get data by http-post-mode
	ret = sdk_http_post_auto(http_post_addr, payload, "application/json", recv_buf, 3072, &recv_len);
    LocalFree(payload); payload = NULL;
	
    MercuryTrace("post recv_buf:%s\n", recv_buf);
	MercuryTrace("post recv_len:%d\n", recv_len);

    MercuryTrace("----------------------------------------------\r\n");

    if( recv_len > 0 && (recv_buf[0] != 0) )
    {
        int retVal = parseRegisterDeviceResponse(recv_buf);
        if(ERROR == retVal)
        {
            // #if defined(AMR_AUDIO)
            // sdk_audio_play_nv(AUDIO_FAILURE+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_FAILURE));
            // #elif defined(MP3_AUDIO)
            // sdk_audio_play_nv_mp3(AUDIO_FAILURE);
            // #endif
            
            return ERROR;
        }

        int mqtt_thread_buffer_size = (64 * 1024);
        char *mqtt_thread_buffer = malloc(mqtt_thread_buffer_size);
        CreateThreadWithStack(NULL, mqtt_thread_buffer, mqtt_thread_buffer_size, mqtt_soundbox_thread,0,"MqTeStThReAd",STACK_SIZE_RESERVATION,NULL);

        // sdk_audio_play_nv(AUDIO_SETUP_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SETUP_SUCCESS));
        return SUCCESS;
    }


    return ERROR;    
}