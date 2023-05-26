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

	void OnAttach();
	void OnDetach();
	void OnEvent(Aryl::Event& e);

	bool OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e);

	void ArylNetExample();

	void SetupSender(Aryl::IPv4Endpoint endpoint);
	void SetupReceiver(Aryl::IPv4Endpoint endpoint, std::function<void(Aryl::NetPacket)> callback);

private:
	Aryl::Entity myTestingEntity;

	Ref<Aryl::UdpSocketSender> mySender;
	Ref<Aryl::UdpSocketReceiver> myReceiver;

	std::unordered_map<std::string, Aryl::IPv4Endpoint> myConnectedClients;
	std::vector<std::string> myChatMessages;
};