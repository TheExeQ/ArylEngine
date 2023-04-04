#pragma once
#include "Aryl/Networking/Socket/UdpSocketSender.h"
#include "Aryl/Platform/Networking/Asio/AsioContext.h"
#include "Aryl/Platform/Networking/Asio/Socket/AsioUdpSocket.h"

namespace Aryl
{
	class AsioUdpSocketSender : public UdpSocketSender
	{
	public:
		AsioUdpSocketSender(Ref<UdpSocket> socket);
		~AsioUdpSocketSender() = default;

	protected:
		void Update() override;

	private:
		friend class AsioUdpSocketBuilder;
	};
}