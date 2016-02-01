#include "thread.h"

void thread_create(thread_handle_t *handle, void *task, void *param)
{
#if WIN32
	*handle = CreateThread(NULL, 0, (DWORD WINAPI (*)(LPVOID param))task, param, 0, NULL);
#else
	pthread_create(handle, NULL, task, param);
#endif
}
void thread_join(thread_handle_t handle)
{
#if WIN32
	WaitForSingleObject(handle, INFINITE);
#else
	pthread_join(handle);
#endif
}

void close_handle(thread_handle_t handle)
{
#if WIN32
	CloseHandle(handle);
#endif
}

void thread_sleep(long milisec)
{
#if WIN32
	Sleep(milisec);
#else
	usleep(milisec*1000);
#endif
}