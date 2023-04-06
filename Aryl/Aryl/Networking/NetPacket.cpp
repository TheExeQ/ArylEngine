#pragma once
#include "NetPacket.h"

#include <memory>

namespace Aryl
{
	void NetPacket::SetData(uint8_t* data, uint32_t size)
	{
		YL_CORE_ASSERT(size <= PACKET_MAXSIZE, "Packet size is too big");
		myData.resize(size);
		memcpy_s(myData.data(), size, data, size);
	}

	const Aryl::PacketBuffer& NetPacket::GetData() const
	{
		return myData;
	}

	Aryl::IPv4Endpoint NetPacket::GetSender() const
	{
		return mySender;
	}

	Aryl::IPv4Endpoint NetPacket::GetReceiver() const
	{
		return myReceiver;
	}
}