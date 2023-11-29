#include "AsioUdpSocketSender.h"

namespace Aryl
{
	AsioUdpSocketSender::AsioUdpSocketSender(Ref<UdpSocket> socket, Ref<NetReliableHandler> nrh)
		: UdpSocketSender(socket, nrh)
	{

	}

	void AsioUdpSocketSender::Update()
	{
		UdpSocketSender::Update();
		
		if (!mySendQueue.empty())
		{
			auto message = mySendQueue.front();
			mySendQueue.pop();

			mySocket->SendTo(message->serialize(), message->endpoint);
		}
	}
}
