#pragma once
#include "Aryl/Networking/Socket/UdpSocket.h"
#include "Aryl/Platform/Networking/Asio/AsioContext.h"

namespace Aryl
{
	class AsioUdpSocket : public UdpSocket
	{
	public:
		AsioUdpSocket(Ref<AsioContext> context);
		~AsioUdpSocket() = default;

	private:
		friend class AsioUdpSocketBuilder;

		Ref<AsioContext> myContext = nullptr;
		udp::socket mySocket;
		udp::endpoint myEndpoint;
	};
}