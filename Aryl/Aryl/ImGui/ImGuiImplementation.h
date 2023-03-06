#pragma once

#include "Aryl/Core/Base.h"
#include <imgui.h>

struct GLFWwindow;

namespace Aryl
{
	class ImGuiImplementation
	{
	public:
		ImGuiImplementation();
		~ImGuiImplementation();

		void Begin();
		void End();

		static Scope<ImGuiImplementation> Create();

	private:
		ImFont* myFont = nullptr;
		GLFWwindow* myWindowPtr = nullptr;
	};
}