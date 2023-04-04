#pragma once
#include "Aryl/Networking/Socket/UdpSocketReceiver.h"
#include "Aryl/Platform/Networking/Asio/AsioContext.h"
#include "Aryl/Platform/Networking/Asio/Socket/AsioUdpSocket.h"

namespace Aryl
{
	class AsioUdpSocketReceiver : public UdpSocketReceiver
	{
	public:
		AsioUdpSocketReceiver(Ref<UdpSocket> socket);
		~AsioUdpSocketReceiver() = default;

	protected:
		void Update() override;

	private:
		friend class AsioUdpSocketBuilder;
	};
}