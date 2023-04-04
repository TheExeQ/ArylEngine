#pragma once

#include "Aryl/Networking/NetContext.h"

#include <thread>
#include <string>

namespace Aryl
{
	class UdpSocket
	{
	public:
		UdpSocket(Ref<NetContext> context) : myContext(context) {};
		virtual ~UdpSocket() = default;

	protected:
		Ref<NetContext> myContext = nullptr;
	};
}