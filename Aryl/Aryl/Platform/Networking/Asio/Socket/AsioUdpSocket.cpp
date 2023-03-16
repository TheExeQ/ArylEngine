#include "AsioUdpSocket.h"

namespace Aryl
{
	AsioUdpSocket::AsioUdpSocket(Ref<AsioContext> context)
		: myContext(context), mySocket(myContext->GetIoContext(), udp::v4())
	{
	}
}
