#pragma once

#include "NetworkTesting.h"
#include "RendererTesting.h"

#include <Aryl/Core/Layer/Layer.h>
#include <Aryl/Events/ApplicationEvent.h>

class Editor : public Aryl::Layer
{
public:
	Editor();
	~Editor() override;

	void OnAttach() override;
	void OnDetach() override;

	void OnEvent(Aryl::Event& e) override;

	inline static Editor& Get() { return *myInstance; }

private:
	Ref<NetworkTesting> myNetworkTest;
	Ref<RendererTesting> myRendererTest;

	inline static Editor* myInstance = nullptr;
};