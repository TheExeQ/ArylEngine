#pragma once

#include <string>
#include <filesystem>

#include <GLFW/glfw3.h>

namespace Aryl
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Update();

	private:
		GLFWwindow* myWindow;

		inline static Application* myInstance;
	};

	static Application* CreateApplication(const std::filesystem::path& appPath);
}