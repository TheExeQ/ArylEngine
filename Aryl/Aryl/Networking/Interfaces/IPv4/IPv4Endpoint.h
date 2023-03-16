#pragma once
#include "IPv4Address.h"

namespace Aryl
{
	class IPv4Endpoint
	{
	public:
		IPv4Endpoint() = default;
		IPv4Endpoint(IPv4Address address, uint32_t port) : myAddress(address), myPort(port) {};
		~IPv4Endpoint() = default;

		bool IsValid() const { return myAddress.IsValid() && myPort; }
		IPv4Address GetAddress() const { return myAddress; };
		uint32_t GetPort() const { return myPort; };

	private:
		IPv4Address myAddress;
		uint32_t myPort = 0u;
	};
}