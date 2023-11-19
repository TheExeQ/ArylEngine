#pragma once

#include "TimeStep.h"

namespace Aryl 
{
	class Time
	{
	public:
		Time() {}

		Timestep GetDeltaTime() const { return myDeltaTime; }

	private:
		friend class Application;
		
		Timestep myDeltaTime = 0;
		Timestep myLastFrameTime = 0;
	};

}