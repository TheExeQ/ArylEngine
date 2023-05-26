#pragma once
#include "Aryl/Network/Socket/UdpSocketReceiver.h"

#include "Platform/Network/Asio/AsioContext.h"
#include "Platform/Network/Asio/Socket/AsioUdpSocket.h"

namespace Aryl
{
	class AsioUdpSocketReceiver : public UdpSocketReceiver
	{
	public:
		AsioUdpSocketReceiver(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate = nullptr);
		~AsioUdpSocketReceiver() = default;

	protected:
		void Update() override;
	};
}