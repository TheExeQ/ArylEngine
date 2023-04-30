#pragma once

#include "Aryl/Core/Base.h"

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Aryl 
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}