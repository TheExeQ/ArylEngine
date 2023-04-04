#pragma once

#include "Interfaces/IPv4/IPv4Endpoint.h"

namespace Aryl
{
	struct NetPacket
	{
	public:
		NetPacket() = default;
		~NetPacket() = default;
	
		IPv4Endpoint endpoint;
		uint8_t* data;
	};
}