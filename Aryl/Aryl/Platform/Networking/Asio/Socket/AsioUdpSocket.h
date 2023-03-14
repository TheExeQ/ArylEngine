#pragma once
#include "Aryl/Networking/Socket/UdpSocket.h"
#include "Aryl/Platform/Networking/Asio/AsioContext.h"
#include "Aryl/Platform/Networking/Asio/Interfaces/IPv4/AsioIPv4Endpoint.h"

namespace Aryl
{
	class AsioUdpSocket : public UdpSocket
	{
	public:
		AsioUdpSocket(const AsioContext& context, AsioIPv4Endpoint endpoint);
		~AsioUdpSocket() = default;

	private:
		udp::socket mySocket;
	};
}