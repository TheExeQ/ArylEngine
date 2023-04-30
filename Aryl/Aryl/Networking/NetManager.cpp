#include "NetManager.h"

namespace Aryl
{
	NetManager::NetManager()
	{
		myContext.reset(NetContext::Create());
	}

	NetManager::~NetManager()
	{
		myContext = nullptr;
	}

	Scope<NetManager> NetManager::Create()
	{
		return CreateScope<NetManager>();
	}
}