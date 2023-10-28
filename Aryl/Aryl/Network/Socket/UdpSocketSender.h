#pragma once
#include "Aryl/Core/Base.h"
#include "Aryl/Network/Socket/UdpSocket.h"

#include "Aryl/Network/NetPacket.h"
#include "Aryl/Network/Interfaces/IPv4/IPv4Address.h"
#include "Aryl/Network/Interfaces/IPv4/IPv4Endpoint.h"

#include <queue>

namespace Aryl
{
	class UdpSocketSender
	{
	public:
		UdpSocketSender(Ref<UdpSocket> socket);
		virtual ~UdpSocketSender() { myThread.join(); };

		void Stop()
		{
			myStopping = true;
		}

		bool Send(Ref<NetPacket> packet, IPv4Endpoint receiver);
		Ref<UdpSocket> GetSocket() { return mySocket; };

		static Ref<UdpSocketSender> Create(Ref<UdpSocket> socket);

	protected:
		virtual void Update() {};

		std::queue<Ref<NetPacket>> mySendQueue;
		
		bool myStopping = false;
		std::thread myThread;

		Ref<UdpSocket> mySocket = nullptr;

	private:
		uint32_t Run();

		std::unordered_map<std::string, uint32_t> myPacketIdMap;
	};
}