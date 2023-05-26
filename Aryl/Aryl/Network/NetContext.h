#pragma once

#include "NetAPI.h"
#include "Aryl/Core/Base.h"

namespace Aryl
{
	class NetContext
	{
	public:
		NetContext() = default;
		virtual ~NetContext() = default;

		virtual void Init() = 0;

		static Ref<NetContext> Create();
	};
}