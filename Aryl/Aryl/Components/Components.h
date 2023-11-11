#pragma once

#include "Aryl/Reflection/ReflectionMacros.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>

namespace Aryl
{
	struct TagComponent
	{
		std::string tag;
	};

	struct TransformComponent
	{
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
	};

	struct ObjectMovement
	{
		glm::vec2 direction;
		float lerp_time;
	};

	struct SpriteRendererComponent
	{
		std::filesystem::path spritePath;
	};

	struct TestComponent
	{
		int intValue = 0;
		float floatValue = 0.f;
		bool boolValue = false;
	};

	inline void RegisterReflection()
	{
		// TagComponent
		{
			YL_REFLECT(Aryl::TagComponent, tag);
		}

		// TransformComponent
		{
			YL_REFLECT(Aryl::TransformComponent, position);
			YL_REFLECT(Aryl::TransformComponent, rotation);
			YL_REFLECT(Aryl::TransformComponent, scale);
		}

		// SpriteRendererComponent
		{
			YL_REFLECT(Aryl::SpriteRendererComponent, spritePath);
		}

		// TestComponent
		{
			YL_REFLECT(Aryl::TestComponent, intValue);
			YL_REFLECT(Aryl::TestComponent, floatValue);
			YL_REFLECT(Aryl::TestComponent, boolValue);
		}
	}
}
