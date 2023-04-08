#pragma once

#include <string>

struct my_type
{
	float value;
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