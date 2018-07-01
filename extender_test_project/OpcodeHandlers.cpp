#include "stdafx.h"

using namespace l2clientextender;
using namespace l2clientextender::network;

NpcHtmlMessageHandler::NpcHtmlMessageHandler() :AbstractOpcodeHandler(NPCHTMLMESSAGE_OPCODE)
{
}

BOOL NpcHtmlMessageHandler::handleOpcode(CCHAR *buf, const INT len)
{
	FILE *f = fopen("debug.txt", "a");
	fprintf(f, "NpcHtmlMessage packet captured\n");
	fclose(f);
	return FALSE;
}