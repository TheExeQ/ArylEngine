#include "AsioUdpSocketBuilder.h"

namespace Aryl
{
	Ref<UdpSocket> AsioUdpSocketBuilder::Build() const
	{
		Ref<AsioUdpSocket> result = CreateRef<AsioUdpSocket>(myContext);

		auto& io_context = ((AsioContext*)myContext.get())->GetIoContext();
		udp::resolver resolver(io_context);
		udp::resolver::results_type endpoints = resolver.resolve(myAddress.GetAddressString(), std::to_string(myPort));

		if (!myAddress.IsValid())
		{
			YL_CORE_ASSERT(false, "Address needs to be provided");
			return nullptr;
		}

		try
		{
			result->mySocket = udp::socket(io_context, *endpoints.begin());
			result->myEndpoint = IPv4Endpoint(myAddress, myPort);
		}
		catch (std::system_error err)
		{
			YL_CORE_ERROR("Could not bind to: {0}:{1}", myAddress.GetAddressString(), myPort);
			return nullptr;
		}

		result->mySocket.non_blocking(!myBlocking);

		asio::socket_base::receive_buffer_size resv_bufsize_option(myBufferSize);
		asio::socket_base::receive_buffer_size send_bufsize_option(myBufferSize);
		result->mySocket.set_option(resv_bufsize_option);
		result->mySocket.set_option(send_bufsize_option);

		return result;
	}
}
