#include "MercuryCSTD.h"
#include "MercuryPlat.h"
#include "audio.h"

#include "nv.h"
#include "global.h"

char l_sAudioType[MAX_AUDIO_FILE_NAME_LEN] = DEF_AUDIO_TYPE;

void set_audio_type(char * sAudioType) {
    strcpy(l_sAudioType, sAudioType);
}

void set_audio_type_int(int nAudioType)
{
	switch(nAudioType)
	{
		case AUDIO_TYPE_AMR:
			set_audio_type(".amr");
			break;		
		case AUDIO_TYPE_WAV:
			set_audio_type(".wav");
			break;
		case AUDIO_TYPE_PCM:
			set_audio_type(".pcm");
			break;
		case AUDIO_TYPE_MP3:
			set_audio_type(".mp3");
			break;
	}
}

char * get_audio_type(void) {
    return l_sAudioType;
}

void get_audio_file_name_byid(char * fileName, int index)
{
	int i;
		
	for (i=0;;i++)
	{
		if (g_audioMapTbl[i].id == AUDIO_INDEX_MAX)
			break;
		if (index == g_audioMapTbl[i].id)
			break;
	}

	if (i == AUDIO_INDEX_MAX)
		return;
	
    format_audio_file_name(fileName, g_audioMapTbl[i].fileid, l_sAudioType);
}

void get_audio_file_name(char * fileName, int fileId) {
    format_audio_file_name(fileName, fileId, l_sAudioType);
}

void format_audio_file_name(char * fileName, int fileId, char * sAudioType) {
    sprintf(fileName, "%saudio_%d%s", FS_DRIVE, fileId, sAudioType);
}

// 获取数字的 单位 语音文件名称
void get_unit_audio_file_name(int nDecBit, char * fileName) {
    switch (nDecBit) 
    {
    // case 2: // 百
    //     get_audio_file_name_byid(fileName, AUDIO_HUNDRED);
    //     break;
    // case 3: // 千
    //     get_audio_file_name_byid(fileName, AUDIO_THOUSAND);
    //     break;
    // case 6: // 百万
    //     get_audio_file_name_byid(fileName, AUDIO_MILLION);
    //     break;
    default:
        strcpy(fileName, "");
        break;
    }
}

// 获取数字的 单位 语音索引
void get_unit_audio_index(int nDecBit, int *index)
{
    switch (nDecBit)
	{
    // case 2: // 百
    //     *index = AUDIO_HUNDRED;
    //     break;
    // case 3: // 千
	//     *index = AUDIO_THOUSAND;
    //     break;
    // case 6: // 百万
    // 	*index = AUDIO_MILLION;
    //     break;
    default:
        break;
    }
}

// 获取个位数或十几的语音文件名称
void get_digit_audio_file_name(int nDigit, char * fileName) 
{
    
    // int fileID = 0;
    // if (nDigit <= 0) {
    //     strcpy(fileName, "");
    //     return;
    // } else if (nDigit < 10) {
	// 	fileID = 41000+nDigit;
    // } else if (nDigit == 10) {
	// 	fileID = 41013;
    //     sprintf(fileID, "41013");
    // } else if (nDigit < 20) {
	// 	fileID = 41000+nDigit+7;
    // } else {
	// 	fileID = 41000+nDigit / 10 + 25;
    // }
    //  get_audio_file_name(fileName, fileID);


    int fileID = 0;
    if (nDigit <= 0) 
    {
        strcpy(fileName, "");
        return;
    }
    else if (nDigit <= 10) 
    {
		fileID = 10000+nDigit;
    }
    else if (nDigit > 10 && nDigit < 20) 
    {
		fileID = 10014+(nDigit-10);
    }
    // else
    // {
	// 	fileID = 10000+nDigit / 10 + 25;
    // }

    get_audio_file_name(fileName, fileID);
}

// 获取个位数或十几的语音文件名称
void get_digit_audio_index(int nDigit, int *index)
{
    if (nDigit <= 0)
	{
         return;
    }
	else if (nDigit < 10)
	{
		// *index = AUDIO_ONE+nDigit-1;
    }
	else if (nDigit == 10)
	{
        // *index = AUDIO_TEN;
    }
	else if (nDigit < 20)
	{
        // *index = AUDIO_ELEVN+nDigit-11;
    }
	else
	{
		// *index = AUDIO_TWENTY+ nDigit/10-2;
    }
}

// 前面插入 分隔符
void prefix_splitter(char * filesBuffer) {
    memmove(filesBuffer + 1, filesBuffer, strlen(filesBuffer) + 1);
    filesBuffer[0] = '|';
}

// 前面插入 语音文件名
void prefix_audio_file_name(char * filesBuffer, char * audioFile) {
    if (strlen(audioFile) == 0)
        return;
    prefix_splitter(filesBuffer);
    memmove(filesBuffer + strlen(audioFile), filesBuffer, strlen(filesBuffer) + 1);
    memcpy(filesBuffer, audioFile, strlen(audioFile));
}

// 前面插入 数字
void prefix_digit(int nNumber, char * filesBuffer) {
    char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};
    get_digit_audio_file_name(nNumber, fileName);
    prefix_audio_file_name(filesBuffer, fileName);
    // MercuryTrace("after prefix_audio_file_name filesBuffer=%s fileName=%s \r\n", filesBuffer, fileName);
}

// 前面插入 单位
void prefix_unit(int nDigitPos, char * filesBuffer) {
    char unitFile[MAX_AUDIO_FILE_NAME_LEN] = {0};
    get_unit_audio_file_name(nDigitPos, unitFile);
    prefix_audio_file_name(filesBuffer, unitFile);
}

// 前面插入 and
void prefix_and(char * filesBuffer) {
    char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};
    // get_audio_file_name_byid(fileName, AUDIO_AND);
    prefix_audio_file_name(filesBuffer, fileName);
}

void prefix_two_digits_audio_file(int nNumber, char * filesBuffer) {
    char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};
    int nTens;
    if (nNumber == 0)
        return;
    if (nNumber < 20) {
        // 1--19
        prefix_digit(nNumber, filesBuffer);
    } else {
        // 20以上，十位 + 个位
        nTens = nNumber / 10;
        nNumber = nNumber % 10;
        prefix_digit(nNumber, filesBuffer);
        prefix_digit(nTens * 10, filesBuffer);
    }
}

void prefix_two_digits_audio_index(int nNumber, int index_array[2], int *res_num)
{
    int nTens;
    if (nNumber == 0)
    {
    	*res_num = 0;
    	return;
    }
    if (nNumber < 20)
	{
        // 1--19
        get_digit_audio_index(nNumber, &index_array[0]);
		*res_num = 1;
    } 
	else
	{
        // 20以上，十位 + 个位
        nTens = nNumber / 10;
        nNumber = nNumber % 10;
		get_digit_audio_index(nTens * 10, &index_array[0]);
		*res_num = 1;
		if (nNumber)
		{	
			get_digit_audio_index(nNumber, &index_array[1]);
			*res_num = 2;
		}
    }
}

void prefix_three_digits_audio_file(int nNumber, char * filesBuffer)
{
    int nHundreds = nNumber / 100;
    nNumber = nNumber % 100;

    // 处理两位数
    prefix_two_digits_audio_file(nNumber, filesBuffer);
    // MercuryTrace("after prefix_two_digits_audio_file nHundreds=%d nNumber=%d\r\n", nHundreds, nNumber);
    if (nHundreds > 0) {
        if ((nNumber < 10) && (nNumber > 0)) {
            // 没有tens，用and连接
            prefix_and(filesBuffer);
        }
        // 几百
        prefix_unit(2, filesBuffer);
        prefix_digit(nHundreds, filesBuffer);
    }
}

void prefix_three_digits_audio_index(int nNumber, int index_array[10], int *res_num)
{
	int array[2];
	int two_num = 0;
	int offset = 0;
	
    int nHundreds = nNumber / 100;
    nNumber = nNumber % 100;

    if (nHundreds > 0)
	{
		get_digit_audio_index(nHundreds, &index_array[offset++]);
		
		// index_array[offset++] = AUDIO_HUNDRED;

		if ((nNumber < 10) && (nNumber > 0))
		{
            // 没有tens，用and连接
            // index_array[offset++] = AUDIO_AND;
        }
    }
    // 处理两位数
	prefix_two_digits_audio_index(nNumber, &array, &two_num);
	if (two_num == 2)
	{
		index_array[offset++] = array[0];
		index_array[offset++] = array[1];
	}
	else if(two_num == 1)
		index_array[offset++] = array[0];
	*res_num = offset;
}


int num_to_audio_files_list(int nNumber, char * filesBuffer) 
{
    int nDigit = 0;
    int nDigitPos = 0; // 当前处理的位数，偏移量，从0开始
    while (1) 
    {
        nDigit = nNumber % 1000;
        nNumber = nNumber / 1000;

        MercuryTrace("nDigit = %d\r\n", nDigit);
        MercuryTrace("nNumber = %d\r\n", nNumber);

        // 填数字：几百几十几
        prefix_three_digits_audio_file(nDigit, filesBuffer);
        MercuryTrace("after prefix_three_digits_audio_file nDigitPos=%d filesBuffer=%s \r\n", nDigitPos, filesBuffer);

        // 已处理完3位
        nDigitPos += 3;
        // 剩余数字
        if (nNumber == 0)
            break;

        MercuryTrace("before prefix_unit nDigitPos=%d filesBuffer=%s \r\n", nDigitPos, filesBuffer);
        // 获取单位，并添加到数字前
        prefix_unit(nDigitPos, filesBuffer);
        MercuryTrace("after prefix_unit nDigitPos=%d filesBuffer=%s \r\n", nDigitPos, filesBuffer);
    }

    return 0;
}




/*

int num_to_audio_999(int nAmount, const unsigned char ln)
{
    // 999
    int tempAmt = nAmount / 100;
    if(tempAmt > 0)
    {
        #if defined(DUAL_AUDIO)
        if((unsigned char)0 == ln)
        {
            play_audio(tempAmt);
            play_audio(AUDIO_HUNDRED);
        }
        else
        {
            play_audio(tempAmt+OTH_AUDIO_000);
            play_audio(OTH_AUDIO_HUNDRED);
        }
        #else
        // #if defined(AMR_AUDIO)
	    // sdk_audio_play_nv(tempAmt+NV_AUDIO_START_INDEX, get_nv_audio_data_len(tempAmt));
	    // sdk_audio_play_nv(AUDIO_HUNDRED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_HUNDRED));
        // #elif defined(MP3_AUDIO)
        // sdk_audio_play_nv_mp3(tempAmt);
        // sdk_audio_play_nv_mp3(AUDIO_HUNDRED);
        // #endif
        play_audio(tempAmt);
        play_audio(AUDIO_HUNDRED);
        #endif
    }

    nAmount = nAmount % 100;

    if((unsigned char)0 == ln)
    {
        if(nAmount > 20)
        {
            tempAmt = nAmount / 10;
            // sdk_audio_play_nv((tempAmt*10)+NV_AUDIO_START_INDEX, get_nv_audio_data_len(tempAmt*10));
            play_audio(AUDIO_020 + (((tempAmt*10)-AUDIO_020)/10));
            nAmount = nAmount % 10;
        }
    }

    if(nAmount > 0)
    {
        #if defined(DUAL_AUDIO)
        if((unsigned char)0 == ln)
            play_audio(nAmount);
        else
            play_audio(nAmount+OTH_AUDIO_000);
        #else
        // #if defined(AMR_AUDIO)
        // sdk_audio_play_nv(nAmount+NV_AUDIO_START_INDEX, get_nv_audio_data_len(nAmount));
        // #elif defined(MP3_AUDIO)
        // sdk_audio_play_nv_mp3(nAmount);
        // #endif
        play_audio(nAmount);
        #endif
    }
}

int num_to_audio_mosambee(int nAmount, const unsigned char ln) 
{
    if(nAmount <= 0)
	    return 1;

    int tempAmt;  
    if(nAmount >= CRORE)
    {
        MercuryTrace("Parsing Crore---------------------->\r\n");
        tempAmt = nAmount / CRORE;
        MercuryTrace("tempAmt = %d\r\n------------------->", tempAmt);

        num_to_audio_999(tempAmt, ln);
        
        #if defined(DUAL_AUDIO)
        if((unsigned char)0 == ln)
            play_audio(AUDIO_CRORE);
        else
            play_audio(OTH_AUDIO_CRORE);
        #else
        // #if defined(AMR_AUDIO)
        // sdk_audio_play_nv(AUDIO_CRORE+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CRORE));
        // #elif defined(MP3_AUDIO)
        // sdk_audio_play_nv_mp3(AUDIO_CRORE);
        // #endif
        play_audio(AUDIO_CRORE);
        #endif
        
        nAmount = nAmount % CRORE;
    }

    if(nAmount >= LAKH)
    {
        MercuryTrace("Parsing Lakhs---------------------->\r\n");
        tempAmt = nAmount / LAKH;
        MercuryTrace("tempAmt = %d\r\n------------------->", tempAmt);
        
        num_to_audio_999(tempAmt, ln);

        #if defined(DUAL_AUDIO)
        if((unsigned char)0 == ln)
            play_audio(AUDIO_LAKH);
        else
            play_audio(OTH_AUDIO_LAKH);
        #else
        // #if defined(AMR_AUDIO)
        // sdk_audio_play_nv(AUDIO_LAKH+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_LAKH));
        // #elif defined(MP3_AUDIO)
        // sdk_audio_play_nv_mp3(AUDIO_LAKH);
        // #endif
        play_audio(AUDIO_LAKH);
        #endif

        nAmount = nAmount % LAKH;
    }

    
    if(nAmount >= THOUSAND)
    {
        MercuryTrace("Parsing Thousands---------------------->\r\n");
        tempAmt = nAmount / THOUSAND;
        MercuryTrace("tempAmt = %d\r\n------------------->", tempAmt);

        num_to_audio_999(tempAmt, ln);
        
        #if defined(DUAL_AUDIO)
        if((unsigned char)0 == ln)
            play_audio(AUDIO_THOUSAND);
        else
            play_audio(OTH_AUDIO_THOUSAND);
        #else
        // #if defined(AMR_AUDIO)
        // sdk_audio_play_nv(AUDIO_THOUSAND+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_THOUSAND));
        // #elif defined(MP3_AUDIO)
        // sdk_audio_play_nv_mp3(AUDIO_THOUSAND);
        // #endif
        play_audio(AUDIO_THOUSAND);
        #endif
        nAmount = nAmount % THOUSAND;
    }

    MercuryTrace("Parsing Hundreds---------------------->\r\n");
    MercuryTrace("nAmount = %d\r\n------------------->", nAmount);
    num_to_audio_999(nAmount, ln);			// Play numbers below a thousand

    return 0;
}

int confirmation_code_to_audio_mosambee(const char * cnf_code, const unsigned char ln)
{
    if(strlen(cnf_code) == 0)
	    return;

    #if defined(DUAL_AUDIO)
    if((unsigned char)0 == ln)
        play_audio(AUDIO_CNF_CODE_H);
    else
        play_audio(OTH_AUDIO_CNF_CODE_H);
    #else
    // #if defined(AMR_AUDIO)
    // sdk_audio_play_nv(AUDIO_CNF_CODE_H+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CNF_CODE_H));
    // #elif defined(MP3_AUDIO)
    // sdk_audio_play_nv_mp3(AUDIO_CNF_CODE_H);
    // #endif
    play_audio(AUDIO_CNF_CODE_H);
    #endif

    int digit = 0;
    char *p = cnf_code;
    while(*p != 0)
    {
        MercuryTrace("CNF CODE - digit = %d\r\n", (*p-0x30));
        
        #if defined(DUAL_AUDIO)
        if((unsigned char)0 == ln)
            play_audio((*p-0x30));
        else
            play_audio(OTH_AUDIO_000+(*p-0x30));
        play_audio(AUDIO_SEPARATOR);
        #else
        // #if defined(AMR_AUDIO)
        // sdk_audio_play_nv((*p-0x30)+NV_AUDIO_START_INDEX, get_nv_audio_data_len(*p-0x30));
        // sdk_audio_play_nv(AUDIO_SEPARATOR+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SEPARATOR));
        // #elif defined(MP3_AUDIO)
        // sdk_audio_play_nv_mp3(*p-0x30);
        // sdk_audio_play_nv_mp3(AUDIO_SEPARATOR);
        // #endif
        play_audio(*p-0x30);
        play_audio(AUDIO_SEPARATOR);
        #endif

        ++p;
    }
}

int serial_number_to_audio_mosambee(const char * sn)
{
    if(strlen(sn) == 0)
	    return;

    play_audio(AUDIO_DEVICE_SERIAL_NUMBER);

    int digit = 0;
    char *p = sn;
    while(*p != 0)
    {
        MercuryTrace("SN CODE - digit = %d\r\n", (*p-0x30));

        play_audio((*p-0x30));
        // play_audio(AUDIO_SEPARATOR);

        ++p;
    }
}

int sdk_audio_play_nv_mp3(int AUDIO)
{
    int len;
    char *data;

    len = NV_GetItemidDataLen(AUDIO+NV_AUDIO_START_INDEX);

    MercuryTrace("%d\r\n",len);

    data = LocalAlloc(LMEM_ZEROINIT, len+1);

    if (0 != NV_Read(AUDIO+NV_AUDIO_START_INDEX, len, data))
    {
        LocalFree(data);
        return -1;
    }
#if 0
	//同步播放
    sdk_audio_play_data_sync(data, len, AUDIO_TYPE_MP3);
    LocalFree(data);
#else
	//异步播放
	//异步播放不要free内存,SDK播报完会自动free
    return sdk_audio_play_data(data, len, AUDIO_TYPE_MP3);
#endif
    
}

int play_audio(int AUDIO_IDX)
{
    #if defined(AMR_AUDIO)
    {
        sdk_audio_play_nv(AUDIO_IDX+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_IDX));
    }
    #elif defined(MP3_AUDIO)    // MP3
    {
        sdk_audio_play_nv_mp3(AUDIO_IDX);
    }
    #endif
}


int play_amount_mosambee(const unsigned char bReplay)
{
    int INTEGER = 0;
	int DECIMAL = 0;
	sscanf(mqtt_txn_details.amt, "%d.%d", &INTEGER, &DECIMAL);

	MercuryTrace("INTEGER:%d\tDECIMAL:%d\r\n", INTEGER, DECIMAL);
	
	// anfu_led_on();
	// play_notification_sound();


    unsigned char lang = 1;
    if(!strcmp(mqtt_txn_details.ln,"en"))
	    lang = (unsigned char)0;
    else if(!strcmp(mqtt_txn_details.ln,"hi"))
	    lang = (unsigned char)1;
    else if(!strcmp(mqtt_txn_details.ln,"mr"))
	    lang = (unsigned char)2;

    if(bReplay == (unsigned char)1)
        lang = (unsigned char)0;


    if(mqtt_txn_details.notifType == '0' || mqtt_txn_details.notifType == '3') // transaction OR testTransaction
	{
		if(INTEGER > 0 && INTEGER <= 999999)
			sdk_bcs_number(INTEGER, 0);
		else
			sdk_bcs_number(0, 0);

		switch((int)bReplay)
        {
            case 0:

            #if defined(DUAL_AUDIO)            
            if((unsigned char)0 == lang)
                play_audio(AUDIO_RECVD_PAYMENT_OF);
            // else
            //     play_audio(OTH_AUDIO_RECVD_PAYMENT_OF);
            #else
            // #if defined(AMR_AUDIO)
            // // sdk_audio_play_nv(AUDIO_RECVD+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_RECVD));
            // // sdk_audio_play_nv(AUDIO_PAYMENT_OF+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_PAYMENT_OF));
            // sdk_audio_play_nv(AUDIO_RECVD_PAYMENT_OF+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_RECVD_PAYMENT_OF));
            // #elif defined(MP3_AUDIO)
            // sdk_audio_play_nv_mp3(AUDIO_RECVD_PAYMENT_OF);
            // #endif
            play_audio(AUDIO_RECVD_PAYMENT_OF);
            #endif
            
            break;

            case 1: // Last transaction amount is
            #if defined(DUAL_AUDIO)
            if((unsigned char)0 == lang)
                play_audio(AUDIO_LAST_RECVD);
            else
                play_audio(OTH_AUDIO_LAST_RECVD);
            #else
            // #if defined(AMR_AUDIO)
            // sdk_audio_play_nv(AUDIO_LAST_RECVD+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_LAST_RECVD));
            // #elif defined(MP3_AUDIO)
            // sdk_audio_play_nv_mp3(AUDIO_LAST_RECVD);
            // #endif
            play_audio(AUDIO_LAST_RECVD);
            #endif

            break;
            
            default:
            break;
        }

		if(INTEGER > 0)
		{	
			num_to_audio_mosambee(INTEGER, lang);

            #if defined(DUAL_AUDIO)
            if((unsigned char)0 == lang)
                play_audio(AUDIO_RUPEES);
            else
                play_audio(OTH_AUDIO_RUPEES);
            #else
            // #if defined(AMR_AUDIO)
			// sdk_audio_play_nv(AUDIO_RUPEES+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_RUPEES));
            // #elif defined(MP3_AUDIO)
            // sdk_audio_play_nv_mp3(AUDIO_RUPEES);
            // #endif
            play_audio(AUDIO_RUPEES);
            #endif
		}

		if(DECIMAL > 0)
		{
            if(INTEGER > 0)
            {
                #if defined(DUAL_AUDIO)
                if((unsigned char)0 == lang)
                    play_audio(AUDIO_AND);
                else
                    play_audio(OTH_AUDIO_AND);
                #else
                // #if defined(AMR_AUDIO)
                // sdk_audio_play_nv(AUDIO_AND+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_AND));
                // #elif defined(MP3_AUDIO)
                // sdk_audio_play_nv_mp3(AUDIO_AND);
                // #endif
                play_audio(AUDIO_AND);
                #endif
            }

			num_to_audio_mosambee(DECIMAL,lang);

            #if defined(DUAL_AUDIO)
            if((unsigned char)0 == lang)
                play_audio(AUDIO_PAISE);
            else
                play_audio(OTH_AUDIO_PAISE);
            #else
            // #if defined(AMR_AUDIO)
			// sdk_audio_play_nv(AUDIO_PAISE+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_PAISE));
            // #elif defined(MP3_AUDIO)
            // sdk_audio_play_nv_mp3(AUDIO_PAISE);
            // #endif
            play_audio(AUDIO_PAISE);
            #endif
		}

        #if defined(DUAL_AUDIO)
        if((unsigned char)0 == lang)
        {
            play_audio(AUDIO_THANK_YOU);
        }
        else
        {
            play_audio(OTH_AUDIO_RECVD_PAYMENT_OF);
            play_audio(OTH_AUDIO_THANK_YOU);
        }
        #else
        // #if defined(AMR_AUDIO)
		// sdk_audio_play_nv(AUDIO_THANK_YOU+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_THANK_YOU));
        // #elif defined(MP3_AUDIO)
        // sdk_audio_play_nv_mp3(AUDIO_THANK_YOU);
        // #endif
        play_audio(AUDIO_THANK_YOU);
        #endif

	}
    else if(mqtt_txn_details.notifType == '1')
	{
		sscanf(mqtt_txn_details.otp, "%d", &INTEGER);
		
		if(INTEGER > 0 && INTEGER <= 999999)
			sdk_bcs_number(INTEGER, 0);
		else
			sdk_bcs_number(0, 0);

		confirmation_code_to_audio_mosambee(mqtt_txn_details.otp, lang);
	}
	else if(mqtt_txn_details.notifType == '2')
    {
        // #if defined(AMR_AUDIO)
		// sdk_audio_play_nv(AUDIO_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SUCCESS));
        // #elif defined(MP3_AUDIO)
        // sdk_audio_play_nv_mp3(AUDIO_SUCCESS);
        // #endif
        play_audio(AUDIO_SUCCESS);
    }
	

    // anfu_led_off();
}

*/

// ---------------------------------------------------------------------------------- //

int play_audio_by_name(char * file, const unsigned char bSync)
{    
    char filename[32+1] = {0};
    sprintf(filename, "%s%s%s", FS_DRIVE, file, AUDIO_TYPE);

    MercuryTrace(">>>>>>>>>>>>>>>>>>>>>>>>>>>>> Playing Audio File: %s\r\n", filename);
    

    // if((unsigned char)0 == bSync) // async
    //     sdk_audio_play_file(filename);
    // else
    //     sdk_audio_play_file_sync(filename);


    sdk_audio_play_file(filename); // async

}

void play_audio_header(unsigned char bReplay, unsigned char ln, const unsigned char bSync)
{
    switch(ln)
    {
        case EN:
        {
            if((int)bReplay)
                play_audio_by_name("eLRcvd", bSync);
            else
            {
                if(mqtt_txn_details.notifType != '3')
                    play_audio_by_name("eRcvd", bSync);
                else
                    play_audio_by_name("eTRcvd", bSync);
            }
        }
        break;

        case HI:
        {
            if((int)bReplay)
            {
                // play_audio_by_name("hLRcvd", bSync);
                play_audio_by_name("eLRcvd", bSync);
            }
            // else
            // play_audio_by_name("hRcvd", bSync);
        }
        break;

        default:
        break;
    }
}

void play_audio_otp_header(unsigned char ln, const unsigned char bSync)
{
    switch(ln)
    {
        case EN:
        play_audio_by_name("eOTPH", bSync);
        break;

        case HI:
        play_audio_by_name("hOTPH", bSync);
        break;

        default:
        break;
    }
}

void play_audio_footer(unsigned char ln, const unsigned char bSync)
{
    switch((int)ln)
    {
        case EN:
        play_audio_by_name("eCFoot", bSync);
        // play_audio_by_name("eTY", bSync);
        break;

        case HI:
        play_audio_by_name("hRcvd", bSync);
        play_audio_by_name("hTY", bSync);
        break;

        default:
        break;
    }
}

void play_audio_units(int index, unsigned char ln, const unsigned char bSync)
{
    char filename[16]={0};
    memset(filename,0x00,sizeof(filename));

    switch(ln)
    {
        case EN:
        sprintf(filename, "eUnit%d", index);
        break;

        case HI:
        sprintf(filename, "hUnit%d", index);
        break;

        default:
        break;
    }

    play_audio_by_name(filename, bSync);

}

void play_audio_num(int num, const unsigned char ln, const unsigned char bSync)
{
    char filename[16]={0};
    memset(filename,0x00,sizeof(filename));

    switch(ln)
    {
        case EN:
        sprintf(filename, "eNum%d", num);
        break;

        case HI:
        sprintf(filename, "hNum%d", num);
        break;
        
        default:
        break;
    }

    play_audio_by_name(filename, bSync);
}

void num_to_audio_999_by_name(int nAmount, const unsigned char ln, const unsigned char bSync)
{
    // 999
    int tempAmt = nAmount / 100;
    if(tempAmt > 0)
    {
        #if defined(DUAL_AUDIO)
        play_audio_num(tempAmt, ln, bSync);
        play_audio_units(0, ln, bSync);
        #else
        play_audio_num(tempAmt, (unsigned char)0);
        play_audio_units(0, (unsigned char)0);
        #endif
    }

    nAmount = nAmount % 100;

    if((unsigned char)0 == ln)
    {
        if(nAmount > 20)
        {
            tempAmt = nAmount / 10;
            play_audio_num(tempAmt*10, ln, bSync);
            nAmount = nAmount % 10;
        }
    }

    if(nAmount > 0)
    {
        #if defined(DUAL_AUDIO)
        play_audio_num(nAmount, ln, bSync);
        #else
        play_audio_num(nAmount, (unsigned char)0);
        #endif
    }
}

void INTEGER_to_audio_by_name(int amt, unsigned char ln, const unsigned char bSync)
{
    if(amt <= 0)
	    return;

    int amt_bkp = amt;
    int m;
    //play_audio_header(ln);


    if(EN == ln)
    {
        if(amt_bkp > 1)
            play_audio_by_name("eRupees", bSync);
        else
            play_audio_by_name("eRupee", bSync);
    }

    if(amt >= CRORE)		// If it is Crore,
    {
        m = amt / CRORE;
        num_to_audio_999_by_name(m, ln, bSync);		// play the number of more than Ten million first
        play_audio_units(3, ln, bSync);	            // plus the  "Crore"
        amt = amt % CRORE;		                // Remove more than one million of the amount
    }

    if(amt >= LAKH)			// If it is Lakhs,
    {
        m = amt / LAKH;
        num_to_audio_999_by_name(m, ln, bSync);		// play the number of more than One hundred thousand first
        play_audio_units(2, ln, bSync);	            // plus the LAKH
        amt = amt % LAKH;		                // Remove more than One hundred thousand of the amount
    }

    // Determine if it is more than a thousand
    if(amt >= THOUSAND){
        m = amt / THOUSAND;
        num_to_audio_999_by_name(m, ln, bSync);		// play a part of a thousand or more
        play_audio_units(1, ln, bSync);	            // play a thousand
        amt = amt % THOUSAND;
    }

    num_to_audio_999_by_name(amt, ln, bSync);		// Play numbers below a thousand


    if(HI == ln)
        play_audio_by_name("hRupees", bSync);

    // switch(ln)
    // {
    //     case EN:
    //     if(amt_bkp > 1)
    //         play_audio_by_name("eRupees", bSync);
    //     else
    //         play_audio_by_name("eRupee", bSync);
    //     break;
        
    //     case HI:
    //     play_audio_by_name("hRupees", bSync);
    //     break;
        
    //     default:
    //     break;
    // }
}

void DECIMAL_to_audio_by_name(int amt, unsigned char ln, const unsigned char bSync)
{
    if(amt <= 0)
	    return;

    num_to_audio_999_by_name(amt, ln, bSync);	// Play numbers below one hundred

    switch(ln)
    {
        case EN:
        play_audio_by_name("ePaise", bSync);
        break;

        case HI:
        play_audio_by_name("hPaise", bSync);
        break;

        default:
        break;
    }

}

int play_audio_confirmation_code(const char * cnf_code, const unsigned char ln, const unsigned char bSync)
{
    if(strlen(cnf_code) == 0)
	    return;

    #if defined(DUAL_AUDIO)
    play_audio_otp_header(ln, bSync);
    #else
    play_audio_otp_header((unsigned char)0);
    #endif

    int digit = 0;
    char *p = cnf_code;
    char filename[16] = {0};
    while(*p != 0)
    {   
        memset(filename, 0x00, sizeof(filename));
        
        #if defined(DUAL_AUDIO)
        if(ln == EN)
            sprintf(filename,"eNum%c", *p);
        else if(ln == HI)
            sprintf(filename,"hNum%c", *p);
        #else
        sprintf(filename,"eNum%c.mp3", *p);
        #endif

        play_audio_by_name(filename, bSync);
        play_audio_by_name("beep", bSync);

        ++p;
    }
}

int play_audio_serial_number(const char * sn, const unsigned char bSync)
{
    if(strlen(sn) == 0)
	    return;

    play_audio_by_name("snH", bSync);

    int digit = 0;
    char *p = sn;
    char filename[16] = {0};
    while(*p != 0)
    {
        memset(filename, 0x00, sizeof(filename));
	    sprintf(filename,"eNum%c", *p);

        play_audio_by_name(filename, bSync);
        ++p;
    }
}

void play_audio_full_amt(int INTEGER, int DECIMAL, unsigned char ln, const unsigned char bReplay, const unsigned char bSync)
{
    if(INTEGER > 0 || DECIMAL > 0)
        play_audio_header(bReplay, ln, bSync);

    if(INTEGER > 0)
	    INTEGER_to_audio_by_name(INTEGER, ln, bSync);

    if(DECIMAL > 0)
    {
        if(INTEGER> 0)
        {
            if(ln == EN)
                play_audio_by_name("eAnd", bSync);
            else if(ln == HI)
                play_audio_by_name("hAnd", bSync);
        }

    	DECIMAL_to_audio_by_name(DECIMAL, ln, bSync);
    }

    if(INTEGER > 0 || DECIMAL > 0)
        play_audio_footer(ln, bSync);
}

/*
int play_amount_mosambee(const unsigned char bReplay)
{
    int INTEGER = 0;
	int DECIMAL = 0;

	sscanf(mqtt_txn_details.amt, "%d.%d", &INTEGER, &DECIMAL);
	MercuryTrace("INTEGER:%d\tDECIMAL:%d\r\n", INTEGER, DECIMAL);
	
	// anfu_led_on();
	// play_notification_sound();

    unsigned char lang = (unsigned char)0;
    if(!strcmp(mqtt_txn_details.ln,"en"))
	    lang = (unsigned char)0;
    else if(!strcmp(mqtt_txn_details.ln,"hi"))
	    lang = (unsigned char)1;
    else if(!strcmp(mqtt_txn_details.ln,"mr"))
	    lang = (unsigned char)2;

    if(bReplay == (unsigned char)1)
        lang = (unsigned char)0;


    if(mqtt_txn_details.notifType == '0' || mqtt_txn_details.notifType == '3') // transaction OR testTransaction
	{
		if(INTEGER > 0 && INTEGER <= 999999)
        {
            // display speaker
            qyy_bcs_set_speaker(TRUE);

            sdk_bcs_number(INTEGER, 0);
            // start_bcs_clear_timer();
        }
		else
			sdk_bcs_number(0, 0);

		// sscanf(mqtt_txn_details.amt, "%d.%d", &INTEGER, &DECIMAL);
	    // MercuryTrace("INTEGER:%d\tDECIMAL:%d\r\n", INTEGER, DECIMAL);    

        play_audio_full_amt(INTEGER, DECIMAL, lang, bReplay);

	}
    else if(mqtt_txn_details.notifType == '1')
	{
		sscanf(mqtt_txn_details.otp, "%d", &INTEGER);
		
		if(INTEGER > 0 && INTEGER <= 999999)
			sdk_bcs_number(INTEGER, 0);
		else
			sdk_bcs_number(0, 0);

		play_audio_confirmation_code(mqtt_txn_details.otp, lang);
	}
	else if(mqtt_txn_details.notifType == '2')
    {
        play_audio_by_name("pairCmp");
    }
	
    // anfu_led_off();
}
*/

int play_amount_mosambee(const unsigned char bReplay, const unsigned char bSync)
{
    int INTEGER = 0;
	int DECIMAL = 0;

    float total_amt = 0.0;

    sscanf(mqtt_txn_details.amt, "%d.%d", &INTEGER, &DECIMAL);

    // if(DECIMAL < 10)
    //     DECIMAL *= 10;
    
	MercuryTrace("INTEGER:%d\tDECIMAL:%d\r\n", INTEGER, DECIMAL);
	
    // sscanf(mqtt_txn_details.amt, "%.2f", &total_amt);
	// MercuryTrace("Total Amount:%f\r\n", total_amt);

	// anfu_led_on();
	// play_notification_sound();

    unsigned char lang = (unsigned char)0;
    if(!strcmp(mqtt_txn_details.ln,"en"))
	    lang = (unsigned char)0;
    else if(!strcmp(mqtt_txn_details.ln,"hi"))
	    lang = (unsigned char)1;
    else if(!strcmp(mqtt_txn_details.ln,"mr"))
	    lang = (unsigned char)2;

    if(bReplay == (unsigned char)1)
        lang = (unsigned char)0;


    if(mqtt_txn_details.notifType == '0' || mqtt_txn_details.notifType == '3') // transaction OR testTransaction
	{
		// if(INTEGER > 0 && INTEGER <= 999999)
        // {
        //     // display speaker
        //     qyy_bcs_set_speaker(TRUE);

        //     sdk_bcs_number(INTEGER, 0);
        //     // start_bcs_clear_timer();
        // }
		// else
		// 	sdk_bcs_number(0, 0);

        // display speaker
        qyy_bcs_set_speaker(TRUE);

        if(INTEGER < 10000)
        {
            // display speaker
            // qyy_bcs_set_speaker(TRUE);

            // sdk_bcs_number((int)(total_amt*100.00), 2);
            sdk_bcs_number(INTEGER*100+DECIMAL, 2);
            

            // start_bcs_clear_timer();
        }
        else if(INTEGER > 9999 && INTEGER < 100000)
        {
            // qyy_bcs_set_speaker(TRUE);
            // sdk_bcs_number((int)(total_amt*10.00), 1);
            sdk_bcs_number((INTEGER*10)+(DECIMAL/10), 1);
        }
		else if(INTEGER > 99999 && INTEGER < 1000000)
        {
			// qyy_bcs_set_speaker(TRUE);
            // sdk_bcs_number((int)total_amt, 0);
            sdk_bcs_number(INTEGER, 0);
        }

		// sscanf(mqtt_txn_details.amt, "%d.%d", &INTEGER, &DECIMAL);
	    // MercuryTrace("INTEGER:%d\tDECIMAL:%d\r\n", INTEGER, DECIMAL);    

        play_audio_full_amt(INTEGER, DECIMAL, lang, bReplay, bSync);

        if((unsigned char)0 == bReplay) // MOGO
        {
            Sleep(250);
            play_audio_by_name("mogo", bSync);
        }

	}
    else if(mqtt_txn_details.notifType == '1')
	{
		sscanf(mqtt_txn_details.otp, "%d", &INTEGER);
		
		if(INTEGER > 0 && INTEGER <= 999999)
			sdk_bcs_number(INTEGER, 0);
		else
			sdk_bcs_number(0, 0);

		play_audio_confirmation_code(mqtt_txn_details.otp, lang, bSync);
	}
	else if(mqtt_txn_details.notifType == '2')
    {
        play_audio_by_name("pairCmp", bSync);
    }
	
    // anfu_led_off();
}

// ---------------------------------------------------------------------------------- //






char *get_str_from_list(char * strList, char * sSep, int nIndex) {
    int nPos = 0;
    char * str;
    char * p = strList;
    while (1)
    {
        str = strtok(p, sSep);
        if ((str == NULL) || (strlen(str) == 0))
            return NULL;
        if (nIndex == nPos)
            break; 
        nPos++;
        p = NULL;
    }
    return str;
}


unsigned char * revstr( unsigned char * sn)
{
    int i, j, len;
    len = strlen(sn);
    if(0 == len)
        return NULL;

    unsigned char temp = 0;
    for(i = 0, j = len-1; i < len/2 ; ++i, --j)
    {
        temp = sn[i];
        sn[i] = sn[j];
        sn[j] = temp;
    }
    
    return sn;
}

char * generatePassword(const char * sn)
{
    char * final_pwd = malloc(48 +1);
    memset(final_pwd, 0x00, (48 +1));

    char sn_dup[20+1] = {0};
    strcpy(sn_dup, sn);
    sprintf(final_pwd, "Sound@$%s%%^Box", (char *)revstr(sn_dup));

    MercuryTrace("MQTT Prod PWD: %s\r\n", final_pwd);

    return final_pwd;
}

int num_to_audio_index_array(int nNumber, int index_array[20], int *res_num)
{
    int nDigit = 0;
    int nDigitPos = 0; // 当前处理的位数，偏移量，从0开始
    int array[10] = {0};
	int get_num = 0;
	int total_num = 0;
	int unit;
	
    while (1) {
    // {
        nDigit = nNumber % 1000;
        nNumber = nNumber / 1000;

        // 填数字：几百几十几
        if (nDigit)
        {
			prefix_three_digits_audio_index(nDigit, &array, &get_num);
	        MercuryTrace("after prefix_three_digits_audio_index nDigitPos=%d,get_num:%d \r\n", nDigitPos, get_num);
			total_num += get_num;

			memcpy(&index_array[19-total_num], array, get_num*4);
        }			
        // 已处理完3位
        nDigitPos += 3;
        // 剩余数字
        if (nNumber == 0)
            break;

        MercuryTrace("before prefix_unit nDigitPos=%d total_num=%d \r\n", nDigitPos, total_num);
        // 获取单位，并添加到数字前
        get_unit_audio_index(nDigitPos, &unit);
		total_num++;
		index_array[19-total_num] = unit;
        MercuryTrace("after prefix_unit nDigitPos=%d unit=%d \r\n", nDigitPos, unit);
    }

	memmove(index_array, &index_array[19-total_num], total_num*4);
	*res_num = total_num;
    return 0;
}

void test_audio_array_conv(void)
{
	int array[20];
	int num;
	num_to_audio_index_array(12345, array, &num);
	for(int i=0;i<num;i++)
	{
		MercuryTrace("i:%d, array[i]:%d\n", i, array[i]);
	}
}

