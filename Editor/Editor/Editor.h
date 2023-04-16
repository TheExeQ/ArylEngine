#pragma once

#include <Aryl/Core/Layer/Layer.h>
#include <Aryl/Events/ApplicationEvent.h>

#include <Aryl/Networking/Socket/UdpSocketSender.h>
#include <Aryl/Networking/Socket/UdpSocketReceiver.h>

#include <Aryl/Scene/Entity.h>

class Editor : public Aryl::Layer
{
public:
	Editor();
	~Editor() override;

	void OnAttach() override;
	void OnDetach() override;

	void OnEvent(Aryl::Event& e) override;
	bool OnRender(Aryl::AppRenderEvent& e);
	bool OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e);

	void TempOpenGLPreFrame();
	void TempOpenGLTesting();

	void ArylNetExample();

	void SetupSender(Aryl::IPv4Endpoint endpoint);
	void SetupReceiver(Aryl::IPv4Endpoint endpoint, std::function<void(Aryl::NetPacket)> callback);

	inline static Editor& Get() { return *myInstance; }

private:
	Aryl::Entity myTestingEntity;

	Ref<Aryl::UdpSocketSender> mySender;
	Ref<Aryl::UdpSocketReceiver> myReceiver;

	std::unordered_map<std::string, Aryl::IPv4Endpoint> myConnectedClients;
	std::vector<std::string> myChatMessages;

	inline static Editor* myInstance = nullptr;
};