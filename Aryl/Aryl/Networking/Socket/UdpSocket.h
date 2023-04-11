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

		virtual void SendTo(PacketBuffer data, IPv4Endpoint endpoint) {};
		virtual void ReceieveFrom(PacketBuffer& outData, IPv4Endpoint& outSender) {};

		IPv4Endpoint GetEndpoint() const { return myEndpoint; };

	protected:
		friend class UdpSocketBuilder;
		IPv4Endpoint myEndpoint;

		Ref<NetContext> myContext = nullptr;
	};
}