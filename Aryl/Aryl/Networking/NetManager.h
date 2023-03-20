#pragma once

#include "NetContext.h"

#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

#include "Socket/UdpSocketBuilder.h"

namespace Aryl
{
	class NetManager
	{
	public:
		NetManager();
		~NetManager();

		Ref<NetContext> GetContext() { return myContext; };

		static Scope<NetManager> Create();

	private:
		Ref<NetContext> myContext = nullptr;
	};
}