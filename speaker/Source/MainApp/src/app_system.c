#include "MercuryDef.h"
#include "MercuryPlat.h"

void Dbg_dump(const unsigned char *buff, uint32_t len)                                                                       
{
	int index = 0;

	for(index=0; index<len; index++){
		MercuryTrace("%02x ", *(buff + index) & 0xff );
	}   
	MercuryTrace("\n");

	return;
}

uint32 CharString2UnicodeString(char * destination, const char * string)
{
    uint32 length = 0;

    if (string != NULL)
    {
        while (*string)
        {
            *destination++ = *string++;
			*destination++ = '\0';
            length += 2;
        }
    }
    *destination++ = '\0';
    *destination++ = '\0';
    length += 2;

    return length;
}


void app_hex2str( unsigned char *source, unsigned int source_len, unsigned char *dst )
{
	char *zEncode = "0123456789ABCDEF";
	int i = 0, j = 0;
	
	for( i = 0; i < source_len; i++ )
	{
		dst[j++] = zEncode[(source[i] >> 4) & 0x0f];
		dst[j++] = zEncode[(source[i]) & 0x0f];
	}
}