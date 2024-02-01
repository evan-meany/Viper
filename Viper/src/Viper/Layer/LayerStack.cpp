#include "vppch.h"
#include "LayerStack.h"

namespace Viper {

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) 
	{
		// Add new layers before overlays, keep track of position
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex++, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		// Always add overlays to very end of stack
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto lookup = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (lookup != m_Layers.end())
		{
			m_Layers.erase(lookup);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto lookup = std::find(m_Layers.begin(), m_Layers.end(), overlay);

		if (lookup != m_Layers.end())
		{
			m_Layers.erase(lookup);
		}
	}

} // End namespace ViperS