#pragma once
#include "Aryl/Networking/Socket/UdpSocketSender.h"

#include "Platform/Networking/Asio/AsioContext.h"
#include "Platform/Networking/Asio/Socket/AsioUdpSocket.h"

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