#pragma once

#include <Aryl/Core/Layer/Layer.h>
#include <Aryl/Events/ApplicationEvent.h>

class Game : public Aryl::Layer
{
public:
	Game();
	~Game();

	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Aryl::Event& event) override;

	bool OnUpdate(Aryl::AppUpdateEvent& e);

private:

};
