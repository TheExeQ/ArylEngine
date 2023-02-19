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

	return app;
}
