#include <Aryl/Core/Application.h>
#include <Aryl/EntryPoint.h>

class EditorApp : public Aryl::Application
{
public:
	EditorApp()
		: Aryl::Application()
	{
	}
};

inline void Create(EditorApp*& appPtr)
{
	appPtr = new EditorApp();
}

Aryl::Application* Aryl::CreateApplication(const std::filesystem::path& appPath)
{
	EditorApp* app;
	Create(app);

	//GLFWwindow* window;

	///* Initialize the library */
	//if (!glfwInit())
	//	return -1;

	///* Create a windowed mode window and its OpenGL context */
	//window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	//if (!window)
	//{
	//	glfwTerminate();
	//	return -1;
	//}

	///* Make the window's context current */
	//glfwMakeContextCurrent(window);

	///* Loop until the user closes the window */
	//while (!glfwWindowShouldClose(window))
	//{
	//	/* Render here */
	//	glClear(GL_COLOR_BUFFER_BIT);

	//	/* Swap front and back buffers */
	//	glfwSwapBuffers(window);

	//	/* Poll for and process events */
	//	glfwPollEvents();
	//}

	//glfwTerminate();
	return 0;

	return app;
}
