#pragma once

#include "Aryl/Networking/NetPacket.h"
#include "Aryl/Networking/NetContext.h"
#include "Aryl/Networking/Interfaces/IPv4/IPv4Endpoint.h"

#include <thread>
#include <string>

namespace Aryl
{
	class UdpSocket
	{
	public:
		UdpSocket(Ref<NetContext> context) : myContext(context) {};
		virtual ~UdpSocket() = default;

		virtual void SendTo(PacketBuffer data, uint32_t size, IPv4Endpoint endpoint) {};
		virtual void ReceieveFrom(PacketBuffer& outData, IPv4Endpoint& outSender) {};

	protected:
		Ref<NetContext> myContext = nullptr;
	};
}