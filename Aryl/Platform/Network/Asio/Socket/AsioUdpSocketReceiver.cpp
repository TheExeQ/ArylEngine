#include "AsioUdpSocketReceiver.h"

namespace Aryl
{
	AsioUdpSocketReceiver::AsioUdpSocketReceiver(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate, Ref<NetReliableHandler> nrh)
		: UdpSocketReceiver(socket, onDataReceivedDelegate, nrh)
	{

	}

	void AsioUdpSocketReceiver::Update()
	{

	}
}
