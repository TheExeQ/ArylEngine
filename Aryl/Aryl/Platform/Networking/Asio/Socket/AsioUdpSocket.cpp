#include "AsioUdpSocket.h"

namespace Aryl
{
	AsioUdpSocket::AsioUdpSocket(Ref<NetContext> context)
		: UdpSocket(context), mySocket(((AsioContext*)myContext.get())->GetIoContext(), udp::v4())
	{
	}
}
