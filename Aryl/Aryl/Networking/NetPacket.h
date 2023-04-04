#pragma once
#define PACKET_SIZE 512u

#include "Interfaces/IPv4/IPv4Endpoint.h"

#include <array>

namespace Aryl
{
	typedef std::array<uint8_t, PACKET_SIZE> PacketBuffer;

	struct NetPacket
	{
	public:
		NetPacket() = default;
		~NetPacket() = default;
	
		IPv4Endpoint endpoint;
		PacketBuffer data;
	};
}