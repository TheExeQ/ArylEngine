#pragma once
#include "Aryl/Core/Base.h"
#include "Aryl/Networking/Socket/UdpSocket.h"

#include "Aryl/Networking/NetPacket.h"
#include "Aryl/Networking/Interfaces/IPv4/IPv4Address.h"
#include "Aryl/Networking/Interfaces/IPv4/IPv4Endpoint.h"

#include <functional>

namespace Aryl
{
	class UdpSocketReceiver
	{
	public:
		UdpSocketReceiver(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate = nullptr);
		virtual ~UdpSocketReceiver() { myThread.join(); };

		void Stop()
		{
			myStopping = true;
		}

		static Ref<UdpSocketReceiver> Create(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate = nullptr);

	protected:
		virtual void Update() {};
		void SetPacketSender(NetPacket& outPacket, IPv4Endpoint sender) { outPacket.mySender = sender; };

		std::function<void(NetPacket)> myOnDataReceivedDelegate = nullptr;

		bool myStopping = false;
		std::thread myThread;

		Ref<UdpSocket> mySocket = nullptr;

	private:
		uint32_t Run();
	};
}