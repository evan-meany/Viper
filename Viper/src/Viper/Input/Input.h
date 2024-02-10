#pragma once

#include "Viper/Core/Core.h"

namespace Viper {

	struct JoystickData
	{
		float LeftStickX;
		float LeftStickY;
		float RightStickX;
		float RightStickY;
	};

	class VIPER_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static void HideCursor(bool hide = true) { s_Instance->HideCursorImpl(hide); }
		inline static JoystickData GetJoystickData(int joystickID) { return s_Instance->GetJoystickDataImpl(joystickID);  }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual void HideCursorImpl(bool hide) = 0;
		virtual JoystickData GetJoystickDataImpl(int joystickID) = 0;

	private:
		static Input* s_Instance;
	};

}
