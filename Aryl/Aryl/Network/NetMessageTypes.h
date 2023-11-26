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
		Ping,
		Acknowledgement,
		StringMessage,
		CreateEntity,
		RemoveEntity,
		SyncWorld,
	};
}