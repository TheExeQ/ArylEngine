#pragma once
#define PACKET_MAXSIZE 512u

#include "NetMessageTypes.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

#include <vector>

namespace Aryl
{
	typedef std::vector<uint8_t> PacketBuffer;

	struct NetPacketHeader
	{
		NetMessageType id = NetMessageType::Unknown;
		uint32_t size = 0;
	};

	struct NetPacket
	{
		// Header & Body vector
		NetPacketHeader header;
		PacketBuffer data;

		IPv4Endpoint endpoint;
		
		// serializes the packet
		PacketBuffer serialize()
		{
			uint32_t size = 0u;
			uint32_t offset = 0u;

			size += sizeof(header);
			size += data.size();
			
			PacketBuffer buffer;
			
			buffer.resize(size);

			memcpy_s(buffer.data() + offset, sizeof(header), &header, sizeof(header));
			offset += sizeof(header);

			memcpy_s(buffer.data() + offset, data.size(), data.data(), data.size());
			offset += data.size();

			return buffer;
		}

		// deserializes the packet
		void deserialize(PacketBuffer buffer)
		{
			uint32_t offset = 0u;

			memcpy_s(&header, sizeof(header), buffer.data() + offset, sizeof(header));
			offset += sizeof(header);

			data.resize(header.size);

			memcpy_s(data.data(), data.size(), buffer.data() + offset, data.size());
			offset += data.size();
		}

		// returns size of data in bytes
		size_t size() const
		{
			return data.size();
		}

		// Override for std::cout compatibility - produces friendly description of message
		friend std::ostream& operator << (std::ostream& os, const NetPacket& msg)
		{
			os << "ID:" << (int)msg.header.id << " Size:" << msg.header.size;
			return os;
		}

		// Pushes any POD-like data into the message buffer
		template<typename DataType>
		friend NetPacket& operator << (NetPacket& msg, const DataType& data)
		{
			YL_CORE_ASSERT(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			size_t i = msg.data.size();

			msg.data.resize(msg.data.size() + sizeof(DataType));

			memcpy_s(msg.data.data() + i, sizeof(DataType), &data, sizeof(DataType));
			msg.header.size = msg.size();

			return msg;
		}

		// Pulls any POD-like data form the message buffer
		template<typename DataType>
		friend NetPacket& operator >> (NetPacket& msg, DataType& data)
		{
			YL_CORE_ASSERT(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

			size_t i = msg.data.size() - sizeof(DataType);
			memcpy_s(&data, sizeof(DataType), msg.data.data() + i, sizeof(DataType));

			msg.data.resize(i);
			msg.header.size = msg.size();

			return msg;
		}
	};

	//struct NetPacket
	//{
	//public:
	//	NetPacket() = default;
	//	~NetPacket() = default;
	//
	//	void SetData(uint8_t* data, uint32_t size);

	//	const PacketBuffer& GetData() const;
	//	IPv4Endpoint GetSender() const;
	//	IPv4Endpoint GetReceiver() const;

	//private:
	//	friend class UdpSocketReceiver;
	//	friend class UdpSocketSender;

	//	IPv4Endpoint mySender;
	//	IPv4Endpoint myReceiver;
	//	PacketBuffer myData = {};
	//};
}