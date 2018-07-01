/**
 * author: Nadir Román Guerrero
 * nadir.ro.gue@gmail.com
 */

#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_

#include <Windows.h>
#include <queue>

#define HIGHER_OPCODE 0xff
#define PACKET_BUILDER_DATA_SIZE 4096

namespace l2clientextender
{
	namespace network
	{
		// ===================================================================================
		// ===================================================================================

		class NetworkManager final
		{
		private:
			static NetworkManager INSTANCE;

			std::queue<TCPPacket> outQueue;
			INT gsPort, lsPort;

			NetworkManager();
			VOID pushPacketToQueue(const TCPPacket &packet);
		public:
			static NetworkManager & getInstance();

			VOID setLoginPort(const INT port);
			VOID setGamePort(const INT port);
			INT getLoginServerPort();
			INT getGameServerPort();
			TCPPacket & popPacket();
		};

		// ===================================================================================
		// ===================================================================================

		class AbstractOpcodeHandler
		{
		private:
			const INT servingOpcode;
		public:
			AbstractOpcodeHandler();
			AbstractOpcodeHandler(const INT opcode);
			INT getOpcode() const;
			virtual BOOL handleOpcode(CCHAR *buf, const INT len);
			virtual VOID addHandler(const AbstractOpcodeHandler &handler);
		};

		// ===================================================================================
		// ===================================================================================

		class OpcodeContainerHandler final : public AbstractOpcodeHandler
		{
		private:
			AbstractOpcodeHandler *handlers;
			DWORD32 stackSize;

			VOID resizeHandlerStack(const DWORD32 newSize);
		public:
			OpcodeContainerHandler(const INT opcode, AbstractOpcodeHandler &defaultHandler);
			BOOL handleOpcode(CCHAR *buf, const INT len);
			VOID addHandler(const AbstractOpcodeHandler &handler);
		};

		// ===================================================================================
		// ===================================================================================

		class PacketManager final
		{
		private:
			static PacketManager INSTANCE;

			OpcodeContainerHandler *serverHandlers;
			OpcodeContainerHandler *clientHandlers;

			PacketManager();
		public:
			static PacketManager & getInstance();

			BOOL handleServerOpcode(CCHAR *buf, const INT len);
			BOOL handleClientOpcode(CCHAR *buf, const INT len);
		};
	}
}

#endif