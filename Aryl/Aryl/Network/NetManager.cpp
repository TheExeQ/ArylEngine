#include "NetManager.h"

namespace Aryl
{
	NetManager::NetManager()
	{
		myContext = NetContext::Create();
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