#pragma once

#include <Aryl/Events/ApplicationEvent.h>
#include "Aryl/Events/KeyEvent.h"
#include <Aryl/Scene/Entity.h>

class NetworkTesting
{
public:
	NetworkTesting();
	~NetworkTesting();

	void ReflectionTesting();
	
	void OnAttach();
	void OnDetach();
	void OnEvent(Aryl::Event& e);

	bool OnKeyDown(Aryl::KeyPressedEvent& e);
	bool OnUpdate(Aryl::AppUpdateEvent& e);
	bool OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e);

	void ArylNetExample();

private:
	Aryl::Entity myTestingEntity;
};
