#pragma once

#include "Core/Application.h"

#include <filesystem>

extern Aryl::Application* Aryl::CreateApplication(const std::filesystem::path& appPath);

namespace Aryl
{
	void Create(const std::filesystem::path& appPath)
	{
		Application* app = Aryl::CreateApplication(appPath);
		app->Run();

		delete app;
	}
}

int main(int argc, char** argv)
{
	Aryl::Create(std::filesystem::current_path());
	return 0;
}
