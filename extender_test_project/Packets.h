#pragma once

#include <Windows.h>

typedef VOID (__stdcall*serverpacket_handler)(CCHAR *buf, INT len);
extern serverpacket_handler *handlers;

VOID InitializePacketHandlers();

class TCPPacket {
private:
	CCHAR data[1024];
	INT len;
	INT pos;
public:
	TCPPacket(VOID);
	VOID Pos(INT newpos);
	INT Pos(VOID);
	INT GetSize(VOID);
	VOID WriteByte(BYTE val);
	VOID WriteChar(CCHAR val);
	VOID WriteShort(SHORT val);
	VOID WriteInt(INT val);
	VOID WriteStr(CCHAR *ascii_str);
	VOID Get(CCHAR *dst_buf);
};

VOID Packet_SendHWID(TCPPacket &packet, CCHAR *str);
VOID Packet_SendSecurityNumber(TCPPacket &packet, INT rnd_n);
VOID Packet_SendSiteVoted(TCPPacket &packet, INT site);

VOID ServerPacket_RequestHWID(CCHAR *buf, INT len);
VOID ServerPacket_RequestSecurityNumber(CCHAR *buf, INT len);
VOID ServerPacket_StartingVoteSite(CCHAR *buf, INT len);