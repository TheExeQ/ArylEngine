#pragma once

#include "Aryl/Core/Layer/Layer.h"

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
	inline static Editor* myInstance = nullptr;
};