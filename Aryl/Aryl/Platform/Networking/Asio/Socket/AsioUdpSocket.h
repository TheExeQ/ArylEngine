#pragma once
#include "Aryl/Networking/Socket/UdpSocket.h"

#include "Aryl/Platform/Networking/Asio/AsioContext.h"

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

		PacketBuffer myRecvBuffer;
		udp::socket mySocket;
	};
}