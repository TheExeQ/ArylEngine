#include "AsioUdpSocket.h"

namespace Aryl
{
	AsioUdpSocket::AsioUdpSocket(Ref<NetContext> context)
		: UdpSocket(context), mySocket(((AsioContext*)myContext.get())->GetIoContext(), udp::v4())
	{
	}

	void AsioUdpSocket::Update(uint32_t waitTime)
	{
		YL_CORE_TRACE("Socket: Update");
	}
}
