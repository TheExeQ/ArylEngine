#pragma once

#include <Aryl/Events/ApplicationEvent.h>

#include <Aryl/Network/Socket/UdpSocketSender.h>
#include <Aryl/Network/Socket/UdpSocketReceiver.h>

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

	bool OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e);

	void ArylNetExample();

private:
	Aryl::Entity myTestingEntity;
};