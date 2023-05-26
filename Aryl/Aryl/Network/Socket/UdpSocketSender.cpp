#pragma once
#include "UdpSocketSender.h"

#include "Aryl/Network/NetAPI.h"

#include "Platform/Network/Asio/Socket/AsioUdpSocketSender.h"

namespace Aryl
{

	UdpSocketSender::UdpSocketSender(Ref<UdpSocket> socket)
		: mySocket(socket), myThread([this]() { Run(); })
	{

	}

	uint32_t UdpSocketSender::Run()
	{
		while (!myStopping)
		{
			Update();
		}

		return 0;
	}

	bool UdpSocketSender::Send(Ref<NetPacket> packet, IPv4Endpoint receiver)
	{
		packet->header.size = packet->size();
		if (packet->size() > PACKET_MAXSIZE)
		{
			YL_CORE_WARN("Packet exceeded allowed size limit and will be aborted.");
			return false;
		}

		packet->endpoint = receiver;

		if (!myStopping && packet->endpoint.IsValid())
		{
			mySendQueue.push(packet);
			return true;
		}

		return false;
	}

	Ref<UdpSocketSender> UdpSocketSender::Create(Ref<UdpSocket> socket)
	{
		switch (NetAPI::Current())
		{
			case NetAPIType::None: return nullptr;
			case NetAPIType::Asio: return CreateRef<AsioUdpSocketSender>(socket);
			case NetAPIType::WinSock2: return nullptr;
		}

		YL_CORE_ASSERT(false, "Unknown NetAPI");
		return nullptr;
	}
}