#include "UdpSocketBuilder.h"

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

	UdpSocketBuilder& UdpSocketBuilder::BoundToEndpoint(IPv4Endpoint Endpoint)
	{
		myEndpoint = Endpoint;
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
}