#include "stdafx.h"
#include "engine.h"

#include "protocol.h"

#define OP_DOWNLOAD			 1
#define OP_COMMAND			 2
#define OP_PUSH				 3
#define OP_PULL				 4
#define OP_FILECMD			 5
#define OP_NVEDITE			 6
#define OP_WRITESIGN		 7
#define OP_IMG				 8
#define OP_READIMG			 9

typedef struct Action Action;
struct Action
{
	Action *next;
	uint32_t op;
	MC_FASTBOOT_CMD_E cmd;

	void *data;
	unsigned int size;
	char *path;
	char *ptn;
	const char *msg;

	int(*func)(Action *a, int status, char *resp);
	double start;
};

static Action *action_list = NULL;
static Action *action_last = NULL;



void die(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "error: ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
	exit(1);
}
char *mkmsg(const char *fmt, ...)
{
	char buf[256];
	char *s;
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	s = strdup(buf);
	if (s == 0) die("out of memory");
	return s;
}

int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;


	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;


	return (0);
}
double now()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}



static int cb_default(Action *a, int status, char *resp)
{
	if (status) {
		fprintf(stderr, "FAILED (%s)\n", resp);
	}
	else {
		//double split = now();
		//fprintf(stderr, "OKAY [%7.3fs]\n", (split - a->start));
		//a->start = split;
		fprintf(stderr, "OKAY\n");
	}
	return status;
}

static Action *queue_action(unsigned op, MC_FASTBOOT_CMD_E cmd)
{
	Action *a;
	
	size_t cmdsize;

	a = (Action *)calloc(1, sizeof(Action));
	if (a == 0) die("out of memory");

	a->cmd = cmd;
	
	if (action_last) {
		action_last->next = a;
	}
	else {
		action_list = a;
	}
	action_last = a;
	a->op = op;
	a->func = cb_default;

	a->start = -1;

	return a;
}
void fb_queue_command(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size)
{
	Action *a = queue_action(OP_COMMAND, cmd);
	a->msg = msg;
	a->data = (void*)data;
	a->size = size;
}

void fb_queue_img(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size)
{
	Action *a = queue_action(OP_IMG, cmd);
	a->msg = msg;
	a->data = (void*)data;
	a->size = size;
}
void fb_queue_readLogoPart(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size, const char *path)
{

	Action *a = queue_action(OP_READIMG, cmd);
	a->msg = msg;
	a->data = (void*)data;
	a->size = size;
	a->path = (char*)path;
}
void fb_queue_nvEdite(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *info, const char *data, unsigned int size)
{
	Action *a = queue_action(OP_NVEDITE, cmd);
	a->msg = msg;
	a->ptn = (char*)info;
	a->data = (void*)data;
	a->size = size;
}

void fb_queue_fileSys(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size)
{
	Action *a = queue_action(OP_FILECMD, cmd);
	a->msg = msg;
	a->data = (void*)data;
	a->size = size;
}

void fb_queue_download(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, unsigned int size)
{
	Action *a = queue_action(OP_DOWNLOAD, cmd);
	a->msg = msg;
	a->data = (void*)data;
	a->size = size;
}
void fb_queue_writeSign(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *data, char *type, unsigned int size)
{
	Action *a = queue_action(OP_WRITESIGN, cmd);
	a->ptn = type;
	a->msg = msg;
	a->data = (void*)data;
	a->size = size;
}
void fb_queue_push(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *path, const char *data, unsigned int size)
{
	Action *a = queue_action(OP_PUSH, cmd);
	a->msg = msg;
	a->data = (void*)data;
	a->size = size;
	a->path = (char*)path;
}
void fb_queue_pull(MC_FASTBOOT_CMD_E cmd, const char *msg, const char *path, const char *data, unsigned int size)
{
	Action *a = queue_action(OP_PULL, cmd);
	a->msg = msg;
	a->size = size;
	a->path = (char*)path;
	a->ptn = (char*)data;
}

int fb_execute_queue(ICommChannel* m_pProChan)
{
	Action *a;

	int status = 0;
	double start = -1;
	a = action_list;

	for (a = action_list; a; a = a->next)
	{
		a->start = now();
		if (start < 0) start = a->start;

		if (a->msg) {
			fprintf(stderr, "%s...\n", a->msg);
		}

		if (a->op == OP_COMMAND) {
			status = fb_command(m_pProChan, a->cmd, a->data, a->size);
			status = a->func(a, status, status ? fb_get_error() : "");
			if (status) break;
		}
		if (a->op == OP_DOWNLOAD)
		{
			status = fb_downlad(m_pProChan, a->cmd, a->data, a->size);
			status = a->func(a, status, status ? fb_get_error() : "");
			if (status) break;
		}
		if (a->op == OP_PUSH)
		{
			status = fb_push(m_pProChan, a->cmd, a->path, a->data, a->size);
			status = a->func(a, status, status ? fb_get_error() : "");
			if (status) break;
		}
		if (a->op == OP_PULL)
		{
			status = fb_pull(m_pProChan, a->cmd, a->path, a->ptn, a->size);
			status = a->func(a, status, status ? fb_get_error() : "");
			if (status) break;
		}
		if (a->op == OP_FILECMD)
		{
			status = fb_fileSys(m_pProChan, a->cmd, a->data, a->size);
			status = a->func(a, status, status ? fb_get_error() : "");
			if (status) break;
		}
		if (a->op == OP_NVEDITE)
		{
			status = fb_nvEdite(m_pProChan, a->cmd, a->ptn, a->data, a->size);
			status = a->func(a, status, status ? fb_get_error() : "");
			if (status) break;
		}

		if (a->op == OP_WRITESIGN)
		{
			status = fb_writeSign(m_pProChan, a->cmd, a->data, a->ptn, a->size);
			status = a->func(a, status, status ? fb_get_error() : "");
			if (status) break;
		}
		if (a->op == OP_IMG) {
			status = fb_downlad_img(m_pProChan, a->cmd, a->data, a->size);
			status = a->func(a, status, status ? fb_get_error() : "");
			if (status) break;
		}
		if (a->op == OP_READIMG) {
			
			status = fb_read_img(m_pProChan, a->cmd, a->data, a->size, a->path);
			status = a->func(a, status, status ? fb_get_error() : "");
			if (status) break;
		}
	}
	fprintf(stderr, "finished. total time: %.3fs\n", (now() - start));
	return status;
}