#pragma once

#include <Aryl/Core/Layer/Layer.h>
#include <Aryl/Events/ApplicationEvent.h>

class Launcher : public Aryl::Layer
{
public:
	Launcher();
	~Launcher() override;

	void OnAttach() override;
	void OnDetach() override;

	void OnEvent(Aryl::Event& e) override;
	bool OnRender(Aryl::AppRenderEvent& e);

	inline static Launcher& Get() { return *myInstance; }

private:
	inline static Launcher* myInstance = nullptr;
};