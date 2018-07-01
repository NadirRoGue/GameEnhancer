#ifndef OPCODE_HANDLERS_H_
#define OPCODE_HANDLERS_H_

#include <Windows.h>
#include "NetworkHandler.h"

#define NPCHTMLMESSAGE_OPCODE 0x19

namespace l2clientextender
{
	namespace network
	{
		class NpcHtmlMessageHandler final: public AbstractOpcodeHandler
		{
		public:
			NpcHtmlMessageHandler();
			BOOL handleOpcode(CCHAR *buf, const INT len);
		};
	}
}

#endif