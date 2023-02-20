#include "Editor.h"

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
	// Print events for debugging
	YL_CORE_TRACE(e.ToString());
}
