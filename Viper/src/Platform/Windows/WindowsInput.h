#pragma once

#include "Viper/Input/Input.h"

namespace Viper {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual void HideCursorImpl(bool hide) override;
		virtual JoystickData GetJoystickDataImpl(int joystickID) override;
	};

}
