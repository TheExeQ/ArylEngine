#include "Application.h"

#include <iostream>

namespace Aryl
{
	Application::Application()
	{
		/* Initialize the library */
		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW" << std::endl;
			return;
		}

		/* Create a window */
		myWindow = glfwCreateWindow(1080, 720, "Aryl Engine", nullptr, nullptr);
		if (!myWindow)
		{
			glfwTerminate();
			std::cout << "Failed to setup Window" << std::endl;
			return;
		}
	}

	Application::~Application()
	{
		glfwTerminate();
	}

	void Application::Update()
	{
		/* Make the window's context current */
		glfwMakeContextCurrent(myWindow);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(myWindow))
		{
			/* Swap front and back buffers */
			glfwSwapBuffers(myWindow);

			/* Poll for and process events */
			glfwPollEvents();

			// Update
			std::cout << "Application::Update()" << std::endl;
		}
	}
}