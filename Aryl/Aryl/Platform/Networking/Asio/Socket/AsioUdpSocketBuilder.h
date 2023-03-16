#pragma once
#include "Aryl/Core/Base.h"

#include "Aryl/Networking/Socket/UdpSocketBuilder.h"
#include "Aryl/Platform/Networking/Asio/AsioContext.h"
#include "Aryl/Platform/Networking/Asio/Socket/AsioUdpSocket.h"

namespace Aryl
{
	class AsioUdpSocketBuilder : public UdpSocketBuilder
	{
	public:
		AsioUdpSocketBuilder(Ref<AsioContext> context) : myContext(context) {};
		~AsioUdpSocketBuilder() = default;

		Ref<UdpSocket> Build() const override;

	private:
		Ref<AsioContext> myContext = nullptr;
	};
}