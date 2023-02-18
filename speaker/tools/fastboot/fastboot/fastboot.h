#pragma once

#include "resource.h"
#include "ICommChannel.h"
#include "engine.h"


const UINT32 vds_crc32_table[256] =
{
	0x00000000L, 0x77073096L, 0x0ee0e612cL, 0x990951baL,
	0x076dc419L, 0x706af48fL, 0x0e963a535L, 0x9e6495a3L,
	0x0edb8832L, 0x79dcb8a4L, 0x0e0d5e91eL, 0x97d2d988L,
	0x09b64c2bL, 0x7eb17cbdL, 0x0e7b82d07L, 0x90bf1d91L,
	0x1db71064L, 0x6ab020f2L, 0x0f3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0x0f4d4b551L, 0x83d385c7L,
	0x136c9856L, 0x646ba8c0L, 0x0fd62f97aL, 0x8a65c9ecL,
	0x14015c4fL, 0x63066cd9L, 0x0fa0f3d63L, 0x8d080df5L,
	0x3b6e20c8L, 0x4c69105eL, 0x0d56041e4L, 0xa2677172L,
	0x3c03e4d1L, 0x4b04d447L, 0x0d20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0x0dbbbc9d6L, 0xacbcf940L,
	0x32d86ce3L, 0x45df5c75L, 0x0dcd60dcfL, 0xabd13d59L,
	0x26d930acL, 0x51de003aL, 0x0c8d75180L, 0xbfd06116L,
	0x21b4f4b5L, 0x56b3c423L, 0x0cfba9599L, 0xb8bda50fL,
	0x2802b89eL, 0x5f058808L, 0x0c60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0x0c1611dabL, 0xb6662d3dL,
	0x76dc4190L, 0x01db7106L, 0x098d220bcL, 0xefd5102aL,
	0x71b18589L, 0x06b6b51fL, 0x09fbfe4a5L, 0xe8b8d433L,
	0x7807c9a2L, 0x0f00f934L, 0x09609a88eL, 0xe10e9818L,
	0x7f6a0dbbL, 0x086d3d2dL, 0x091646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x0856530d8L, 0xf262004eL,
	0x6c0695edL, 0x1b01a57bL, 0x08208f4c1L, 0xf50fc457L,
	0x65b0d9c6L, 0x12b7e950L, 0x08bbeb8eaL, 0xfcb9887cL,
	0x62dd1ddfL, 0x15da2d49L, 0x08cd37cf3L, 0xfbd44c65L,
	0x4db26158L, 0x3ab551ceL, 0x0a3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0x0a4d1c46dL, 0xd3d6f4fbL,
	0x4369e96aL, 0x346ed9fcL, 0x0ad678846L, 0xda60b8d0L,
	0x44042d73L, 0x33031de5L, 0x0aa0a4c5fL, 0xdd0d7cc9L,
	0x5005713cL, 0x270241aaL, 0x0be0b1010L, 0xc90c2086L,
	0x5768b525L, 0x206f85b3L, 0x0b966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0x0b0d09822L, 0xc7d7a8b4L,
	0x59b33d17L, 0x2eb40d81L, 0x0b7bd5c3bL, 0xc0ba6cadL,
	0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
	0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
	0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
	0xf00f9344L, 0x8708a3d2L, 0x01e01f268L, 0x6906c2feL,
	0xf762575dL, 0x806567cbL, 0x0196c3671L, 0x6e6b06e7L,
	0xfed41b76L, 0x89d32be0L, 0x010da7a5aL, 0x67dd4accL,
	0xf9b9df6fL, 0x8ebeeff9L, 0x017b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x038d8c2c4L, 0x4fdff252L,
	0xd1bb67f1L, 0xa6bc5767L, 0x03fb506ddL, 0x48b2364bL,
	0xd80d2bdaL, 0xaf0a1b4cL, 0x036034af6L, 0x41047a60L,
	0xdf60efc3L, 0xa867df55L, 0x0316e8eefL, 0x4669be79L,
	0xcb61b38cL, 0xbc66831aL, 0x0256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x0220216b9L, 0x5505262fL,
	0xc5ba3bbeL, 0xb2bd0b28L, 0x02bb45a92L, 0x5cb36a04L,
	0xc2d7ffa7L, 0xb5d0cf31L, 0x02cd99e8bL, 0x5bdeae1dL,
	0x9b64c2b0L, 0xec63f226L, 0x0756aa39cL, 0x026d930aL,
	0x9c0906a9L, 0xeb0e363fL, 0x072076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x07bb12baeL, 0x0cb61b38L,
	0x92d28e9bL, 0xe5d5be0dL, 0x07cdcefb7L, 0x0bdbdf21L,
	0x86d3d2d4L, 0xf1d4e242L, 0x068ddb3f8L, 0x1fda836eL,
	0x81be16cdL, 0xf6b9265bL, 0x06fb077e1L, 0x18b74777L,
	0x88085ae6L, 0xff0f6a70L, 0x066063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x0616bffd3L, 0x166ccf45L,
	0xa00ae278L, 0xd70dd2eeL, 0x04e048354L, 0x3903b3c2L,
	0xa7672661L, 0xd06016f7L, 0x04969474dL, 0x3e6e77dbL,
	0xaed16a4aL, 0xd9d65adcL, 0x040df0b66L, 0x37d83bf0L,
	0xa9bcae53L, 0xdebb9ec5L, 0x047b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x053b39330L, 0x24b4a3a6L,
	0xbad03605L, 0xcdd70693L, 0x054de5729L, 0x23d967bfL,
	0xb3667a2eL, 0xc4614ab8L, 0x05d681b02L, 0x2a6f2b94L,
	0xb40bbe37L, 0xc30c8ea1L, 0x05a05df1bL, 0x2d02ef8dL,
};

typedef enum
{
	FILESYS_CAPACITY_1M = 1,
	FILESYS_CAPACITY_2M,
	FILESYS_CAPACITY_3M,
	FILESYS_CAPACITY_4M,
	FILESYS_CAPACITY_5M,
	FILESYS_CAPACITY_6M,
	FILESYS_CAPACITY_7M,
	FILESYS_CAPACITY_8M,
	FILESYS_CAPACITY_9M,
	FILESYS_CAPACITY_10M,
	FILESYS_CAPACITY_11M,
	FILESYS_CAPACITY_12M,
	FILESYS_CAPACITY_13M,
	FILESYS_CAPACITY_14M,
	FILESYS_CAPACITY_15M,
	FILESYS_CAPACITY_16M,
	FILESYS_CAPACITY_MAX,
	MERCURY_ENUM_FILESYS_MAX = 0x7fffffff
}FILESYS_CAPACITY_E;

typedef struct
{
	char					    point[4];
	UINT32 						startAddr;
	UINT8						fileSysCap;
	UINT8			 			spiFlashCap;
}MC_FILESYS_INFO;

typedef struct
{
	char disk_sym;
	uint32_t start_add;
	uint32_t size;
	uint32_t flashType;
	uint32_t ioLevel;
	uint32_t powerLevel;
} MARS_FS_SET_T;

typedef struct
{
	char					    point[4];
	char						password[16];
}MC_FILESYS_FORMAT;

typedef struct
{
	int							type;
	UINT32						dmaFlag;
	UINT32						baud;
}MC_FILESYS_CONFIG;

#define RDASIGN     "RDAS"
#define PKALG       "Ed"
#define KDFALG      "BK"

#define SIGBYTES    64
#define PUBLICBYTES 32
#define SECRETBYTES 64
#define FPLEN        8

struct enckey {
	uint8_t rdasign[4];//RDAS
	uint8_t pkalg[2];  //Ed
	uint8_t kdfalg[2]; //BK
	uint32_t kdfrounds;
	uint8_t salt[16];
	uint8_t checksum[8];
	uint8_t name[16];
	uint8_t fingerprint[FPLEN];
	uint8_t sk[SECRETBYTES];
};

typedef struct enckey RDA_ENC_KEY;

struct pubkey {
	uint8_t rdasign[4];//RDAS
	uint8_t pkalg[2];  //Ed
	uint8_t dummy[2];
	uint8_t name[16];
	uint8_t fingerprint[FPLEN];
	uint8_t pk[PUBLICBYTES];
};
typedef struct pubkey RDA_PUBLIC_KEY;

// RDASEdPo
struct sig {
	uint8_t rdasign[4];//RDAS
	uint8_t pkalg[2];  //Ed
	uint8_t hashalg[2];//Po/B2/SH
	uint8_t name[16];
	uint8_t fingerprint[FPLEN];
	uint8_t signature[SIGBYTES];
};
typedef struct sig RDA_SIGNATRUE;


typedef struct RDA_KEY
{
	RDA_ENC_KEY enc_key;
	RDA_PUBLIC_KEY pub_key;
}RDA_KEY_PAIR_T;

typedef struct
{
	UINT32 magicNum;
	UINT16 left;
	UINT16 top;
	UINT16 width;
	UINT16 height;
	UINT32 imageLen;
	UINT32 lcdTypeIndex;
	UINT8 bootloaderLogoFinish;
	UINT8 backLightLevel;
	UINT8 powerGpioNo;
	UINT8 gpioLevel;
	UINT16 LCDIOLevel;
	UINT8 LCDClk;
}MERCURY_BOOT_IMAGE_S;

typedef struct
{
	UINT32 magicNum;
	UINT16 left;
	UINT16 top;
	UINT16 width;
	UINT16 height;
	UINT32 imageLen;
}MERCURY_SHOW_FOTA_INFO_S;

typedef struct
{
	UINT32 secureFlagRSA;
	UINT8 pubkeyRSA_N[512];
	UINT8 pubkeyRSA_E[8];
	UINT32 reserve;
}MERCURY_SECURERSA_S;

typedef void(*DLL_SIGN_FUNC)(struct enckey *enckey, char *msg, unsigned long long msglen, struct sig *sig);
typedef int(*DLL_VERITY_FUNC)(struct pubkey *pubkey, char *msg, unsigned long long msglen, struct sig *sig);
typedef void(*DLL_GENERATE_KEY_PAIRN_FUNC)(RDA_PUBLIC_KEY *pubkey, RDA_ENC_KEY *enckey);

typedef BOOL(*pfCreateChannel)(ICommChannel **, CHANNEL_TYPE);
typedef void(*pfReleaseChannel)(ICommChannel *);

unsigned int ReadDataFromPort(unsigned char *Buffer, unsigned int BytesToBeRead, ICommChannel* m_pProChan);
unsigned int ReadDataFromPort2(unsigned char *Buffer, unsigned int BytesToBeRead, ICommChannel* m_pProChan, DWORD dwTimeOut);
void WriteDataToPort(unsigned char *Buffer, unsigned int BytesToBeWritten, ICommChannel* m_pProChan);
void ClosePort(ICommChannel* m_pProChan);
void portClear(ICommChannel* m_pProChan);
