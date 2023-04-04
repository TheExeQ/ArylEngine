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

	private:
		friend class AsioUdpSocketBuilder;

		udp::socket mySocket;
	};
}