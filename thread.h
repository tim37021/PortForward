#ifndef PF_THREAD_H_
#define PF_THREAD_H_

#if WIN32
#include <windows.h>
typedef HANDLE thread_handle_t;
#define DECL_TASK(name) DWORD WINAPI name(LPVOID param)
#define EXIT_TASK() return 0;
#else
#include <pthread.h>
typedef pthread_t thread_handle_t;
#define DECL_TASK(name) void *name(void *param)
#define EXIT_TASK() return NULL;
#endif

void thread_create(thread_handle_t *handle, void *task, void *param);
void thread_join(thread_handle_t handle);
void close_handle(thread_handle_t handle);
void thread_sleep(long milisec);

#endif
