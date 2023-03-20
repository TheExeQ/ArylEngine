#pragma once

#include "UdpSocket.h"

namespace Aryl
{
	void UdpSocket::Start()
	{
		myThread = std::thread([this]() { Run(); });
	}

	void UdpSocket::Stop()
	{
		myStopping = true;
	}

	uint32_t UdpSocket::Run()
	{
		while (!myStopping)
		{
			Update(myWaitTime);
		}

		return 0;
	}
}