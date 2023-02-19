#pragma once

#include <string>
#include <filesystem>

//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>

namespace Aryl
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		bool myIsRunning = false;

		inline static Application* myInstance;
	};

	static Application* CreateApplication(const std::filesystem::path& appPath);
}