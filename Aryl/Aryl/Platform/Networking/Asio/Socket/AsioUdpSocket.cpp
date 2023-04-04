#include "AsioUdpSocket.h"

#include <system_error>

namespace Aryl
{
	AsioUdpSocket::AsioUdpSocket(Ref<NetContext> context)
		: UdpSocket(context), mySocket(((AsioContext*)myContext.get())->GetIoContext(), udp::v4()), myRecvBuffer({})
	{
	}

	void AsioUdpSocket::SendTo(PacketBuffer data, uint32_t size, IPv4Endpoint endpoint)
	{
		asio::ip::udp::endpoint remote_endpoint;

		//mySocket.open(asio::ip::udp::v4());
		remote_endpoint = asio::ip::udp::endpoint(
			asio::ip::address::from_string(endpoint.GetAddress().GetAddressString()), endpoint.GetPort());

		std::error_code err;
		mySocket.send_to(asio::buffer(data, (size < data.size()) ? size : data.size()), remote_endpoint, 0, err);
		//mySocket.close();
	}

	void AsioUdpSocket::ReceieveFrom(PacketBuffer& outData, IPv4Endpoint& outSender)
	{
		asio::ip::udp::endpoint remote_endpoint;

		//mySocket.open(asio::ip::udp::v4());

		size_t len = mySocket.receive_from(asio::buffer(myRecvBuffer), remote_endpoint);
		memcpy_s(outData.data(), len, myRecvBuffer.data(), len);

		//mySocket.close();
	}
}
