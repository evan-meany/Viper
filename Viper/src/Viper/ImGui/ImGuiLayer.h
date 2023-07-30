#pragma once

#include "Viper/Layer.h"

namespace Viper {

	class VIPER_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& evnt);

	private:
		float m_Time = 0.0f;
	};

}

