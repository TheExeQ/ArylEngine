#pragma once

#include "Aryl/Reflection/ReflectionMacros.h"

#include <string>

struct my_type
{
	float value = 0.f;
	bool value2 = false;
	float value3 = 0.f;
};

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