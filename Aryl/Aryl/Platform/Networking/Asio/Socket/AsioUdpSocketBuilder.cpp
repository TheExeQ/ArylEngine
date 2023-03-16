#include "AsioUdpSocketBuilder.h"

namespace Aryl
{
	Ref<UdpSocket> AsioUdpSocketBuilder::Build() const
	{
		Ref<AsioUdpSocket> result = CreateRef<AsioUdpSocket>(myContext);

		result->mySocket = udp::socket(myContext->GetIoContext());

		if (myAddress.IsValid() && myPort)
		{
			result->mySocket.bind(udp::endpoint(udp::v4(), myPort));
		}

		result->mySocket.non_blocking(!myBlocking);

		asio::socket_base::receive_buffer_size resv_bufsize_option(myBufferSize);
		asio::socket_base::receive_buffer_size send_bufsize_option(myBufferSize);
		result->mySocket.set_option(resv_bufsize_option);
		result->mySocket.set_option(send_bufsize_option);

		if (myEndpoint.IsValid())
		{
			udp::resolver resolver(myContext->GetIoContext());
			udp::resolver::results_type endpoints = resolver.resolve(myEndpoint.GetAddress().GetAddressString(), std::to_string(myEndpoint.GetPort()));
			result->myEndpoint = *endpoints.begin();
		}

		return result;
	}
}
