#pragma once
#include "UdpSocketReceiver.h"

#include "Aryl/Network/NetAPI.h"

#include "Platform/Network/Asio/Socket/AsioUdpSocketReceiver.h"

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
			if (myOnDataReceivedDelegate)
			{
				PacketBuffer buffer;
				IPv4Endpoint sender;

				mySocket->ReceieveFrom(buffer, sender);

				NetPacket packet;
				packet.deserialize(buffer);

				packet.endpoint = sender;

				myOnDataReceivedDelegate(packet);
			}

			Update();
		}

		return 0;
	}

	Ref<UdpSocketReceiver> UdpSocketReceiver::Create(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate)
	{
		switch (NetAPI::Current())
		{
			case NetAPIType::None: return nullptr;
			case NetAPIType::Asio: return CreateRef<AsioUdpSocketReceiver>(socket, onDataReceivedDelegate);
			case NetAPIType::WinSock2: return nullptr;
		}

		YL_CORE_ASSERT(false, "Unknown NetAPI");
		return nullptr;
	}
}
