#ifndef _PF_TYPE_H_
#define _PF_TYPE_H_

#include "network.h"
#include "thread.h"

enum Status {STATUS_NOT_YET, STATUS_READY, STATUS_ERROR};

struct terminal_t
{
	const char *ip;
	int port;
	enum Status status;
	SOCKET fd;
};

struct session_t
{
	struct terminal_t term[2];
};

#define THE_OTHER(t) (!(t))

#endif

