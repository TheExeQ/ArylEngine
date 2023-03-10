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
		m_layers.emplace(m_layers.begin() + m_lastInsertIndex, layer);
		m_lastInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_lastInsertIndex, layer);
		if (it != m_layers.begin() + m_lastInsertIndex)
		{
			layer->OnDetach();
			m_layers.erase(it);
			m_lastInsertIndex--;
			delete layer;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin() + m_lastInsertIndex, m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			overlay->OnDetach();
			m_layers.erase(it);
			delete overlay;
		}
	}

	void LayerStack::PopLast()
	{
		Layer* lastLayer = m_layers.back();
		PopLayer(lastLayer);
	}

	Layer* LayerStack::GetLastLayer()
	{
		if (m_layers.empty())
		{
			return nullptr;
		}
		return m_layers.back();
	}

	void LayerStack::Clear()
	{
		for (Layer* layer : m_layers)
		{
			layer->OnDetach();
			delete layer;
		}

		m_lastInsertIndex = 0;
		m_layers.clear();
	}
}