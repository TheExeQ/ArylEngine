#pragma once
#include "NetApi.h"

#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

#include "Aryl/Core/Base.h"

namespace Aryl
{
	class Socket
	{
	public:
		Socket() = default;
		~Socket() = default;
		
	private:

	};

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

		virtual Ref<Socket> Build() const = 0;
	};

}