#pragma once

#include "Viper/Layer.h"

#include "Viper/Events/KeyEvent.h"
#include "Viper/Events/MouseEvent.h"
#include "Viper/Events/ApplicationEvent.h"

namespace Viper {

	class VIPER_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}

