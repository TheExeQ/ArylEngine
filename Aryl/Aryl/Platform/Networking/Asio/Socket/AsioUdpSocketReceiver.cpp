#include "AsioUdpSocketReceiver.h"

namespace Aryl
{
	AsioUdpSocketReceiver::AsioUdpSocketReceiver(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate)
		: UdpSocketReceiver(socket, onDataReceivedDelegate)
	{

	}

	void AsioUdpSocketReceiver::Update()
	{
		//YL_CORE_TRACE("Receiver: Update");

		PacketBuffer buffer;
		IPv4Endpoint sender;

		mySocket->ReceieveFrom(buffer, sender);

		std::string str((const char*)buffer.data());

		YL_CORE_TRACE(str);
	}
}
