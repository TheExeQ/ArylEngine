#pragma once
#define PACKET_MAXSIZE 512u

#include "Interfaces/IPv4/IPv4Endpoint.h"

#include <vector>

namespace Aryl
{
	typedef std::vector<uint8_t> PacketBuffer;

	struct NetPacket
	{
	public:
		NetPacket() = default;
		~NetPacket() = default;
	
		void SetData(uint8_t* data, uint32_t size);

		const PacketBuffer& GetData() const;
		IPv4Endpoint GetSender() const;
		IPv4Endpoint GetReceiver() const;

	private:
		friend class UdpSocketReceiver;
		friend class UdpSocketSender;

		IPv4Endpoint mySender;
		IPv4Endpoint myReceiver;
		PacketBuffer myData = {};
	};
}