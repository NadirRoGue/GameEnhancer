#include "stdafx.h"

CCHAR recv_gs_buffer[65535];
CCHAR send_client_buffer[8192];

UINT16 GetPort(SOCKET *s) {
	struct sockaddr *name = new sockaddr;
	int sockaddr_len = sizeof(sockaddr);
	getsockname(*s,name,&sockaddr_len);
	struct sockaddr_in *net_data = (struct sockaddr_in*)name;
	return ntohs(net_data->sin_port);
}

BOOL IsLoginServerPacket(SOCKET *s) {
	if(ls_port == -1) {
		ls_port = GetPort(s);
		return TRUE;
	} else if(gs_port == -1) {
		UINT16 temp = GetPort(s);
		if(temp != ls_port) {
			gs_port = temp;
			return FALSE;
		}
		return TRUE;
	}
	return GetPort(s) == ls_port;
}

int __stdcall NewConnect(SOCKET socket, struct sockaddr *name, int namelen) {

	struct sockaddr_in *net_data = (struct sockaddr_in*)name;
	
	net_data->sin_addr.S_un.S_addr = inet_addr("85.25.199.81");

	if(ls_port != -1 && net_data->sin_port == htons(2106))
		ls_port = -1;
	else if(gs_port != -1 && net_data->sin_port == htons(7777)) {
		gs_crypt.enabled = FALSE;
		client_crypt.enabled = FALSE;
		gs_port = -1;
		while(!network_queue.empty())
			network_queue.pop();
	}

	return sysConnect(socket,name,namelen);
}

int __stdcall NewSend(SOCKET socket, char *data, int datalen, int flags) {
	int result = 0;
	if(!IsLoginServerPacket(&socket)) {

		while(!network_queue.empty()) {
			TCPPacket p = network_queue.front();
			p.Get(send_client_buffer);
			network_queue.pop();
			Crypt(&gs_crypt,send_client_buffer,p.GetSize());
			sendPacket(socket,send_client_buffer,p.GetSize(),flags);
		}

		INT size = (*(SHORT*)(data) & 0xffff) - 2;
		Decrypt(&client_crypt,data+2,size);
		Crypt(&gs_crypt,data+2,size);
		result = sendPacket(socket,data,datalen,flags);
	} else
		result = sendPacket(socket,data,datalen,flags);

	return result;
}

int __stdcall NewRecv(SOCKET socket, char *data, int datalen, int flags) {
	int result = 0;
	if(!IsLoginServerPacket(&socket)) {
		
		INT size = 0,opcode = 0,temp = 0,writer = 0,second_op = 0;
		do {
			size = 0;
			second_op = 0;
			temp = 0;
			writer = 0;
			
			INT val = 0;
			while(temp < 2) {
				val = recvPacket(socket,recv_gs_buffer+writer,2 - temp,flags);
				if(val > 0) {
					temp += val;
					writer += val;
				}
			}
			
			size = (*(SHORT*)recv_gs_buffer) & 0xffff;
					
			val = 0;
			while(temp < size) {
				val = recvPacket(socket,recv_gs_buffer+writer,size - temp,flags);
				if(val > 0) {
					temp += val;
					writer += val;
				} 
			}

			Decrypt(&gs_crypt,recv_gs_buffer+2,size-2);
			opcode = (*(recv_gs_buffer+2) & 0xff);
			
			if(opcode == EXTENDED_PROTOCOL) {
				second_op = (*(SHORT*)(recv_gs_buffer + 3)) & 0xffff;
				HandleGameServerPacket(second_op,recv_gs_buffer+2,size-2);
			}
		} while(second_op == 0x8a || second_op == 0x8b || second_op == 0x8c);

		if(opcode != 0x00) {
			Crypt(&client_crypt,recv_gs_buffer+2,size-2);
		} else {
			CCHAR var_01_[16];
			memcpy(var_01_,recv_gs_buffer + 4,16);
			for(int i = 0; i < 8; i++)
			{
				if(i % 2 == 0 && var_01_[i] > (-127))
					var_01_[i] -= 2;
				else if(var_01_[i] < 123)
					var_01_[i] += 5;
			}

			SetKey(var_01_,&gs_crypt);

			for(int i = 0; i < 8; i++)
				recv_gs_buffer[i + 4] = rand() % 255;

			SetKey(recv_gs_buffer+4,&client_crypt);
		}

		memcpy(data,recv_gs_buffer,size);
		result = size;
	} else
		result = recvPacket(socket,data,datalen,flags);

	return result;
}

__declspec(naked) int TryLockBlock(int * used) {
	__asm {
		mov ecx, eax
		xchg eax, [ecx]
		ret
	}
}

BYTE * AllocMemory() {
	DWORD32 blocks = ALLOC_SIZE / sizeof(MemLine);
	static MemLine * start = (MemLine*)VirtualAllocEx(GetCurrentProcess(),NULL,ALLOC_SIZE,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	for(MemLine *it = start, *end = start + blocks; it != end; it++) {
		if(!TryLockBlock(&it->used))
			return it->mem;
	}
	return 0;
}

VOID WriteJump(BYTE * addr, VOID * func) {
	*addr = 0xe9;
	*(int*)(addr + 1) = (BYTE*)func - (addr + 5);
}

VOID FixRelativeJump(BYTE * addr, BYTE *storage) {
	int old_relative = *(int*)(addr + 1);
	DWORD32 absolute = (DWORD32)(old_relative + (addr + 5));
	*storage = 0xe9;
	*(int*)(storage + 1) = (BYTE*)absolute - (storage + 5);
}

DWORD32 InjectFunction(BYTE *addr, VOID * hook) {
	DWORD32 cb = 5;
	BYTE * saved = AllocMemory();
	if(*addr == 0xe9)
		FixRelativeJump(addr, saved);
	else {
		memcpy(saved,addr,cb);
		WriteJump(saved+cb,addr+cb);
	}
	DWORD oldProtect;
	VirtualProtect(addr,5,PAGE_EXECUTE_READWRITE,&oldProtect);
	WriteJump(addr,hook);
	VirtualProtect(addr,5,oldProtect,&oldProtect);
	return (DWORD32)saved;

}

VOID PerformHooks() {
	HMODULE dll = GetModuleHandle(L"ws2_32.dll");

	if(dll == NULL)
		return;

	sysConnect = (system_connect)InjectFunction((BYTE*)GetProcAddress(dll, "connect"), NewConnect);
	sendPacket = (system_send)InjectFunction((BYTE*)GetProcAddress(dll, "send"), NewSend);
	recvPacket = (system_recv)InjectFunction((BYTE*)GetProcAddress(dll, "recv"), NewRecv);
}