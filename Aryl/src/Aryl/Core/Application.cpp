#include "Application.h"

#include <iostream>

namespace Aryl
{
	Application::Application()
		: myIsRunning(true)
	{
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