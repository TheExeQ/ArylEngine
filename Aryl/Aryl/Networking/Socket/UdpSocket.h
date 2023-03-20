#pragma once

#include "Aryl/Networking/NetContext.h"

#include <thread>
#include <string>

namespace Aryl
{
	// #SAMUEL_TODO: Break out to sender and receiver
	class UdpSocket
	{
	public:
		UdpSocket(Ref<NetContext> context) : myContext(context) {};
		virtual ~UdpSocket() = default;

		void Start();
		void Stop();

		void SetWaitTime(uint32_t waitTime)
		{
			myWaitTime = waitTime;
		}

	protected:
		virtual void Update(uint32_t waitTime) {};

		bool myStopping = false;
		uint32_t myWaitTime = 0u;

		std::thread myThread;

		Ref<NetContext> myContext = nullptr;

	private:
		uint32_t Run();
	};
}