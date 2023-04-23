#pragma once

#include "Aryl/Reflection/ReflectionMacros.h"

#include <string>

namespace Aryl
{
	struct TagComponent
	{
		std::string tag;
	};

	struct TestComponent
	{
		int intValue = 0;
		float floatValue = 0.f;
		bool boolValue = false;
	};
}
