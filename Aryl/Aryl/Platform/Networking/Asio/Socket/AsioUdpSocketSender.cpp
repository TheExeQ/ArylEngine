#include "AsioUdpSocketSender.h"

namespace Aryl
{

	AsioUdpSocketSender::AsioUdpSocketSender(Ref<UdpSocket> socket)
		: UdpSocketSender(socket)
	{

	}

	void AsioUdpSocketSender::Update()
	{
		YL_CORE_TRACE("Sender: Update");
	}
}
