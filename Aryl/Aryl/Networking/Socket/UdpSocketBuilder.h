#pragma once
#include "Aryl/Core/Base.h"
#include "Aryl/Networking/NetAPI.h"

#include "Aryl/Networking/Socket/UdpSocket.h"

#include "Aryl/Networking/Interfaces/IPv4/IPv4Address.h"
#include "Aryl/Networking/Interfaces/IPv4/IPv4Endpoint.h"

namespace Aryl
{
	class UdpSocketBuilder
	{
	public:
		UdpSocketBuilder() = default;
		~UdpSocketBuilder() = default;

		virtual UdpSocketBuilder& AsBlocking() = 0;
		virtual UdpSocketBuilder& AsNonBlocking() = 0;
		virtual UdpSocketBuilder& AsReusable() = 0;
		virtual UdpSocketBuilder& BoundToAddress(const IPv4Address& Address) = 0;
		virtual UdpSocketBuilder& BoundToEndpoint(const IPv4Endpoint& Endpoint) = 0;
		virtual UdpSocketBuilder& BoundToPort(uint32_t Port) = 0;
		virtual UdpSocketBuilder& WithSendBufferSize(uint32_t SizeInBytes) = 0;

		virtual Ref<UdpSocket> Build() const = 0;
	};
}