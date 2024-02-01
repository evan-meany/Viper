#pragma once

#include "Layer.h"

namespace Viper {

	class VIPER_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		// Add ability to directly iterate m_Layers in public scope
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		// m_LayerInsert is the most recent layer position
		// any layers in the stack after this are overlays
		unsigned int m_LayerInsertIndex = 0;
	};

} // End namespace Viper
