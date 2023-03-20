#pragma once

#include <Aryl/Core/Layer/Layer.h>
#include <Aryl/Events/ApplicationEvent.h>

#include <Aryl/Networking/Socket/UdpSocket.h>

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

	inline static Editor& Get() { return *myInstance; }

private:
	Ref<Aryl::UdpSocket> mySocket;

	inline static Editor* myInstance = nullptr;
};