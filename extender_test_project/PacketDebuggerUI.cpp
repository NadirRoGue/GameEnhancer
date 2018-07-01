#include "PacketDebuggerUI.h"

VOID PacketDebuggerUI::PushPacket(CCHAR *buf, DWORD32 len) {
	CCHAR opcode = *(buf + 2);
	struct PacketDump pd;
	pd.opcode = opcode;
	
	pd.dataPtr = new CCHAR[len];
	memcpy(pd.dataPtr,buf+3,len-3);
	UpdateUI();
}