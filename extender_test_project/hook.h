#ifndef HOOK_H_
#define HOOK_H_

#include <WinSock2.h>
#include <Windows.h>

#define ALLOC_SIZE 4092

//#define LEAVE_WORLD_PACKET
#define EXTENDED_PROTOCOL 0xfe
#define SERVER_CLOSE_PACKET 0x26
#define CHAR_SELECT_INFO_PACKET 0x13
#define CHAR_SELECTED_PACKET 0x15

#define PACKET_READ_CHUNK 1

#define LOGOUT_PACKET 0x09

__declspec(align(1)) struct MemLine {
	BYTE mem[28];
	int used;
};

typedef int (__stdcall*system_connect)(SOCKET socket, struct sockaddr * name, int namelen);
typedef int (__stdcall*system_send)(SOCKET socket, char *data, int datalen, int flags);
typedef int (__stdcall*system_recv)(SOCKET socket, char *data, int datalen, int flags);

static system_connect sysConnect;
static system_send sendPacket;
static system_recv recvPacket;

static FILE *file;

VOID PerformHooks();

#endif