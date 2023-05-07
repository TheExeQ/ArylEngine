#pragma once
#include "Aryl/Core/Base.h"

#include "Aryl/Networking/NetContext.h"
#include "Aryl/Networking/Socket/UdpSocketBuilder.h"

#include "Platform/Networking/Asio/AsioContext.h"
#include "Platform/Networking/Asio/Socket/AsioUdpSocket.h"

namespace Aryl
{
	class AsioUdpSocketBuilder : public UdpSocketBuilder
	{
	public:
		AsioUdpSocketBuilder(Ref<NetContext> context) : UdpSocketBuilder(context) {};
		~AsioUdpSocketBuilder() = default;

		Ref<UdpSocket> Build() const override;
	};
}