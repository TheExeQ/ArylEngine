#pragma once

namespace Aryl
{
	enum class NetPacketType
	{
		Reliable,
		Unreliable,
	};
	
	enum class NetMessageType : uint32_t
	{
		Unknown,
		Connect,
		Disconnect,
		Acknowledgement,
		StringMessage,
		CreateEntity,
		RemoveEntity,
		SyncWorld,
	};
}