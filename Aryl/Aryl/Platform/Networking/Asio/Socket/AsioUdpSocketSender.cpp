#include "AsioUdpSocketSender.h"

namespace Aryl
{
	AsioUdpSocketSender::AsioUdpSocketSender(Ref<UdpSocket> socket)
		: UdpSocketSender(socket)
	{

	}

	void AsioUdpSocketSender::Update()
	{
		//YL_CORE_TRACE("Sender: Update");

		if (!mySendQueue.empty())
		{
			auto message = mySendQueue.front();
			mySendQueue.pop();

			mySocket->SendTo(message->data, message->data.size(), message->endpoint);
		}
	}
}
