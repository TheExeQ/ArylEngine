#pragma once
#include "UdpSocketSender.h"

#include "Aryl/Networking/NetAPI.h"

#include "Aryl/Platform/Networking/Asio/Socket/AsioUdpSocketSender.h"

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
		packet->myReceiver = receiver;

		if (!myStopping && packet->myReceiver.IsValid())
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