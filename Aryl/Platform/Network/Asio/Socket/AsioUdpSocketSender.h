#pragma once
#include "Aryl/Network/Socket/UdpSocketSender.h"

#include "Platform/Network/Asio/AsioContext.h"
#include "Platform/Network/Asio/Socket/AsioUdpSocket.h"

namespace Aryl
{
	class AsioUdpSocketSender : public UdpSocketSender
	{
	public:
		AsioUdpSocketSender(Ref<UdpSocket> socket);
		~AsioUdpSocketSender() = default;

	protected:
		void Update() override;
	};
} 