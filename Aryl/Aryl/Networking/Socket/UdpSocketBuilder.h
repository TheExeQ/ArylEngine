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
		virtual ~UdpSocketBuilder() = default;

		UdpSocketBuilder& AsBlocking();
		UdpSocketBuilder& AsNonBlocking();
		UdpSocketBuilder& BoundToAddress(IPv4Address Address);
		UdpSocketBuilder& BoundToEndpoint(IPv4Endpoint Endpoint);
		UdpSocketBuilder& BoundToPort(uint32_t Port);
		UdpSocketBuilder& WithSendBufferSize(uint32_t SizeInBytes);

		virtual Ref<UdpSocket> Build() const = 0;

	protected:
		bool myBlocking = true;

		IPv4Address myAddress;
		IPv4Endpoint myEndpoint;

		uint32_t myPort = 0u;
		uint32_t myBufferSize = 512u;
	};
}