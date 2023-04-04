#pragma once
#include "UdpSocketReceiver.h"

#include "Aryl/Networking/NetAPI.h"

#include "Aryl/Platform/Networking/Asio/Socket/AsioUdpSocketReceiver.h"

namespace Aryl
{
	UdpSocketReceiver::UdpSocketReceiver(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate)
		: mySocket(socket), myThread([this]() { Run(); }), myOnDataReceivedDelegate(onDataReceivedDelegate)
	{

	}

	uint32_t UdpSocketReceiver::Run()
	{
		while (!myStopping)
		{
			Update();
		}

		return 0;
	}

	Ref<UdpSocketReceiver> UdpSocketReceiver::Create(Ref<UdpSocket> socket)
	{
		switch (NetAPI::Current())
		{
			case NetAPIType::None: return nullptr;
			case NetAPIType::Asio: return CreateRef<AsioUdpSocketReceiver>(socket);
			case NetAPIType::WinSock2: return nullptr;
		}

		YL_CORE_ASSERT(false, "Unknown NetAPI");
		return nullptr;
	}
}
