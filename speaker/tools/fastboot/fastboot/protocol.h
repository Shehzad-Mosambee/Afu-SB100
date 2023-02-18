#pragma once
#include "engine.h"
#include "ICommChannel.h"


typedef struct _tagMercuryHeader {
	BYTE	Signature[256];
	BYTE	Name[48];
	BYTE	Company[48];
	DWORD	VersionHi;
	DWORD	VersionMi;
	DWORD	VersionLo;
	DWORD	Flag1;
	DWORD	Flag2;
	DWORD	LoadAddress;
	DWORD	ExecuteAddress;
	DWORD	ProgramLen;
}MERCURY_HEADER;

char *fb_get_error(void);

int fb_command(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size);
int fb_downlad(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size);
int fb_push(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const char *path, const void *data, unsigned int size);
int fb_pull(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const char *path, const void *data, unsigned int size);
int fb_fileSys(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size);
int fb_nvEdite(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *info, const void *data, unsigned int size);
int fb_writeSign(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, char *type, unsigned int size);
int fb_downlad_img(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size);
int fb_read_img(ICommChannel* m_pProChan, MC_FASTBOOT_CMD_E cmd, const void *data, unsigned int size, const void *Path);