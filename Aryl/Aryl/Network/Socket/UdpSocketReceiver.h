#pragma once
#include "Aryl/Core/Base.h"
#include "Aryl/Network/Socket/UdpSocket.h"

#include "Aryl/Network/NetPacket.h"
#include "Aryl/Network/Interfaces/IPv4/IPv4Address.h"
#include "Aryl/Network/Interfaces/IPv4/IPv4Endpoint.h"

#include <functional>

namespace Aryl
{
	class UdpSocketReceiver
	{
	public:
		UdpSocketReceiver(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate = nullptr, Ref<NetReliableHandler> nrh = nullptr);
		virtual ~UdpSocketReceiver() { myThread.join(); };

		void Stop()
		{
			myStopping = true;
		}

		Ref<UdpSocket> GetSocket() { return mySocket; };

		static Ref<UdpSocketReceiver> Create(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate = nullptr, Ref<NetReliableHandler> nrh = nullptr);

	protected:
		virtual void Update() {};

		std::function<void(NetPacket)> myOnDataReceivedDelegate = nullptr;

		bool myStopping = false;
		std::thread myThread;

		Ref<UdpSocket> mySocket = nullptr;

	private:
		uint32_t Run();
		
		Ref<NetReliableHandler> reliableFallback = nullptr;
	};
}