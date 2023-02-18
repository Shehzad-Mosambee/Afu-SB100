#ifndef _QYY_API_FOTA_H
#define _QYY_API_FOTA_H

int sdk_fota_download_http( const char *url );

int sdk_fota_download_http_ex( const char *url, char *userhead,unsigned int file_len );

int sdk_fota_download_http_plain( const char *url, char *body, unsigned int file_len);

 typedef enum
{
	ALGORITHMS_TYPE_NONE   = 0,  
	ALGORITHMS_TYPE_MD5    = 1,  
	ALGORITHMS_TYPE_SHA256 = 2,  
}ALGORITHMS_TYPE_E;
int sdk_fota_update( const unsigned char *checkvalue, ALGORITHMS_TYPE_E type );

int sdk_http_post( unsigned long handle, unsigned char *url, unsigned char *body, 
                        unsigned char *authorization, unsigned char *content_type, 
                        unsigned char *user_agent, unsigned char *response, unsigned int timeout );

int sdk_http_post_auto(unsigned char* url, unsigned char* body, 
                        unsigned char* content_type, 
                        unsigned char *response,
                        unsigned int buff_len,
						unsigned int *recv_len);

int sdk_http_get(unsigned char* url, unsigned char* body, 
                        unsigned char* content_type, 
                        unsigned char *response,
                        unsigned int buff_len,
						unsigned int *recv_len);



/*****
 * Reads data from logo partition.
 *
 * arg-addr     : Read data start from the address.
 *                (0 ~ 160*1024)
 * arg-buf      : Pointer to a buffer to storage the data.
 * arg-read_len : The actual length, in bytes, of the data that want to read from the patition.
 *
 * RETURN       : On success, zero is returned.
 *
 * WARNING      : The sum of addr and read_len cannot greater than 160*1024.
 */
int sdk_flash_logo_read(uint32 addr, uint8* buf, uint32 read_len);



/*****
 * Write data into logo partition.
 *
 * arg-addr     : Wead data start from the address.
 *                (0 ~ 160*1024)
 * arg-buf      : The buffer for data to be written.
 * arg-write_len: The length, in bytes, of data that will be written into logo partition.
 *
 * RETURN       : On success, zero is returned.
 *
 * WARNING      : The sum of addr and read_len cannot greater than 160*1024.
 */
int sdk_flash_logo_write(uint32 addr, uint8* buf, uint32 write_len);



/*****
 * Erases data the logo partition.
 *
 * arg-addr     : Erase data from this address, 32*1024 bytes a time.
 *                The values of addr can't be greater than (160-32)*1024, 
 *                and the addr must be exactly 32K times or equal to zero.
 *
 * RETURN       : On success, zero is returned.
 *
 * WARNING      : The sum of addr and read_len cannot greater than 160*1024.
 *
 * NOTE         : Fill with 0xff.
 *
 */
int sdk_flash_logo_erase(uint32 addr);

void sdk_set_ota_mode(int mode, int type);


#endif
