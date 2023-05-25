#pragma once

#include <Aryl/Core/Layer/Layer.h>
#include <Aryl/Events/ApplicationEvent.h>

#include <Aryl/Networking/Socket/UdpSocketSender.h>
#include <Aryl/Networking/Socket/UdpSocketReceiver.h>

#include <Aryl/Renderer/Renderer.h>
#include <Aryl/Renderer/Shader.h>
#include <Aryl/Renderer/Buffer.h>
#include <Aryl/Renderer/Texture.h>
#include <Aryl/Renderer/VertexArray.h>
#include <Aryl/Renderer/Camera/Camera.h>

#include <Aryl/Renderer/Renderer.h>

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
	bool OnUpdate(Aryl::AppUpdateEvent& e);
	bool OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e);

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

	glm::mat4 myCubeTransform;
	Ref<Aryl::Camera> myOrthoCamera;
	Ref<Aryl::Camera> myPerspCamera;
	Ref<Aryl::Texture2D> myTexture;
	Ref<Aryl::Shader> myShader;
	Ref<Aryl::VertexArray> mySquareVertexArray;

	inline static Editor* myInstance = nullptr;
};