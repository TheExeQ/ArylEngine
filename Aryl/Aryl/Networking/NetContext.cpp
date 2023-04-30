#include "NetContext.h"

#include "Aryl/Platform/Networking/Asio/AsioContext.h"

namespace Aryl
{
	Ref<NetContext> NetContext::Create()
	{
		switch (NetAPI::Current())
		{
			case NetAPIType::None: return nullptr;
			case NetAPIType::Asio: return Ref<AsioContext>();
			case NetAPIType::WinSock2: return nullptr;
		}

		YL_CORE_ASSERT(false, "Unknown NetAPI");
		return nullptr;
	}
}