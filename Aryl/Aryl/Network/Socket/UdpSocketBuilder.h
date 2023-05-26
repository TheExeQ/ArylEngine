#pragma once
#include "Aryl/Core/Base.h"
#include "Aryl/Network/NetContext.h"

#include "Aryl/Network/Socket/UdpSocket.h"

#include "Aryl/Network/Interfaces/IPv4/IPv4Address.h"
#include "Aryl/Network/Interfaces/IPv4/IPv4Endpoint.h"

namespace Aryl
{
	class UdpSocketBuilder
	{
	public:
		UdpSocketBuilder(Ref<NetContext> context) : myContext(context) {};
		virtual ~UdpSocketBuilder() = default;

		UdpSocketBuilder& AsBlocking();
		UdpSocketBuilder& AsNonBlocking();
		UdpSocketBuilder& BoundToAddress(IPv4Address Address);
		UdpSocketBuilder& BoundToPort(uint32_t Port);
		UdpSocketBuilder& WithSendBufferSize(uint32_t SizeInBytes);

		virtual Ref<UdpSocket> Build() const;

		static Ref<UdpSocketBuilder> Create(Ref<NetContext> context);

	protected:
		bool myBlocking = true;

		IPv4Address myAddress;
		uint32_t myPort = 0u;

		uint32_t myBufferSize = 512u;

		Ref<NetContext> myContext = nullptr;
	};
}