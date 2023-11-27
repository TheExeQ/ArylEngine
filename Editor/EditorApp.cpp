#include "Editor/Editor.h"

#include <Game/Game.h>

#include <Aryl/Core/Application.h>
#include <Aryl/EntryPoint.h>

class EditorApp : public Aryl::Application
{
public:
	EditorApp(const Aryl::ApplicationInfo& appInfo)
		: Aryl::Application(appInfo)
	{
		Editor* editor = new Editor();
		Game* game = new Game();
		PushLayer(game);
		PushLayer(editor);
	}
};

inline void Create(EditorApp*& appPtr, const Aryl::ApplicationInfo& info)
{
	appPtr = new EditorApp(info);
}

Aryl::Application* Aryl::CreateApplication(const std::filesystem::path& appPath)
{
	Aryl::ApplicationInfo info{};
	info.title = "Aryl Editor";
	info.headless = true;
	info.useVSync = false;

#ifndef _WINDOWS
	info.headless = true;
#endif

	EditorApp* app;
	Create(app, info);

	return app;
}
