#include "Editor.h"

#include "Aryl/Core/Base.h"

#include <Aryl/Events/KeyEvent.h>
#include <Aryl/Input/KeyCodes.h>

Editor::Editor()
{
	YL_ASSERT(!myInstance, "Editor already exists!");
	myInstance = this;
}

Editor::~Editor()
{
	myInstance = nullptr;
}

void Editor::OnAttach()
{

}

void Editor::OnDetach()
{

}

void Editor::OnEvent(Aryl::Event& e)
{
	if (e.GetEventType() == Aryl::AppUpdate || 
		e.GetEventType() == Aryl::AppRender || 
		e.GetEventType() == Aryl::AppImGuiUpdate) { return; }

	// Print events for debugging
	YL_CORE_TRACE(e.ToString());

	Aryl::EventDispatcher dispatcher(e);

	dispatcher.Dispatch<Aryl::KeyPressedEvent>([](Aryl::KeyPressedEvent& event)
		{
			if (event.GetKeyCode() == YL_KEY_SPACE)
			{
#ifdef YL_DEBUG
				YL_CORE_INFO("Deb: Test Space");
#elif YL_RELEASE
				YL_CORE_INFO("Rel: Test Space");
#endif
			}

			return false;
		});
}
