#ifndef _PF_NETWORK_H_
#define _PF_NETWORK_H_
#if WIN32

#include <winsock2.h>
#include <windows.h>
#define startup_network() {WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);}
#define cleanup_network() WSACleanup()
#define INET_ADDRSTRLEN 16

void inet_ntop(int af, const void *src, char *dst, int size);
#else
// POSIX
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define startup_network() ;
#define cleanup_network() ;
#define SOCKET int
#define closesocket(a) {shutdown(a, SHUT_RDWR); close(a);}
#define INVALID_SOCKET -1


// TODO: Mac OS X
#endif
#endif

