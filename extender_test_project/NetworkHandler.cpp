#include "stdafx.h"

using namespace std;
using namespace l2clientextender;
using namespace l2clientextender::network;

NetworkManager & NetworkManager::getInstance()
{
	return INSTANCE;
}

NetworkManager::NetworkManager() :lsPort(-1), gsPort(-1)
{
}

VOID NetworkManager::setLoginPort(const INT port)
{
	lsPort = port;
}

VOID NetworkManager::setGamePort(const INT port)
{
	gsPort = port;
}

INT NetworkManager::getLoginServerPort()
{
	return lsPort;
}

INT NetworkManager::getGameServerPort()
{
	return gsPort;
}

VOID NetworkManager::pushPacketToQueue(const TCPPacket &packet)
{
	outQueue.push(packet);
}

TCPPacket & NetworkManager::popPacket()
{
	TCPPacket packet = outQueue.front();
	outQueue.pop();
	return packet;
}

// ============================================================================
// ============================================================================

AbstractOpcodeHandler::AbstractOpcodeHandler(const INT opcode) :servingOpcode(opcode)
{
}

INT AbstractOpcodeHandler::getOpcode() const
{
	return servingOpcode;
}

BOOL AbstractOpcodeHandler::handleOpcode(CCHAR *buf, const INT len)
{
	return FALSE;
}

VOID AbstractOpcodeHandler::addHandler(const AbstractOpcodeHandler &handler)
{
}

// ============================================================================
// ============================================================================

OpcodeContainerHandler::OpcodeContainerHandler(const INT opcode, AbstractOpcodeHandler &defaultHandler) :AbstractOpcodeHandler(opcode)
{
	stackSize = HIGHER_OPCODE;
	handlers = new AbstractOpcodeHandler[stackSize];
	DWORD32 i = 0;
	while (i < stackSize)
	{
		handlers[i++] = defaultHandler;
	}
}

BOOL OpcodeContainerHandler::handleOpcode(CCHAR *buf, const INT len)
{
	UINT16 secondOpcode = *(UINT16*)(buf);

	if (secondOpcode < 0 || secondOpcode >= stackSize)
		return FALSE;

	return handlers[secondOpcode].handleOpcode(buf + 2, len);
}

VOID OpcodeContainerHandler::addHandler(const AbstractOpcodeHandler &handler)
{
	if (handler.getOpcode() >= stackSize)
		resizeHandlerStack(handler.getOpcode() + 1);

	handlers[handler.getOpcode()] = handler;
}

VOID OpcodeContainerHandler::resizeHandlerStack(const DWORD32 newSize)
{
	AbstractOpcodeHandler *temp = new AbstractOpcodeHandler[stackSize];
	memcpy(temp, handlers, stackSize * sizeof(AbstractOpcodeHandler));
	delete handlers;
	handlers = new AbstractOpcodeHandler[newSize];
	memcpy(handlers, temp, stackSize * sizeof(AbstractOpcodeHandler));
	stackSize = newSize;
	delete temp;
}

// ============================================================================
// ============================================================================

PacketManager & PacketManager::getInstance()
{
	return INSTANCE;
}

PacketManager::PacketManager()
{
	serverHandlers = new OpcodeContainerHandler(-1, AbstractOpcodeHandler());
	serverHandlers->addHandler(NpcHtmlMessageHandler());

	clientHandlers = new OpcodeContainerHandler(-1, AbstractOpcodeHandler());
}

BOOL PacketManager::handleServerOpcode(CCHAR *buf, const INT len)
{
	return serverHandlers->handleOpcode(buf, len);
}

BOOL PacketManager::handleClientOpcode(CCHAR *buf, const INT len)
{
	return clientHandlers->handleOpcode(buf, len);
}