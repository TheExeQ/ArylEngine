#include "Launcher/Launcher.h"

#include <Game/Game.h>

#include <Aryl/Core/Application.h>
#include <Aryl/EntryPoint.h>

class LauncherApp : public Aryl::Application
{
public:
	LauncherApp(const Aryl::ApplicationInfo& appInfo)
		: Aryl::Application(appInfo)
	{
		Launcher* editor = new Launcher();
		Game* game = new Game();
		PushLayer(editor);
		PushLayer(game);
	}
};

inline void Create(LauncherApp*& appPtr, const Aryl::ApplicationInfo& info)
{
	appPtr = new LauncherApp(info);
}

Aryl::Application* Aryl::CreateApplication(const std::filesystem::path& appPath)
{
	Aryl::ApplicationInfo info{};
	info.title = "Game";
	info.useVSync = false;
	info.enableImGui = false;
	info.isRuntime = true;

	LauncherApp* app;
	Create(app, info);

	return app;
}
