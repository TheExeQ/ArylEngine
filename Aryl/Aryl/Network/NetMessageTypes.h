#pragma once

namespace Aryl
{
	enum class NetMessageType : uint32_t
	{
		Unknown,
		Connect,
		Disconnect,
		StringMessage,
		SyncWorld,
	};
}