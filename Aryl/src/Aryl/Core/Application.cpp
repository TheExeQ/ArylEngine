#include "Application.h"

#include <iostream>

namespace Aryl
{
	Application::Application()
		: myIsRunning(true)
	{
		glm::vec3 testVec = { 3.f, 4.f, 5.f };
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (myIsRunning)
		{
			// Update
			std::cout << "Test" << std::endl;
		}
	}
}