#include "AsioUdpSocket.h"

#include <system_error>

namespace Aryl
{
	AsioUdpSocket::AsioUdpSocket(Ref<NetContext> context)
		: UdpSocket(context), mySocket(((AsioContext*)myContext.get())->GetIoContext(), udp::v4())
	{
	}

	void AsioUdpSocket::SendTo(PacketBuffer data, IPv4Endpoint endpoint)
	{
		if (mySocket.is_open())
		{
			asio::ip::udp::endpoint remote_endpoint;

			remote_endpoint = asio::ip::udp::endpoint(
				asio::ip::address::from_string(endpoint.GetAddress().ToString()), endpoint.GetPort());

			std::error_code err;
			mySocket.send_to(asio::buffer((void*)data.data(), data.size()), remote_endpoint, 0, err);

			if (err)
			{
				YL_CORE_ERROR("Failed to send to {0}:{1}", endpoint.GetAddress().ToString(), endpoint.GetPort());
				YL_CORE_ERROR("Error {0}: {1}", err.value(), err.message());
			}
		}
	}

	void AsioUdpSocket::ReceieveFrom(PacketBuffer& outData, IPv4Endpoint& outSender)
	{
		if (mySocket.is_open())
		{
			asio::ip::udp::endpoint remote_endpoint;

			PacketBuffer recvBuffer;
			recvBuffer.resize(PACKET_MAXSIZE);

			size_t len = mySocket.receive_from(asio::buffer(recvBuffer), remote_endpoint);
			outData.resize(len);
			std::memcpy(outData.data(), recvBuffer.data(), len);

			outSender = IPv4Endpoint(IPv4Address(remote_endpoint.address().to_string()), (uint32_t)remote_endpoint.port());
		}
	}
}
