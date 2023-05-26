#pragma once
#include "Aryl/Core/Base.h"

#include "Aryl/Network/NetContext.h"
#include "Aryl/Network/Socket/UdpSocketBuilder.h"

#include "Platform/Network/Asio/AsioContext.h"
#include "Platform/Network/Asio/Socket/AsioUdpSocket.h"

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