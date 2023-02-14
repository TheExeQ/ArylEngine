#pragma once

#include <string>
#include <filesystem>

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