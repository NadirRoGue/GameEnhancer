#include <GL\glut.h>
#include <Windows.h>
#include <stack>

struct PacketDump {
	CCHAR opcode;
	CCHAR *dataPtr;
};

CCHAR *format;

class PacketDebuggerUI {
private:
	std::stack<PacketDump> packet_debug_stack;
	VOID UpdateUI(VOID);
public:
	VOID PushPacket(CCHAR *buf, DWORD32 buf_len);
};