#pragma once

#include "Aryl/Events/Event.h"

namespace Aryl
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnEvent(Event&) {}
	};
}