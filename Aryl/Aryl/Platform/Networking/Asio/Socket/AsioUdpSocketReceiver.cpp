#include "AsioUdpSocketReceiver.h"

namespace Aryl
{

	AsioUdpSocketReceiver::AsioUdpSocketReceiver(Ref<UdpSocket> socket)
		: UdpSocketReceiver(socket)
	{

	}

	void AsioUdpSocketReceiver::Update()
	{
		YL_CORE_TRACE("Socket: Update");
	}
}
