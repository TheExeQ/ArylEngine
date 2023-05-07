#include "UdpSocketBuilder.h"

#include "Aryl/Networking/NetAPI.h"

#include "Platform/Networking/Asio/Socket/AsioUdpSocketBuilder.h"

namespace Aryl
{
	UdpSocketBuilder& UdpSocketBuilder::AsBlocking()
	{
		myBlocking = true;
		return *this;
	}

	UdpSocketBuilder& UdpSocketBuilder::AsNonBlocking()
	{
		myBlocking = false;
		return *this;
	}

	UdpSocketBuilder& UdpSocketBuilder::BoundToAddress(IPv4Address Address)
	{
		myAddress = Address;
		return *this;
	}

	UdpSocketBuilder& UdpSocketBuilder::BoundToPort(uint32_t Port)
	{
		myPort = Port;
		return *this;
	}

	UdpSocketBuilder& UdpSocketBuilder::WithSendBufferSize(uint32_t SizeInBytes)
	{
		myBufferSize = SizeInBytes;
		return *this;
	}

	Ref<UdpSocket> UdpSocketBuilder::Build() const
	{
		return nullptr;
	}

	Ref<UdpSocketBuilder> UdpSocketBuilder::Create(Ref<NetContext> context)
	{
		switch (NetAPI::Current())
		{
			case NetAPIType::None: return nullptr;
			case NetAPIType::Asio: return CreateRef<AsioUdpSocketBuilder>(context);
			case NetAPIType::WinSock2: return nullptr;
		}

		YL_CORE_ASSERT(false, "Unknown NetAPI");
		return nullptr;
	}
}