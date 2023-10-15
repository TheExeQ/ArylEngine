#include "NetContext.h"

#include "NetAPI.h"
#include "Host/Client.h"
#include "Host/Server.h"
#include "Platform/Network/Asio/AsioContext.h"

namespace Aryl
{
	Ref<Server> NetContext::InitServer(HostSettings hostSettings)
	{
		myServer = CreateRef<Server>(hostSettings);
		return myServer;
	}

	Ref<Client> NetContext::InitClient(HostSettings hostSettings)
	{
		myClient = CreateRef<Client>(hostSettings);
		return myClient;
	}

	Ref<NetContext> NetContext::Create()
	{
		switch (NetAPI::Current())
		{
			case NetAPIType::None: return nullptr;
			case NetAPIType::Asio: return CreateRef<AsioContext>();
			case NetAPIType::WinSock2: return nullptr;
		}

		YL_CORE_ASSERT(false, "Unknown NetAPI");
		return nullptr;
	}
}