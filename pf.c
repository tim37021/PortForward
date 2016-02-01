#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "pf.h"

#define BUFFER_SIZE 81920
#define LISTEN_BAG 5

struct session_t session;
int quiet = 0;
int on_demand = 0;

int parse_args(int argc, char *argv[])
{
	int i;
	int t=0;
	for(i=0; argv[i]; i++)
	{
		if(strcmp(argv[i], "-l")==0)
		{
			if(i+1>=argc)
				return 0;
			session.term[t].ip = NULL;
			session.term[t].port = atoi(argv[i+1]);
			t++;
		}
		if(strcmp(argv[i], "-c")==0)
		{
			if(i+2>=argc)
				return 0;
			session.term[t].ip = argv[i+1];
			session.term[t].port = atoi(argv[i+2]);
			t++;
		}
		if(strcmp(argv[i], "-q")==0)
			quiet = 1;
		if(strcmp(argv[i], "-d")==0)
			on_demand = 1;
	}
	return t==2;
}

void print_msg(const char *msg, ...)
{
	va_list args;
	va_start (args, msg);
	if(!quiet)
	{
		vprintf(msg, args);
		putchar('\n');
	}
	va_end(args);
}

SOCKET start_server(int port)
{

	struct sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(port);
	service.sin_addr.s_addr = INADDR_ANY;

	SOCKET lis_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(lis_sock == INVALID_SOCKET)
		return INVALID_SOCKET;

	if(bind(lis_sock, (const struct sockaddr *)&service, sizeof(service))==-1)
	{
		closesocket(lis_sock);
		print_msg("Cannot bind on port %d", port);
		return INVALID_SOCKET;
	}

	print_msg("Listen on port %d", port);
	if(listen(lis_sock, LISTEN_BAG)==-1)
	{
		closesocket(lis_sock);
		print_msg("Cannot listen on port %d", port);
		return INVALID_SOCKET;
	}
	int addrlen = sizeof(service);
	SOCKET client = accept(lis_sock, (struct sockaddr *)&service, &addrlen);
	char ip[24];
	inet_ntop(AF_INET, &service.sin_addr, ip, INET_ADDRSTRLEN);
	print_msg("Connected from %s", ip);
	closesocket(lis_sock);
	return client;

}
SOCKET start_client(const char *ip, int port)
{
	struct sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(port);
	service.sin_addr.s_addr = inet_addr(ip);
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(connect(sock, (const struct sockaddr *)&service, sizeof(service))==-1)
	{
		closesocket(sock);
		return INVALID_SOCKET;
	}
	print_msg("Connected to %s", ip);

	return sock;
}

void data_forward(SOCKET fd_in, SOCKET fd_out)
{
	int len;
	char *buf = malloc(BUFFER_SIZE);
	while((len=recv(fd_in, buf, BUFFER_SIZE, 0))>0)
	{
		if(send(fd_out, buf, len, 0)<=0)
			break;
	}

	free(buf);
}

void init_term(struct terminal_t *term)
{
	term->ip = NULL;
	term->port = 0;
	term->status = STATUS_NOT_YET;
}

DECL_TASK(task)
{
	int term_id = *(int *)param;

	session.term[term_id].status = STATUS_NOT_YET;
	if(session.term[term_id].ip)
	{
		// if term_id == 1
		while(term_id && on_demand && session.term[THE_OTHER(term_id)].status == STATUS_NOT_YET)
			thread_sleep(10);
		session.term[term_id].fd = start_client(session.term[term_id].ip, session.term[term_id].port);
	}
	else
		session.term[term_id].fd = start_server(session.term[term_id].port);

	if(session.term[term_id].fd!=INVALID_SOCKET)
	{
		session.term[term_id].status = STATUS_READY;
		// NOTE: !term_id denote another term
		while(session.term[THE_OTHER(term_id)].status==STATUS_NOT_YET)
			thread_sleep(10);
		if(session.term[THE_OTHER(term_id)].status == STATUS_READY)
			data_forward(session.term[term_id].fd, session.term[THE_OTHER(term_id)].fd);
	}else
		session.term[term_id].status = STATUS_ERROR;

	// close our socket
	closesocket(session.term[term_id].fd);
	// this is intented to wake the other thread
	closesocket(session.term[THE_OTHER(term_id)].fd);

	EXIT_TASK();
}

int main(int argc, char *argv[])
{
	init_term(&session.term[0]);
	init_term(&session.term[1]);

	if(!parse_args(argc, argv))
	{
		printf("Usage: pf <term> <term>\n");
		printf("-l [port] - Listen on port\n");
		printf("-c [ip] [port] - Connect to ip\n");
		printf("Example: pf -l 3389 -c 192.168.0.1 3389\n");
		return 1;
	}

	startup_network();

	int id1=0, id2=1;
	thread_handle_t handles[2]; 
	thread_create(&handles[0], (void *)task, (void *)&id1);
	thread_create(&handles[1], (void *)task, (void *)&id2);
	thread_join(handles[0]);
	thread_join(handles[1]);
	close_handle(handles[0]);
	close_handle(handles[1]);

	cleanup_network();
	return 0;
}
