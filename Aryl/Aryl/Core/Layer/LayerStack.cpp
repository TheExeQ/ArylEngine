#include "LayerStack.h"

#include "Aryl/Core/Layer/Layer.h"

namespace Aryl
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		Clear();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		myLayers.emplace(myLayers.begin() + myLastInsertIndex, layer);
		myLastInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		myLayers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(myLayers.begin(), myLayers.begin() + myLastInsertIndex, layer);
		if (it != myLayers.begin() + myLastInsertIndex)
		{
			layer->OnDetach();
			myLayers.erase(it);
			myLastInsertIndex--;
			delete layer;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(myLayers.begin() + myLastInsertIndex, myLayers.end(), overlay);
		if (it != myLayers.end())
		{
			overlay->OnDetach();
			myLayers.erase(it);
			delete overlay;
		}
	}

	void LayerStack::PopLast()
	{
		Layer* lastLayer = myLayers.back();
		PopLayer(lastLayer);
	}

	Layer* LayerStack::GetLastLayer()
	{
		if (myLayers.empty())
		{
			return nullptr;
		}
		return myLayers.back();
	}

	void LayerStack::Clear()
	{
		for (Layer* layer : myLayers)
		{
			layer->OnDetach();
			delete layer;
		}

		myLastInsertIndex = 0;
		myLayers.clear();
	}
}