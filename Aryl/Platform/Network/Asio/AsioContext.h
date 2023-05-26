#pragma once

#include "Aryl/Network/NetContext.h"

#include <asio.hpp>

using asio::ip::udp;

namespace Aryl
{
	class AsioContext : public NetContext
	{
	public:
		AsioContext() = default;
		~AsioContext() = default;

		void Init() override
		{
			// Temporary testing code

			try
			{
				// Create a UDP server endpoint on port 8080
				udp::socket socket(myContext, udp::endpoint(udp::v4(), 8080));

				// Wait for a message to arrive
				std::array<char, 1024> recv_buffer;
				udp::endpoint remote_endpoint;
				size_t bytes_transferred = socket.receive_from(
					asio::buffer(recv_buffer), remote_endpoint);

				// Send a reply to the sender
				std::string message = "Hello, client!";
				socket.send_to(asio::buffer(message), remote_endpoint);
			}
			catch (std::exception& e)
			{
				std::cerr << "Exception: " << e.what() << std::endl;
			}
		}

		asio::io_context& GetIoContext() { return myContext; }

	private:
		asio::io_context myContext;
	};
}
