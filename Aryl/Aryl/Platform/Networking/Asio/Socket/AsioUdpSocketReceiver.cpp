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

		if (myOnDataReceivedDelegate)
		{
			PacketBuffer buffer;
			IPv4Endpoint sender;

			mySocket->ReceieveFrom(buffer, sender);

			NetPacket packet;
			packet.SetData(buffer.data(), buffer.size());
			SetPacketSender(packet, sender);

			myOnDataReceivedDelegate(packet);
		}
	}
}
