#pragma once
#include "Aryl/Network/Socket/UdpSocket.h"

#include "Platform/Network/Asio/AsioContext.h"

namespace Aryl
{
	class AsioUdpSocket : public UdpSocket
	{
	public:
		AsioUdpSocket(Ref<NetContext> context);
		~AsioUdpSocket() = default;

		void SendTo(PacketBuffer data, IPv4Endpoint endpoint) override;
		void ReceieveFrom(PacketBuffer& outData, IPv4Endpoint& outSender) override;

	private:
		friend class AsioUdpSocketBuilder;

		udp::socket mySocket;
	};
}