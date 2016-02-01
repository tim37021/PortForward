#include <stdio.h>
#include "network.h"

#if WIN32
inline int is_little_endian()
{
	int dummy = 1;
	return *((unsigned char *)&dummy);
}

// To work on multiple versions of windows, we need to do this
void inet_ntop(int af, const void *src, char *dst, int size)
{
	if(af == AF_INET)
	{
		const unsigned char *src_ = (const unsigned char *)src;
		if(is_little_endian())
			snprintf(dst, size, "%d.%d.%d.%d", src_[0], src_[1], src_[2], src_[3]);
		else
			snprintf(dst, size, "%d.%d.%d.%d", src_[3], src_[2], src_[1], src_[0]);
	}
	if(af == AF_INET6)
	{
		// TODO
	}
}

#endif

