#pragma once
#include <string>

namespace Aryl
{
	class IPv4Address
	{
	public:
		IPv4Address() = default;
		IPv4Address(std::string address) : myAddress(address) {};
		~IPv4Address() = default;

		bool IsValid() const { return !myAddress.empty(); }
		std::string ToString() const { return myAddress; };

	private:
		std::string myAddress;
	};
}