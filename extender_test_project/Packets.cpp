#include "stdafx.h"

serverpacket_handler *handlers;

VOID InitializePacketHandlers()
{
	handlers = new serverpacket_handler[3];
	handlers[0] = (serverpacket_handler)ServerPacket_RequestHWID;
	handlers[1] = (serverpacket_handler)ServerPacket_RequestSecurityNumber;
	handlers[2] = (serverpacket_handler)ServerPacket_StartingVoteSite;
}

TCPPacket::TCPPacket(VOID)
{
	len = pos = 0;
}

VOID TCPPacket::Pos(INT newpos)
{
	pos = newpos;
}

INT TCPPacket::Pos(VOID)
{
	return pos;
}

INT TCPPacket::GetSize(VOID)
{
	return len;
}

VOID TCPPacket::WriteByte(BYTE val)
{
	*(BYTE*)(data+pos) = val;
	pos++;
	len++;
}

VOID TCPPacket::WriteChar(CCHAR val)
{
	*(CCHAR*)(data+pos) = val;
	pos++;
	len++;
}

VOID TCPPacket::WriteShort(SHORT val)
{
	*(SHORT*)(data+pos) = val;
	pos += 2;
	len += 2;
}

VOID TCPPacket::WriteInt(INT val)
{
	*(INT*)(data+pos) = val;
	pos += 4;
	len += 4;
}

VOID TCPPacket::WriteStr(CCHAR *ascii_str)
{
	DWORD32 str_len = strlen(ascii_str);
	memcpy(data+pos,ascii_str,str_len);
	pos += str_len;
	len += str_len;
}

VOID TCPPacket::Get(CCHAR *dst_buf)
{
	memcpy(dst_buf,data,len);
}

// ==========================================================

VOID ServerPacket_RequestHWID(CCHAR *buf, INT len)
{
	rnd_n = *(INT*)(buf + 3);
	TCPPacket tcp;
	CCHAR hwid[1024];
	GetHwidMac(hwid);
	Packet_SendHWID(tcp, hwid);
	SendPacket(tcp);
}

VOID ServerPacket_RequestSecurityNumber(CCHAR *buf, INT len)
{
	TCPPacket tcp;
	Packet_SendSecurityNumber(tcp, rnd_n);
	SendPacket(tcp);
}

VOID ServerPacket_StartingVoteSite(CCHAR *buf, INT len)
{
	INT starting_site = *(INT*)(buf+3);
	//PipeHandler(starting_site);
	CreateThread(NULL, 0, ThreadWarpPipeHandler, &starting_site, 0, NULL); 
}

VOID Packet_SendHWID(TCPPacket &packet, CCHAR *str)
{
	packet.Pos(2);
	packet.WriteChar(0xe0);
	packet.WriteInt(strlen(str));
	packet.WriteStr(str);
	INT size = packet.GetSize();
	packet.Pos(0);
	packet.WriteShort(size + 2);
}

VOID Packet_SendSecurityNumber(TCPPacket &packet, INT rnd_n)
{
	packet.Pos(2);
	packet.WriteChar(0xe1);
	packet.WriteInt(rnd_n);
	INT size = packet.GetSize();
	packet.Pos(0);
	packet.WriteShort(size+2);
}

VOID Packet_SendSiteVoted(TCPPacket &packet, INT voted)
{
	packet.Pos(2);
	packet.WriteChar(0xe2);
	packet.WriteInt(voted);
	INT size = packet.GetSize();
	packet.Pos(0);
	packet.WriteShort(size + 2);
}