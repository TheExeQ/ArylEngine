#include "NetContext.h"

#include "Platform/Network/Asio/AsioContext.h"

namespace Aryl
{
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